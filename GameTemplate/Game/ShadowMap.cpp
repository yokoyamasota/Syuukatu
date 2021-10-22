#include "stdafx.h"
#include "ShadowMap.h"


void ShadowMap::Init()
{
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };//��
	
	m_shadowMap.Create(
		1024,//�����_�����O�^�[�Q�b�g�̉���
		1024,//�����_�����O�^�[�Q�b�g�̏c��
		1,
		1,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);
	

}


void ShadowMap::RenderToShadowMap(RenderContext& rc,Camera& lightCamera)
{
	rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
	rc.SetRenderTargetAndViewport(m_shadowMap);
	rc.ClearRenderTargetView(m_shadowMap);

	for (auto* shadowModel : m_models) {
		// �e���f����`��
		shadowModel->Draw(rc, lightCamera);
	}

	// �������݊����҂�
	rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);

}