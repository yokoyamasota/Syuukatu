#include "stdafx.h"
#include "ShadowMap.h"


void ShadowMap::Init()
{
	
	m_shadowMapRender.Create(
		1024,//�����_�����O�^�[�Q�b�g�̉���
		1024,//�����_�����O�^�[�Q�b�g�̏c��
		1,
		1,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);
}



void ShadowMap::RenderToShadowMap(RenderContext& rc, Camera& lightCamera)
{
	rc.WaitUntilToPossibleSetRenderTarget(m_shadowMapRender);
	rc.SetRenderTargetAndViewport(m_shadowMapRender);
	rc.ClearRenderTargetView(m_shadowMapRender);

	for (auto* shadowModel : m_models) {
		// �e���f����`��
		shadowModel->Draw(rc, lightCamera);
	}

	// �������݊����҂�
	rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMapRender);

}