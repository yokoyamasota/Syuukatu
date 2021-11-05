#include "stdafx.h"
#include "RenderingEngine.h"

void RenderingEngine::Init()
{
	
	//カメラの位置を設定。これはライトの位置。
	lightCamera.SetPosition(200, 500, 500);
	//カメラの注視点を設定。これがライトが照らしている場所。
	lightCamera.SetTarget(0, 50, 0);
	//方向を設定
	lightCamera.SetUp({ 1.0f,0.0f,0.0f });
	//画角
	lightCamera.SetViewAngle(Math::DegToRad(50.0f));
	//ライトビュープロジェクション行列を計算している。
	lightCamera.Update();

	//シャドウマップの初期化
	shadowMap.Init();


	mainRenderTarget.Create(
		1280,
		720,
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

	//ブルームの初期化
	bloom.Init(mainRenderTarget);

	//テクスチャはmainRenderTargetのカラーバッファ。
	spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	spriteInitData.m_width = mainRenderTarget.GetWidth();
	spriteInitData.m_height = mainRenderTarget.GetHeight();
	//モノクロ用のシェーダーを指定する。
	spriteInitData.m_fxFilePath = "Assets/shader/2D.fx";

	copyToFrameBufferSprite.Init(spriteInitData);

}

void  RenderingEngine::RenderEngine(RenderContext& rc)
{

	shadowMap.RenderToShadowMap(rc, lightCamera);



	/*renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);*/
	//レンダリングターゲットを設定。
	rc.SetRenderTargetAndViewport(mainRenderTarget);
	//レンダリングターゲットをクリア。
	rc.ClearRenderTargetView(mainRenderTarget);

	GameObjectManager::GetInstance()->ExecuteRender(rc);

	//レンダリングターゲットへの書き込み終了待ち。
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

	// ブルームの処理
	bloom.RenderToLuminnceRenderTarget(rc, mainRenderTarget);

	// メインレンダリングターゲットの絵をフレームバッファーにコピー
	rc.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

	copyToFrameBufferSprite.Draw(rc);



}
