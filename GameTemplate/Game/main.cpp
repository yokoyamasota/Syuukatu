#include "stdafx.h"
#include "system/system.h"
#include "GameScene.h"


namespace
{
	const int PRIORITY_0 = 0;	//�D��x0
}

///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// �������珉�������s���R�[�h���L�q����B
	//////////////////////////////////////
	
	//�e�`��p�̃��C�g�J����
	Camera lightCamera;
	//�J�����̈ʒu��ݒ�B����̓��C�g�̈ʒu�B
	lightCamera.SetPosition(200, 500, 500);
	//�J�����̒����_��ݒ�B���ꂪ���C�g���Ƃ炵�Ă���ꏊ�B
	lightCamera.SetTarget(0, 50, 0);
	//������ݒ�
	lightCamera.SetUp({ 1.0f,0.0f,0.0f });
	//��p
	lightCamera.SetViewAngle(Math::DegToRad(50.0f));
	//���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���B
	lightCamera.Update();

	//�V���h�E�}�b�v�`��p�̃����_�����O�^�[�Q�b�g
	//�J���[�o�b�t�@�̃N���A�J���[
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };//��
	RenderTarget shadowMap;
	shadowMap.Create(
		1024,//�����_�����O�^�[�Q�b�g�̉���
		1024,//�����_�����O�^�[�Q�b�g�̏c��
		1,
		1,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);

	RenderTarget mainRenderTarget;
	mainRenderTarget.Create(
		1280,
		720,
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);


	RenderTarget luminnceRenderTarget;
	//�𑜓x�A�~�b�v�}�b�v���x��
	luminnceRenderTarget.Create(
		1280,		//�𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ����B
		720,		//�𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ����B
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);
	

	// �V���h�E�}�b�v�ɕ`�悷�郂�f��������������
	ModelInitData shadowModelInitData;

	// �V���h�E�}�b�v�`��p�̃V�F�[�_�[���w�肷��
	shadowModelInitData.m_fxFilePath = "Assets/shader/DrawShadowMap.fx";
	shadowModelInitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	// �J���[�o�b�t�@�̃t�H�[�}�b�g�ɕύX���������̂ŁA��������ύX����B
	shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;
	Model shadowModel;
	shadowModel.Init(shadowModelInitData);
	

	// �e���󂯂�w�i���f����������
	ModelInitData bgModelInitData;
	bgModelInitData.m_fxFilePath = "Assets/shader/ShadowReciever.fx";
	bgModelInitData.m_expandShaderResoruceView[0] = &shadowMap.GetRenderTargetTexture();
	bgModelInitData.m_expandConstantBuffer = (void*)&lightCamera.GetViewProjectionMatrix();
	bgModelInitData.m_expandConstantBufferSize = sizeof(lightCamera.GetViewProjectionMatrix());
	bgModelInitData.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";

	Model bgModel;
	bgModel.Init(bgModelInitData);

	SpriteInitData luminanceSpriteInitData;
	//�P�x���o�p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��B
	luminanceSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect.fx";
	//���_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
	luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
	//�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
	luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
	//�X�v���C�g�̕��ƍ�����luminnceRenderTarget�Ɠ����B
	luminanceSpriteInitData.m_width = 1280;
	luminanceSpriteInitData.m_height = 720;
	//�e�N�X�`���̓��C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�B
	luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	//�`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��B
	luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	Sprite luminanceSprite;
	luminanceSprite.Init(luminanceSpriteInitData);

	//�K�E�V�A���u���[�I�u�W�F�N�g����
	GaussianBlur gaussianBlur;
	//�K�E�V�A���u���[��������
	gaussianBlur.Init(&luminnceRenderTarget.GetRenderTargetTexture());

	SpriteInitData finalSpriteInitData;
	finalSpriteInitData.m_textures[0] = &gaussianBlur.GetBokeTexture();
	//�𑜓x��mainRenderTarget�̕��ƍ����B
	finalSpriteInitData.m_width = 1280;
	finalSpriteInitData.m_height = 720;
	//�ڂ������摜���A�ʏ��2D�Ƃ��ă��C�������_�����O�^�[�Q�b�g�ɕ`�悷��̂ŁA
	//2D�p�̃V�F�[�_�[���g�p����B
	finalSpriteInitData.m_fxFilePath = "Assets/shader/2D.fx";
	//�������A���Z�����ŕ`�悷��̂ŁA�A���t�@�u�����f�B���O���[�h�����Z�ɂ���B
	finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
	//�J���[�o�b�t�@�̃t�H�[�}�b�g�͗�ɂ���āA32�r�b�g���������_�o�b�t�@�B
	finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	Sprite finalSprite;
	//�������������ɉ��Z�����p�̃X�v���C�g������������B
	finalSprite.Init(finalSpriteInitData);

	SpriteInitData spriteInitData;
	//�e�N�X�`����mainRenderTarget�̃J���[�o�b�t�@�B
	spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	spriteInitData.m_width = 1280;
	spriteInitData.m_height = 720;
	//���m�N���p�̃V�F�[�_�[���w�肷��B
	spriteInitData.m_fxFilePath = "Assets/shader/2D.fx";

	Sprite copyToFrameBufferSprite;
	copyToFrameBufferSprite.Init(spriteInitData);

	Vector3 m_pos;
	m_pos = { 0,0,0 };

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X���쐬����B
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();

	/*g_camera3D->SetPosition({ 0.0f, 130.0f, 600.0f });
	g_camera3D->SetTarget({ 0.0f, 130.0f, 0.0f });*/

	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();


	//�Q�[���J�n
	GameScene* m_gameScene = NewGO<GameScene>(PRIORITY_0, "GameScene");

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		//�����_�����O�J�n�B
		g_engine->BeginFrame();
		
		/*Quaternion qAddRot;
		qAddRot.SetRotationDegX(g_pad[0]->GetLStickYF());
		g_camera3D->RotateOriginTarget(qAddRot);*/

		//SkinModelRender * m_skinModel = nullptr;
		//m_skinModel->SetPosition(m_pos);

		shadowModel.UpdateWorldMatrix(
			m_pos,
			g_quatIdentity,
			g_vec3One
		);
		float LStickXF;
		float LStickYF;

		LStickXF = g_pad[0]->GetLStickXF();
		LStickYF = g_pad[0]->GetLStickYF();
		m_pos.x -= LStickXF;
		m_pos.z -= LStickYF;
		/*if (g_pad[0]->GetLStickXF())
		{
			m_pos.x++;
		}
		if (g_pad[0]->GetLStickYF())
		{
			m_pos.z++;
		}*/

		//////////////////////////////////////
		//��������G��`���R�[�h���L�q����B
		//////////////////////////////////////
		
		GameObjectManager::GetInstance()->ExecuteUpdate();
	
		
		//////////////////////////////////////
		//�G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////
		// 
		// �V���h�E�}�b�v�Ƀ����_�����O
		// �����_�����O�^�[�Q�b�g���V���h�E�}�b�v�ɕύX����
		renderContext.WaitUntilToPossibleSetRenderTarget(shadowMap);
		renderContext.SetRenderTargetAndViewport(shadowMap);
		renderContext.ClearRenderTargetView(shadowMap);

		// �e���f����`��
		shadowModel.Draw(renderContext, lightCamera);

		// �������݊����҂�
		renderContext.WaitUntilFinishDrawingToRenderTarget(shadowMap);


		/*renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);*/
		//�����_�����O�^�[�Q�b�g��ݒ�B
		renderContext.SetRenderTargetAndViewport(mainRenderTarget);
		//�����_�����O�^�[�Q�b�g���N���A�B
		renderContext.ClearRenderTargetView(mainRenderTarget);

		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		bgModel.Draw(renderContext);

		//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
		renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

		// �u���[���̏���

		//�P�x���o
		//�P�x���o�p�̃����_�����O�^�[�Q�b�g�ɕύX�B
		renderContext.WaitUntilToPossibleSetRenderTarget(luminnceRenderTarget);
		//�����_�����O�^�[�Q�b�g��ݒ�B
		renderContext.SetRenderTargetAndViewport(luminnceRenderTarget);
		//�����_�����O�^�[�Q�b�g���N���A�B
		renderContext.ClearRenderTargetView(luminnceRenderTarget);
		//�P�x���o���s���B
		luminanceSprite.Draw(renderContext);
		//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
		renderContext.WaitUntilFinishDrawingToRenderTarget(luminnceRenderTarget);


		//�K�E�V�A���u���[�����s����
		gaussianBlur.ExecuteOnGPU(renderContext, 1);

		//�{�P�摜�����C�������_�����O�^�[�Q�b�g�ɉ��Z����
		//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂B
		renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		//�����_�����O�^�[�Q�b�g��ݒ�B
		renderContext.SetRenderTargetAndViewport(mainRenderTarget);
		//�ŏI�����B
		finalSprite.Draw(renderContext);
		//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
		renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

		// ���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�[�ɃR�s�[
		renderContext.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		renderContext.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
		
		//�e���󂯂�w�i��`��
		/*GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		bgModel.Draw(renderContext);*/
		copyToFrameBufferSprite.Draw(renderContext);

		g_engine->EndFrame();
	}
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();
	return 0;
}
