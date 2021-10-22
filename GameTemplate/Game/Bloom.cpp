#include "stdafx.h"
#include "Bloom.h"
//#include "GaussianBlur.h"

void Bloom::Init(RenderTarget& mainRenderTarget)
{
	//解像度、ミップマップレベル
	luminnceRenderTarget.Create(
		mainRenderTarget.GetWidth(),		//解像度はメインレンダリングターゲットと同じ。
		mainRenderTarget.GetHeight(),		//解像度はメインレンダリングターゲットと同じ。
		1,
		1,
		/*DXGI_FORMAT_R32G32B32A32_FLOAT,*/
		mainRenderTarget.GetColorBufferFormat(),
		DXGI_FORMAT_D32_FLOAT
	);

	
	//輝度抽出用のシェーダーのファイルパスを指定する。
	luminanceSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect.fx";
	//頂点シェーダーのエントリーポイントを指定する。
	luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
	//ピクセルシェーダーのエントリーポイントを指定する。
	luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
	//スプライトの幅と高さはluminnceRenderTargetと同じ。
	luminanceSpriteInitData.m_width = mainRenderTarget.GetWidth();
	luminanceSpriteInitData.m_height = mainRenderTarget.GetHeight();
	//テクスチャはメインレンダリングターゲットのカラーバッファ。
	luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	//描き込むレンダリングターゲットのフォーマットを指定する。
	luminanceSpriteInitData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();

	
	luminanceSprite.Init(luminanceSpriteInitData);


	//ガウシアンブラーを初期化
	gaussianBlur.Init(&luminnceRenderTarget.GetRenderTargetTexture());

	
	finalSpriteInitData.m_textures[0] = &gaussianBlur.GetBokeTexture();
	//解像度はmainRenderTargetの幅と高さ。
	finalSpriteInitData.m_width = mainRenderTarget.GetWidth();
	finalSpriteInitData.m_height = mainRenderTarget.GetHeight();
	//ぼかした画像を、通常の2Dとしてメインレンダリングターゲットに描画するので、
	//2D用のシェーダーを使用する。
	finalSpriteInitData.m_fxFilePath = "Assets/shader/2D.fx";
	//ただし、加算合成で描画するので、アルファブレンディングモードを加算にする。
	finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
	//カラーバッファのフォーマットは例によって、32ビット浮動小数点バッファ。
	finalSpriteInitData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();

	//初期化情報を元に加算合成用のスプライトを初期化する。
	finalSprite.Init(finalSpriteInitData);

	
	//テクスチャはmainRenderTargetのカラーバッファ。
	spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	spriteInitData.m_width = mainRenderTarget.GetWidth();
	spriteInitData.m_height = mainRenderTarget.GetHeight();
	//モノクロ用のシェーダーを指定する。
	spriteInitData.m_fxFilePath = "Assets/shader/2D.fx";

	
	copyToFrameBufferSprite.Init(spriteInitData);


}

void Bloom::RenderToLuminnceRenderTarget(RenderContext& rc, RenderTarget& mainRenderTarget)
{
	//輝度抽出
		//輝度抽出用のレンダリングターゲットに変更。
	rc.WaitUntilToPossibleSetRenderTarget(luminnceRenderTarget);
	//レンダリングターゲットを設定。
	rc.SetRenderTargetAndViewport(luminnceRenderTarget);
	//レンダリングターゲットをクリア。
	rc.ClearRenderTargetView(luminnceRenderTarget);
	//輝度抽出を行う。
	luminanceSprite.Draw(rc);
	//レンダリングターゲットへの書き込み終了待ち。
	rc.WaitUntilFinishDrawingToRenderTarget(luminnceRenderTarget);


	//ガウシアンブラーを実行する
	gaussianBlur.ExecuteOnGPU(rc, 1);

	//ボケ画像をメインレンダリングターゲットに加算合成
	//レンダリングターゲットとして利用できるまで待つ。
	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	//レンダリングターゲットを設定。
	rc.SetRenderTargetAndViewport(mainRenderTarget);
	//最終合成。
	finalSprite.Draw(rc);
	//レンダリングターゲットへの書き込み終了待ち。
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

	// メインレンダリングターゲットの絵をフレームバッファーにコピー
	rc.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

	copyToFrameBufferSprite.Draw(rc);

}