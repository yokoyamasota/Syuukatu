#include "stdafx.h"
#include "RenderingEngine.h"

void RenderingEngine::Init()
{
	
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
	shadowMap.Init();


	mainRenderTarget.Create(
		1280,
		720,
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

	//�u���[���̏�����
	bloom.Init(mainRenderTarget);

	//�e�N�X�`����mainRenderTarget�̃J���[�o�b�t�@�B
	spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	spriteInitData.m_width = mainRenderTarget.GetWidth();
	spriteInitData.m_height = mainRenderTarget.GetHeight();
	//���m�N���p�̃V�F�[�_�[���w�肷��B
	spriteInitData.m_fxFilePath = "Assets/shader/2D.fx";

	copyToFrameBufferSprite.Init(spriteInitData);

}

void  RenderingEngine::RenderEngine(RenderContext& rc)
{

	shadowMap.RenderToShadowMap(rc, lightCamera);



	/*renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);*/
	//�����_�����O�^�[�Q�b�g��ݒ�B
	rc.SetRenderTargetAndViewport(mainRenderTarget);
	//�����_�����O�^�[�Q�b�g���N���A�B
	rc.ClearRenderTargetView(mainRenderTarget);

	GameObjectManager::GetInstance()->ExecuteRender(rc);

	//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

	// �u���[���̏���
	bloom.RenderToLuminnceRenderTarget(rc, mainRenderTarget);

	// ���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�[�ɃR�s�[
	rc.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

	copyToFrameBufferSprite.Draw(rc);



}
