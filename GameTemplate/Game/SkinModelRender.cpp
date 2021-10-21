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
	//モデルの位置、回転、拡大を更新
	m_model.UpdateWorldMatrix(m_pos, m_rot, m_sca);
}

//モデルを初期化する関数
void SkinModelRender::Init(const char* tkmFilePath, bool shadowFlg)
{

	if (shadowFlg == false) {
		m_shadowRecieverFlg = false;
		m_light = FindGO<Light>("Light");

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

		////tkmファイルのファイルパスを設定する
		//m_shadowModelInitData.m_tkmFilePath = tkmFilePath;
		////使用するシェーダーファイルパスを設定する
		//m_shadowModelInitData.m_fxFilePath = "Assets/shader/DrawShadowMap.fx";
		////// カラーバッファのフォーマットに変更が入ったので、こちらも変更する。
		//m_shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;
		////初期化情報を使ってモデル表示処理を初期化する
		//m_shadowModel.Init(m_shadowModelInitData);

		/*mainRenderTarget.Create(
			1280,
			720,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);*/

		////解像度、ミップマップレベル
		//luminnceRenderTarget.Create(
		//	1280,		//解像度はメインレンダリングターゲットと同じ。
		//	720,		//解像度はメインレンダリングターゲットと同じ。
		//	1,
		//	1,
		//	DXGI_FORMAT_R32G32B32A32_FLOAT,
		//	DXGI_FORMAT_D32_FLOAT
		//);
		//m_shadowMap.Create(
		//	1024,//レンダリングターゲットの横幅
		//	1024,//レンダリングターゲットの縦幅
		//	1,
		//	1,
		//	DXGI_FORMAT_R32_FLOAT,
		//	DXGI_FORMAT_D32_FLOAT,
		//	clearColor
		//);

		////カメラの位置を設定。これはライトの位置。
		//m_lightCamera.SetPosition(0, 100, 0);
		////カメラの注視点を設定。これがライトが照らしている場所。
		//m_lightCamera.SetTarget(0, 0, 0);
		////方向を設定
		//m_lightCamera.SetUp({ 1.0f,0.0f,0.0f });
		////画角
		//m_lightCamera.SetViewAngle(Math::DegToRad(50.0f));
		////ライトビュープロジェクション行列を計算している。
		//m_lightCamera.Update();

		////輝度抽出用のシェーダーのファイルパスを指定する。
		//luminanceSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect.fx";
		////頂点シェーダーのエントリーポイントを指定する。
		//luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
		////ピクセルシェーダーのエントリーポイントを指定する。
		//luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
		////スプライトの幅と高さはluminnceRenderTargetと同じ。
		//luminanceSpriteInitData.m_width = 1280;
		//luminanceSpriteInitData.m_height = 720;
		////テクスチャはメインレンダリングターゲットのカラーバッファ。
		//luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
		////描き込むレンダリングターゲットのフォーマットを指定する。
		//luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		//luminanceSprite.Init(luminanceSpriteInitData);

		////ガウシアンブラーオブジェクト生成
		//gaussianBlur = NewGO<GaussianBlur>(0, "GaussianBlur");
		////ガウシアンブラーを初期化
		//gaussianBlur->Init(&luminnceRenderTarget.GetRenderTargetTexture());

		//finalSpriteInitData.m_textures[0] = &gaussianBlur->GetBokeTexture();
		////解像度はmainRenderTargetの幅と高さ。
		//finalSpriteInitData.m_width = 1280;
		//finalSpriteInitData.m_height = 720;
		////ぼかした画像を、通常の2Dとしてメインレンダリングターゲットに描画するので、
		////2D用のシェーダーを使用する。
		//finalSpriteInitData.m_fxFilePath = "Assets/shader/2D.fx";
		////ただし、加算合成で描画するので、アルファブレンディングモードを加算にする。
		//finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
		////カラーバッファのフォーマットは例によって、32ビット浮動小数点バッファ。
		//finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		////初期化情報を元に加算合成用のスプライトを初期化する。
		//finalSprite.Init(finalSpriteInitData);

		////テクスチャはmainRenderTargetのカラーバッファ。
		//spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
		//spriteInitData.m_width = 1280;
		//spriteInitData.m_height = 720;
		////モノクロ用のシェーダーを指定する。
		//spriteInitData.m_fxFilePath = "Assets/shader/2D.fx";

		//copyToFrameBufferSprite.Init(spriteInitData);
	}
	if (shadowFlg == true)
	{
		//m_shadowRecieverFlg = true;
		//m_shadowMap.Create(
		//	1024,//レンダリングターゲットの横幅
		//	1024,//レンダリングターゲットの縦幅
		//	1,
		//	1,
		//	DXGI_FORMAT_R32_FLOAT,
		//	DXGI_FORMAT_D32_FLOAT,
		//	clearColor
		//);
		////カメラの位置を設定。これはライトの位置。
		//m_lightCamera.SetPosition(0, 100, 0);
		////カメラの注視点を設定。これがライトが照らしている場所。
		//m_lightCamera.SetTarget(0, 0, 0);
		////方向を設定
		//m_lightCamera.SetUp({ 1.0f,0.0f,0.0f });
		////画角
		//m_lightCamera.SetViewAngle(Math::DegToRad(50.0f));
		////ライトビュープロジェクション行列を計算している。
		//m_lightCamera.Update();

		////tkmファイルのファイルパスを設定する
		//m_shadowRecieverInitData.m_tkmFilePath = tkmFilePath;
		//m_shadowRecieverInitData.m_fxFilePath = "Assets/shader/ShadowReciever.fx";
		//m_shadowRecieverInitData.m_expandShaderResoruceView[0] = &m_shadowMap.GetRenderTargetTexture();
		//m_shadowRecieverInitData.m_expandConstantBuffer = (void*)&m_lightCamera.GetViewProjectionMatrix();
		//m_shadowRecieverInitData.m_expandConstantBufferSize = sizeof(m_lightCamera.GetViewProjectionMatrix());

		//m_shadowReciever.Init(m_shadowRecieverInitData);
	}

}

