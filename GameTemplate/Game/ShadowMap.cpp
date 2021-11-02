#include "stdafx.h"
#include "ShadowMap.h"


void ShadowMap::Init()
{
	
	m_shadowMapRender.Create(
		1024,//レンダリングターゲットの横幅
		1024,//レンダリングターゲットの縦幅
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
		// 影モデルを描画
		shadowModel->Draw(rc, lightCamera);
	}

	// 書き込み完了待ち
	rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMapRender);

}