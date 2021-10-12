#include "stdafx.h"

bool SkinModelRender::Start()
{
	//ライトオブジェクト生成
	gaussianBlur = NewGO<GaussianBlur>(0, "GaussianBlur");
	return true;
}

SkinModelRender::~SkinModelRender()
{

}

void SkinModelRender::Update()
{
	//モデルの位置、回転、拡大を更新
	m_model.UpdateWorldMatrix(m_pos, m_rot, m_sca);
}

//モデルを初期化する関数
void SkinModelRender::Init(const char* tkmFilePath)
{
	m_light = FindGO<Light>("Light");

	mainRenderTarget.Create(
		1280,
		720,
		1,
		1,
		//【注目】カラーバッファのフォーマットを32bit浮動小数点にしている。
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

	//tkmファイルのファイルパスを設定する
	m_modelInitData.m_tkmFilePath = tkmFilePath;
	//使用するシェーダーファイルパスを設定する
	m_modelInitData.m_fxFilePath = "Assets/shader/model.fx";
	/*モデルの初期化情報を定数バッファとしてディスクリプタヒープに
	  登録するためにモデルの初期化情報として渡す					*/
	m_modelInitData.m_expandConstantBuffer = m_light->GetLightAdoress();
	m_modelInitData.m_expandConstantBufferSize = sizeof(m_light->GetLight());
	//初期化情報を使ってモデル表示処理を初期化する
	m_model.Init(m_modelInitData);

	//解像度、ミップマップレベル
	luminnceRenderTarget.Create(
		1280,		//解像度はメインレンダリングターゲットと同じ。
		720,		//解像度はメインレンダリングターゲットと同じ。
		1,
		1,
		//【注目】カラーバッファのフォーマットを32bit浮動小数点にしている。
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

	//輝度抽出用のシェーダーのファイルパスを指定する。
	luminanceSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect.fx";
	//頂点シェーダーのエントリーポイントを指定する。
	luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
	//ピクセルシェーダーのエントリーポイントを指定する。
	luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
	//スプライトの幅と高さはluminnceRenderTargetと同じ。
	luminanceSpriteInitData.m_width = 1280;
	luminanceSpriteInitData.m_height = 720;
	//テクスチャはメインレンダリングターゲットのカラーバッファ。
	luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	//描き込むレンダリングターゲットのフォーマットを指定する。
	luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	luminanceSprite.Init(luminanceSpriteInitData);

	//ガウシアンブラーを初期化
	gaussianBlur->Init(&luminnceRenderTarget.GetRenderTargetTexture());

	finalSpriteInitData.m_textures[0] = &gaussianBlur->GetBokeTexture();
	//解像度はmainRenderTargetの幅と高さ。
	finalSpriteInitData.m_width = 1280;
	finalSpriteInitData.m_height = 720;
	//ぼかした画像を、通常の2Dとしてメインレンダリングターゲットに描画するので、
	//2D用のシェーダーを使用する。
	finalSpriteInitData.m_fxFilePath = "Assets/shader/sample2D.fx";
	//ただし、加算合成で描画するので、アルファブレンディングモードを加算にする。
	finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
	//カラーバッファのフォーマットは例によって、32ビット浮動小数点バッファ。
	finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	//初期化情報を元に加算合成用のスプライトを初期化する。
	finalSprite.Init(finalSpriteInitData);

	//テクスチャはmainRenderTargetのカラーバッファ。
	spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	spriteInitData.m_width = 1280;
	spriteInitData.m_height = 720;
	//モノクロ用のシェーダーを指定する。
	spriteInitData.m_fxFilePath = "Assets/shader/sample2D.fx";

	copyToFrameBufferSprite.Init(spriteInitData);

}

//描画関数
void SkinModelRender::Render(RenderContext& rc)
{

	// step-8 レンダリングターゲットをmainRenderTargetに変更する
		//レンダリングターゲットとして利用できるまで待つ。
	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	//レンダリングターゲットを設定。
	rc.SetRenderTargetAndViewport(mainRenderTarget);
	//レンダリングターゲットをクリア。
	rc.ClearRenderTargetView(mainRenderTarget);
	
	//モデルのドローコールを実行し、表示する
	m_model.Draw(rc);

	//レンダリングターゲットへの書き込み終了待ち。
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

	// step-10 輝度抽出
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


	// step-11 ガウシアンブラーを実行する
	gaussianBlur->ExecuteOnGPU(rc, 20);

	// step-12 ボケ画像をメインレンダリングターゲットに加算合成
	//レンダリングターゲットとして利用できるまで待つ。
	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	//レンダリングターゲットを設定。
	rc.SetRenderTargetAndViewport(mainRenderTarget);
	//最終合成。
	finalSprite.Draw(rc);
	//レンダリングターゲットへの書き込み終了待ち。
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

	// step-13 メインレンダリングターゲットの絵をフレームバッファーにコピー
	rc.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	copyToFrameBufferSprite.Draw(rc);
}