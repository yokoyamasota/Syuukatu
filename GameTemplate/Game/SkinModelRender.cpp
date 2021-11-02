#include "stdafx.h"
#include "ShadowMap.h"

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
	m_shadowModel.UpdateWorldMatrix(m_pos, m_rot, m_sca);
}

//���f��������������֐�
void SkinModelRender::Init(const char* tkmFilePath, bool m_shadowRecieverFlg,ShadowMap* shadowMap,Camera* m_lightCamera)
{

	if (m_shadowRecieverFlg == false) {
		m_light = FindGO<Light>("Light");

		////�J�����̈ʒu��ݒ�B����̓��C�g�̈ʒu�B
		//m_lightCamera.SetPosition(0, 100, 0);
		////�J�����̒����_��ݒ�B���ꂪ���C�g���Ƃ炵�Ă���ꏊ�B
		//m_lightCamera.SetTarget(0, 50, 0);
		////������ݒ�
		//m_lightCamera.SetUp({ 1.0f,0.0f,0.0f });
		////��p
		//m_lightCamera.SetViewAngle(Math::DegToRad(50.0f));
		////���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���B
		//m_lightCamera.Update();

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

		//tkm�t�@�C���̃t�@�C���p�X��ݒ肷��
		m_shadowModelInitData.m_tkmFilePath = tkmFilePath;
		//�g�p����V�F�[�_�[�t�@�C���p�X��ݒ肷��
		m_shadowModelInitData.m_fxFilePath = "Assets/shader/DrawShadowMap.fx";
		// �J���[�o�b�t�@�̃t�H�[�}�b�g�ɕύX���������̂ŁA��������ύX����B
		m_shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;
		//�����������g���ă��f���\������������������
		m_shadowModel.Init(m_shadowModelInitData);

		shadowMap->AddShadowModel(&m_shadowModel);

	}
	if (m_shadowRecieverFlg == true)
	{

		/*m_shadowMap.AddShadowModel(&m_shadowModel);*/

		////�J�����̈ʒu��ݒ�B����̓��C�g�̈ʒu�B
		//m_lightCamera.SetPosition(0, 100, 0);
		////�J�����̒����_��ݒ�B���ꂪ���C�g���Ƃ炵�Ă���ꏊ�B
		//m_lightCamera.SetTarget(0, 50, 0);
		////������ݒ�
		//m_lightCamera.SetUp({ 1.0f,0.0f,0.0f });
		////��p
		//m_lightCamera.SetViewAngle(Math::DegToRad(50.0f));
		////���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���B
		//m_lightCamera.Update();

		//tkm�t�@�C���̃t�@�C���p�X��ݒ肷��
		m_shadowRecieverInitData.m_tkmFilePath = tkmFilePath;
		m_shadowRecieverInitData.m_fxFilePath = "Assets/shader/ShadowReciever.fx";
		m_shadowRecieverInitData.m_expandShaderResoruceView[0] = &shadowMap->GetShadowMap();
		m_shadowRecieverInitData.m_expandConstantBuffer = (void*)&m_lightCamera->GetViewProjectionMatrix();
		m_shadowRecieverInitData.m_expandConstantBufferSize = sizeof(m_lightCamera->GetViewProjectionMatrix());

		m_model.Init(m_shadowRecieverInitData);
	}

}

//�`��֐�
void SkinModelRender::Render(RenderContext& rc)
{
	//shadowMap.RenderToShadowMap(rc, m_lightCamera);
	//���f���̃h���[�R�[�������s���A�\������
	m_model.Draw(rc);
	/*m_shadowReciever.Draw(rc);*/
}