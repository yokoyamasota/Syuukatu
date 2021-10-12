#include "stdafx.h"
#include "system/system.h"
#include "GameScene.h"


// �֐��錾
void InitRootSignature(RootSignature& rs);


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
	RootSignature rs;
	InitRootSignature(rs);

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

	// �V���h�E�}�b�v�ɕ`�悷�郂�f��������������
	ModelInitData shadowModelInitData;

	// �V���h�E�}�b�v�`��p�̃V�F�[�_�[���w�肷��
	shadowModelInitData.m_fxFilePath = "Assets/shader/DrawShadowMap.fx";
	shadowModelInitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	// �J���[�o�b�t�@�̃t�H�[�}�b�g�ɕύX���������̂ŁA��������ύX����B
	shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;
	Model shadowModel;
	shadowModel.Init(shadowModelInitData);
	Vector3 m_pos;
	m_pos = { 0,0,0 };
	

	// �e���󂯂�w�i���f����������
	ModelInitData bgModelInitData;
	bgModelInitData.m_fxFilePath = "Assets/shader/ShadowReciever.fx";
	bgModelInitData.m_expandShaderResoruceView[0] = &shadowMap.GetRenderTargetTexture();
	bgModelInitData.m_expandConstantBuffer = (void*)&lightCamera.GetViewProjectionMatrix();
	bgModelInitData.m_expandConstantBufferSize = sizeof(lightCamera.GetViewProjectionMatrix());
	bgModelInitData.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";

	Model bgModel;
	bgModel.Init(bgModelInitData);

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X���쐬����B
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();

	g_camera3D->SetPosition({ 0.0f, 130.0f, 600.0f });
	g_camera3D->SetTarget({ 0.0f, 130.0f, 0.0f });

	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();


	//�Q�[���J�n
	GameScene* m_gameScene = NewGO<GameScene>(PRIORITY_0, "GameScene");

	/*m_gameScene->m_renderContext(renderContext);

	void GameScene::ehehe(RenderContext& rc)
	{
		rc.WaitUntilToPossibleSetRenderTarget(shadowMap);
	}*/


	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		//�����_�����O�J�n�B
		g_engine->BeginFrame();
		
		Quaternion qAddRot;
		qAddRot.SetRotationDegX(g_pad[0]->GetLStickYF());
		g_camera3D->RotateOriginTarget(qAddRot);

		/*if (g_pad[0]->IsPress(enButtonA))
		{
			m_pos.x++;
		}
		SkinModelRender * m_skinModel = nullptr;
		m_skinModel->SetPosition(m_pos);*/

		shadowModel.UpdateWorldMatrix(
			m_pos,
			g_quatIdentity,
			g_vec3One
		);
		if (g_pad[0]->IsPress(enButtonA))
		{
			m_pos.x++;
		}
		if (g_pad[0]->IsPress(enButtonB))
		{
			m_pos.x--;
		}

		//////////////////////////////////////
		//��������G��`���R�[�h���L�q����B
		//////////////////////////////////////
		
		GameObjectManager::GetInstance()->ExecuteUpdate();
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		
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

		// �ʏ탌���_�����O
		// �����_�����O�^�[�Q�b�g���t���[���o�b�t�@�[�ɖ߂�
		renderContext.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		renderContext.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

		//�e���󂯂�w�i��`��
		bgModel.Draw(renderContext);

		g_engine->EndFrame();
	}
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();
	return 0;
}

// ���[�g�V�O�l�`���̏�����
void InitRootSignature(RootSignature& rs)
{
	rs.Init(D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP);
}