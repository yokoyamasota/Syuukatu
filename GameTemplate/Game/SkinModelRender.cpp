#include "stdafx.h"

bool SkinModelRender::Start()
{
	return true;
}

SkinModelRender::~SkinModelRender()
{

}

void SkinModelRender::Update()
{
	//���f���̈ʒu�A��]�A�g����X�V
	m_model.UpdateWorldMatrix(m_pos, m_rot, m_sca);
}

//���f��������������֐�
void SkinModelRender::Init(const char* tkmFilePath, bool shadowFlg)
{

	if (shadowFlg == false) {
		m_shadowRecieverFlg = false;
		m_light = FindGO<Light>("Light");

		//tkm�t�@�C���̃t�@�C���p�X��ݒ肷��
		m_modelInitData.m_tkmFilePath = tkmFilePath;
		//�g�p����V�F�[�_�[�t�@�C���p�X��ݒ肷��
		m_modelInitData.m_fxFilePath = "Assets/shader/model.fx";
		/*���f���̏���������萔�o�b�t�@�Ƃ��ăf�B�X�N���v�^�q�[�v��
		  �o�^���邽�߂Ƀ��f���̏��������Ƃ��ēn��					*/
		m_modelInitData.m_expandConstantBuffer = m_light->GetLightAdoress();
		m_modelInitData.m_expandConstantBufferSize = sizeof(m_light->GetLight());
		//�����������g���ă��f���\������������������
		m_model.Init(m_modelInitData);

		////tkm�t�@�C���̃t�@�C���p�X��ݒ肷��
		//m_shadowModelInitData.m_tkmFilePath = tkmFilePath;
		////�g�p����V�F�[�_�[�t�@�C���p�X��ݒ肷��
		//m_shadowModelInitData.m_fxFilePath = "Assets/shader/DrawShadowMap.fx";
		////// �J���[�o�b�t�@�̃t�H�[�}�b�g�ɕύX���������̂ŁA��������ύX����B
		//m_shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;
		////�����������g���ă��f���\������������������
		//m_shadowModel.Init(m_shadowModelInitData);

		/*mainRenderTarget.Create(
			1280,
			720,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);*/

		////�𑜓x�A�~�b�v�}�b�v���x��
		//luminnceRenderTarget.Create(
		//	1280,		//�𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ����B
		//	720,		//�𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ����B
		//	1,
		//	1,
		//	DXGI_FORMAT_R32G32B32A32_FLOAT,
		//	DXGI_FORMAT_D32_FLOAT
		//);
		//m_shadowMap.Create(
		//	1024,//�����_�����O�^�[�Q�b�g�̉���
		//	1024,//�����_�����O�^�[�Q�b�g�̏c��
		//	1,
		//	1,
		//	DXGI_FORMAT_R32_FLOAT,
		//	DXGI_FORMAT_D32_FLOAT,
		//	clearColor
		//);

		////�J�����̈ʒu��ݒ�B����̓��C�g�̈ʒu�B
		//m_lightCamera.SetPosition(0, 100, 0);
		////�J�����̒����_��ݒ�B���ꂪ���C�g���Ƃ炵�Ă���ꏊ�B
		//m_lightCamera.SetTarget(0, 0, 0);
		////������ݒ�
		//m_lightCamera.SetUp({ 1.0f,0.0f,0.0f });
		////��p
		//m_lightCamera.SetViewAngle(Math::DegToRad(50.0f));
		////���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���B
		//m_lightCamera.Update();

		////�P�x���o�p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��B
		//luminanceSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect.fx";
		////���_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
		//luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
		////�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
		//luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
		////�X�v���C�g�̕��ƍ�����luminnceRenderTarget�Ɠ����B
		//luminanceSpriteInitData.m_width = 1280;
		//luminanceSpriteInitData.m_height = 720;
		////�e�N�X�`���̓��C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�B
		//luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
		////�`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��B
		//luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		//luminanceSprite.Init(luminanceSpriteInitData);

		////�K�E�V�A���u���[�I�u�W�F�N�g����
		//gaussianBlur = NewGO<GaussianBlur>(0, "GaussianBlur");
		////�K�E�V�A���u���[��������
		//gaussianBlur->Init(&luminnceRenderTarget.GetRenderTargetTexture());

		//finalSpriteInitData.m_textures[0] = &gaussianBlur->GetBokeTexture();
		////�𑜓x��mainRenderTarget�̕��ƍ����B
		//finalSpriteInitData.m_width = 1280;
		//finalSpriteInitData.m_height = 720;
		////�ڂ������摜���A�ʏ��2D�Ƃ��ă��C�������_�����O�^�[�Q�b�g�ɕ`�悷��̂ŁA
		////2D�p�̃V�F�[�_�[���g�p����B
		//finalSpriteInitData.m_fxFilePath = "Assets/shader/2D.fx";
		////�������A���Z�����ŕ`�悷��̂ŁA�A���t�@�u�����f�B���O���[�h�����Z�ɂ���B
		//finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
		////�J���[�o�b�t�@�̃t�H�[�}�b�g�͗�ɂ���āA32�r�b�g���������_�o�b�t�@�B
		//finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		////�������������ɉ��Z�����p�̃X�v���C�g������������B
		//finalSprite.Init(finalSpriteInitData);

		////�e�N�X�`����mainRenderTarget�̃J���[�o�b�t�@�B
		//spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
		//spriteInitData.m_width = 1280;
		//spriteInitData.m_height = 720;
		////���m�N���p�̃V�F�[�_�[���w�肷��B
		//spriteInitData.m_fxFilePath = "Assets/shader/2D.fx";

		//copyToFrameBufferSprite.Init(spriteInitData);
	}
	if (shadowFlg == true)
	{
		//m_shadowRecieverFlg = true;
		//m_shadowMap.Create(
		//	1024,//�����_�����O�^�[�Q�b�g�̉���
		//	1024,//�����_�����O�^�[�Q�b�g�̏c��
		//	1,
		//	1,
		//	DXGI_FORMAT_R32_FLOAT,
		//	DXGI_FORMAT_D32_FLOAT,
		//	clearColor
		//);
		////�J�����̈ʒu��ݒ�B����̓��C�g�̈ʒu�B
		//m_lightCamera.SetPosition(0, 100, 0);
		////�J�����̒����_��ݒ�B���ꂪ���C�g���Ƃ炵�Ă���ꏊ�B
		//m_lightCamera.SetTarget(0, 0, 0);
		////������ݒ�
		//m_lightCamera.SetUp({ 1.0f,0.0f,0.0f });
		////��p
		//m_lightCamera.SetViewAngle(Math::DegToRad(50.0f));
		////���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���B
		//m_lightCamera.Update();

		////tkm�t�@�C���̃t�@�C���p�X��ݒ肷��
		//m_shadowRecieverInitData.m_tkmFilePath = tkmFilePath;
		//m_shadowRecieverInitData.m_fxFilePath = "Assets/shader/ShadowReciever.fx";
		//m_shadowRecieverInitData.m_expandShaderResoruceView[0] = &m_shadowMap.GetRenderTargetTexture();
		//m_shadowRecieverInitData.m_expandConstantBuffer = (void*)&m_lightCamera.GetViewProjectionMatrix();
		//m_shadowRecieverInitData.m_expandConstantBufferSize = sizeof(m_lightCamera.GetViewProjectionMatrix());

		//m_shadowReciever.Init(m_shadowRecieverInitData);
	}

}

