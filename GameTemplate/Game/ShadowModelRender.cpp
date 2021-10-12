#include "stdafx.h"
#include "ShadowModelRender.h"

bool ShadowModelRender::Start()
{
	//�V���h�E�}�b�v�`��p�̃����_�����O�^�[�Q�b�g
	//�J���[�o�b�t�@�̃N���A�J���[
	//clearColor[4];//��

	m_shadowMap.Create(
		1024,//�����_�����O�^�[�Q�b�g�̉���
		1024,//�����_�����O�^�[�Q�b�g�̏c��
		1,
		1,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);

	//�J�����̈ʒu��ݒ�B����̓��C�g�̈ʒu�B
	m_lightCamera.SetPosition(0,100,0);
	//�J�����̒����_��ݒ�B���ꂪ���C�g���Ƃ炵�Ă���ꏊ�B
	m_lightCamera.SetTarget(0, 0, 0);
	//������ݒ�
	m_lightCamera.SetUp({ 1.0f,0.0f,0.0f });
	//��p
	m_lightCamera.SetViewAngle(Math::DegToRad(50.0f));
	//���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���B
	m_lightCamera.Update();

	return true;
}

ShadowModelRender::~ShadowModelRender()
{

}

void ShadowModelRender::Update()
{
	//���f���̈ʒu�A��]�A�g����X�V
	m_shadowModel.UpdateWorldMatrix(m_pos, m_rot, m_sca);

}

//���f��������������֐�
void ShadowModelRender::Init(const char* tkmFilePath)
{
	//�g�p����V�F�[�_�[�t�@�C���p�X��ݒ肷��
	m_shadowModelInitData.m_fxFilePath = "Assets/shader/DrawShadowMap.fx";
	//tkm�t�@�C���̃t�@�C���p�X��ݒ肷��
	m_shadowModelInitData.m_tkmFilePath = tkmFilePath;
	// �J���[�o�b�t�@�̃t�H�[�}�b�g�ɕύX���������̂ŁA��������ύX����B
	m_shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;
	//�����������g���ă��f���\������������������
	m_shadowModel.Init(m_shadowModelInitData);
}

//���f��������������֐�
void ShadowModelRender::InitR(const char* tkmFilePath)
{
	m_shadowRecieverInitData.m_fxFilePath = "Assets/shader/ShadowReciever.fx";
	m_shadowRecieverInitData.m_expandShaderResoruceView[0] = &m_shadowMap.GetRenderTargetTexture();
	m_shadowRecieverInitData.m_expandConstantBuffer = (void*)&m_lightCamera.GetViewProjectionMatrix();
	m_shadowRecieverInitData.m_expandConstantBufferSize = sizeof(m_lightCamera.GetViewProjectionMatrix());
	//tkm�t�@�C���̃t�@�C���p�X��ݒ肷��
	m_shadowRecieverInitData.m_tkmFilePath = tkmFilePath;

	m_shadowReciever.Init(m_shadowRecieverInitData);
}

//�`��֐�
void ShadowModelRender::Render(RenderContext& rc)
{
	// �V���h�E�}�b�v�Ƀ����_�����O
		// �����_�����O�^�[�Q�b�g���V���h�E�}�b�v�ɕύX����
	rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
	rc.SetRenderTargetAndViewport(m_shadowMap);
	rc.ClearRenderTargetView(m_shadowMap);

	//���f���̃h���[�R�[�������s���A�\������
	m_shadowModel.Draw(rc,m_lightCamera);

	// �������݊����҂�
	rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);

	// �ʏ탌���_�����O
	// �����_�����O�^�[�Q�b�g���t���[���o�b�t�@�[�ɖ߂�
	rc.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

	
	//���f���̃h���[�R�[�������s���A�\������
	m_shadowReciever.Draw(rc);
}