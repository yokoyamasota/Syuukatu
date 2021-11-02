#include "stdafx.h"
#include "system/system.h"
#include "GameScene.h"
#include "ShadowMap.h"
//#include "GaussianBlur.h"
#include "Bloom.h"


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
	//
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

	//�V���h�E�}�b�v�̏�����
	ShadowMap shadowMap;
	shadowMap.Init();


	RenderTarget mainRenderTarget;
	mainRenderTarget.Create(
		1280,
		720,
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

	//// �V���h�E�}�b�v�ɕ`�悷�郂�f��������������
	//ModelInitData shadowModelInitData;

	//// �V���h�E�}�b�v�`��p�̃V�F�[�_�[���w�肷��
	//shadowModelInitData.m_fxFilePath = "Assets/shader/DrawShadowMap.fx";
	//shadowModelInitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	//// �J���[�o�b�t�@�̃t�H�[�}�b�g�ɕύX���������̂ŁA��������ύX����B
	//shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;
	//Model shadowModel;
	//shadowModel.Init(shadowModelInitData);
	//shadowMap.AddShadowModel(&shadowModel);

	

	//// �e���󂯂�w�i���f����������
	//ModelInitData bgModelInitData;
	//bgModelInitData.m_fxFilePath = "Assets/shader/ShadowReciever.fx";
	//bgModelInitData.m_expandShaderResoruceView[0] = &shadowMap.GetShadowMap();
	//bgModelInitData.m_expandConstantBuffer = (void*)&lightCamera.GetViewProjectionMatrix();
	//bgModelInitData.m_expandConstantBufferSize = sizeof(lightCamera.GetViewProjectionMatrix());
	//bgModelInitData.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";

	//Model bgModel;
	//bgModel.Init(bgModelInitData);

	//�u���[���̏�����
	Bloom bloom;
	bloom.Init(mainRenderTarget);

	SpriteInitData spriteInitData;
	//�e�N�X�`����mainRenderTarget�̃J���[�o�b�t�@�B
	spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	spriteInitData.m_width = mainRenderTarget.GetWidth();
	spriteInitData.m_height = mainRenderTarget.GetHeight();
	//���m�N���p�̃V�F�[�_�[���w�肷��B
	spriteInitData.m_fxFilePath = "Assets/shader/2D.fx";

	Sprite copyToFrameBufferSprite;
	copyToFrameBufferSprite.Init(spriteInitData);

	/*Vector3 m_pos;
	m_pos = { 0,0,0 };*/

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
	/*GameScene* m_gameScene = NewGO<GameScene>(PRIORITY_0, "GameScene");*/
	SkinModelRender* m_skinModelRender = NewGO<SkinModelRender>(PRIORITY_0, nullptr);
	Light* m_light = NewGO<Light>(PRIORITY_0, "Light");
	SkinModelRender* m_skinModel =nullptr;
	SkinModelRender* m_bgModel = nullptr;
	//////���C�g�I�u�W�F�N�g����
	////Light* m_light = nullptr;
	//�f�B���N�V�������C�g���C�g���Z�b�g
	m_light->SetDirectionLightData();

	m_skinModel = NewGO<SkinModelRender>(PRIORITY_0, nullptr);
	m_skinModel->Init("Assets/modelData/unityChan.tkm", false, &shadowMap,&lightCamera);
	m_bgModel = NewGO<SkinModelRender>(1, nullptr);
	m_bgModel->Init("Assets/modelData/bg/bg.tkm", true, &shadowMap,&lightCamera);
	//shadowMap.AddShadowModel(m_skinModelRender->GetShadowModel());

	//// �e���󂯂�w�i���f����������
	//ModelInitData bgModelInitData;
	//bgModelInitData.m_fxFilePath = "Assets/shader/ShadowReciever.fx";
	//bgModelInitData.m_expandShaderResoruceView[0] = &shadowMap.GetShadowMap();
	//bgModelInitData.m_expandConstantBuffer = (void*)&lightCamera.GetViewProjectionMatrix();
	//bgModelInitData.m_expandConstantBufferSize = sizeof(lightCamera.GetViewProjectionMatrix());
	//bgModelInitData.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";

	//Model bgModel;
	//bgModel.Init(bgModelInitData);
	

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		//�����_�����O�J�n�B
		g_engine->BeginFrame();
		
		/*Quaternion qAddRot;
		qAddRot.SetRotationDegX(g_pad[0]->GetLStickYF());
		g_camera3D->RotateOriginTarget(qAddRot);*/
		
		/*shadowModel.UpdateWorldMatrix(
			m_pos,
			g_quatIdentity,
			g_vec3One
		);
		float LStickXF;
		float LStickYF;

		LStickXF = g_pad[0]->GetLStickXF();
		LStickYF = g_pad[0]->GetLStickYF();
		m_pos.x -= LStickXF;
		m_pos.z -= LStickYF;*/

		//////////////////////////////////////
		//��������G��`���R�[�h���L�q����B
		//////////////////////////////////////
		
		GameObjectManager::GetInstance()->ExecuteUpdate();
	
		
		//////////////////////////////////////
		//�G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////
		
		// �V���h�E�}�b�v�Ƀ����_�����O
		/*shadowMap.RenderToShadowMap(renderContext, m_skinModelRender->GetLightCamera());*/
		shadowMap.RenderToShadowMap(renderContext, lightCamera);
		


		/*renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);*/
		//�����_�����O�^�[�Q�b�g��ݒ�B
		renderContext.SetRenderTargetAndViewport(mainRenderTarget);
		//�����_�����O�^�[�Q�b�g���N���A�B
		renderContext.ClearRenderTargetView(mainRenderTarget);

		GameObjectManager::GetInstance()->ExecuteRender(renderContext);

		//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
		renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

		// �u���[���̏���
		bloom.RenderToLuminnceRenderTarget(renderContext, mainRenderTarget);

		// ���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�[�ɃR�s�[
		renderContext.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		renderContext.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

		copyToFrameBufferSprite.Draw(renderContext);

		g_engine->EndFrame();
	}
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();
	return 0;
}