//�`��֐�
void SkinModelRender::Render(RenderContext& rc)
{

	/*if (m_shadowRecieverFlg == false) {*/


		//���f���̃h���[�R�[�������s���A�\������
		m_model.Draw(rc);

		////�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
		//rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

		////�P�x���o
		////�P�x���o�p�̃����_�����O�^�[�Q�b�g�ɕύX�B
		//rc.WaitUntilToPossibleSetRenderTarget(luminnceRenderTarget);
		////�����_�����O�^�[�Q�b�g��ݒ�B
		//rc.SetRenderTargetAndViewport(luminnceRenderTarget);
		////�����_�����O�^�[�Q�b�g���N���A�B
		//rc.ClearRenderTargetView(luminnceRenderTarget);
		////�P�x���o���s���B
		//luminanceSprite.Draw(rc);
		////�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
		//rc.WaitUntilFinishDrawingToRenderTarget(luminnceRenderTarget);


		////�K�E�V�A���u���[�����s����
		//gaussianBlur->ExecuteOnGPU(rc, 1);

		////�{�P�摜�����C�������_�����O�^�[�Q�b�g�ɉ��Z����
		////�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂B
		//rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		////�����_�����O�^�[�Q�b�g��ݒ�B
		//rc.SetRenderTargetAndViewport(mainRenderTarget);
		////�ŏI�����B
		//finalSprite.Draw(rc);
		////�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
		//rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

		//// ���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�[�ɃR�s�[
		//rc.SetRenderTarget(
		//	g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		//	g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		//);

		//m_model.Draw(rc);
		//copyToFrameBufferSprite.Draw(rc);
	/*}*/

	//if (m_shadowRecieverFlg == true) {
	//	// �V���h�E�}�b�v�Ƀ����_�����O
	//		// �����_�����O�^�[�Q�b�g���V���h�E�}�b�v�ɕύX����
	//	rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
	//	rc.SetRenderTargetAndViewport(m_shadowMap);
	//	rc.ClearRenderTargetView(m_shadowMap);

	//	//���f���̃h���[�R�[�������s���A�\������
	//	m_shadowModel.Draw(rc, m_lightCamera);

	//	// �������݊����҂�
	//	rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);
	
	//// ���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�[�ɃR�s�[
	//rc.SetRenderTarget(
	//	g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
	//	g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	//);
	//rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
	/*copyToFrameBufferSprite.Draw(rc);*/

	/*m_model.Draw(rc);*/
	/*m_shadowReciever.Draw(rc);*/
	
	
}