//描画関数
void SkinModelRender::Render(RenderContext& rc)
{

	/*if (m_shadowRecieverFlg == false) {*/


		//モデルのドローコールを実行し、表示する
		m_model.Draw(rc);

		////レンダリングターゲットへの書き込み終了待ち。
		//rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

		////輝度抽出
		////輝度抽出用のレンダリングターゲットに変更。
		//rc.WaitUntilToPossibleSetRenderTarget(luminnceRenderTarget);
		////レンダリングターゲットを設定。
		//rc.SetRenderTargetAndViewport(luminnceRenderTarget);
		////レンダリングターゲットをクリア。
		//rc.ClearRenderTargetView(luminnceRenderTarget);
		////輝度抽出を行う。
		//luminanceSprite.Draw(rc);
		////レンダリングターゲットへの書き込み終了待ち。
		//rc.WaitUntilFinishDrawingToRenderTarget(luminnceRenderTarget);


		////ガウシアンブラーを実行する
		//gaussianBlur->ExecuteOnGPU(rc, 1);

		////ボケ画像をメインレンダリングターゲットに加算合成
		////レンダリングターゲットとして利用できるまで待つ。
		//rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		////レンダリングターゲットを設定。
		//rc.SetRenderTargetAndViewport(mainRenderTarget);
		////最終合成。
		//finalSprite.Draw(rc);
		////レンダリングターゲットへの書き込み終了待ち。
		//rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

		//// メインレンダリングターゲットの絵をフレームバッファーにコピー
		//rc.SetRenderTarget(
		//	g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		//	g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		//);

		//m_model.Draw(rc);
		//copyToFrameBufferSprite.Draw(rc);
	/*}*/

	//if (m_shadowRecieverFlg == true) {
	//	// シャドウマップにレンダリング
	//		// レンダリングターゲットをシャドウマップに変更する
	//	rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
	//	rc.SetRenderTargetAndViewport(m_shadowMap);
	//	rc.ClearRenderTargetView(m_shadowMap);

	//	//モデルのドローコールを実行し、表示する
	//	m_shadowModel.Draw(rc, m_lightCamera);

	//	// 書き込み完了待ち
	//	rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);
	
	//// メインレンダリングターゲットの絵をフレームバッファーにコピー
	//rc.SetRenderTarget(
	//	g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
	//	g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	//);
	//rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
	/*copyToFrameBufferSprite.Draw(rc);*/

	/*m_model.Draw(rc);*/
	/*m_shadowReciever.Draw(rc);*/
	
	
}