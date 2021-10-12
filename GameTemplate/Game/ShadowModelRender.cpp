#include "stdafx.h"
#include "ShadowModelRender.h"

bool ShadowModelRender::Start()
{
	//シャドウマップ描画用のレンダリングターゲット
	//カラーバッファのクリアカラー
	//clearColor[4];//白

	m_shadowMap.Create(
		1024,//レンダリングターゲットの横幅
		1024,//レンダリングターゲットの縦幅
		1,
		1,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);

	//カメラの位置を設定。これはライトの位置。
	m_lightCamera.SetPosition(0,100,0);
	//カメラの注視点を設定。これがライトが照らしている場所。
	m_lightCamera.SetTarget(0, 0, 0);
	//方向を設定
	m_lightCamera.SetUp({ 1.0f,0.0f,0.0f });
	//画角
	m_lightCamera.SetViewAngle(Math::DegToRad(50.0f));
	//ライトビュープロジェクション行列を計算している。
	m_lightCamera.Update();

	return true;
}

ShadowModelRender::~ShadowModelRender()
{

}

void ShadowModelRender::Update()
{
	//モデルの位置、回転、拡大を更新
	m_shadowModel.UpdateWorldMatrix(m_pos, m_rot, m_sca);

}

//モデルを初期化する関数
void ShadowModelRender::Init(const char* tkmFilePath)
{
	//使用するシェーダーファイルパスを設定する
	m_shadowModelInitData.m_fxFilePath = "Assets/shader/DrawShadowMap.fx";
	//tkmファイルのファイルパスを設定する
	m_shadowModelInitData.m_tkmFilePath = tkmFilePath;
	// カラーバッファのフォーマットに変更が入ったので、こちらも変更する。
	m_shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;
	//初期化情報を使ってモデル表示処理を初期化する
	m_shadowModel.Init(m_shadowModelInitData);
}

//モデルを初期化する関数
void ShadowModelRender::InitR(const char* tkmFilePath)
{
	m_shadowRecieverInitData.m_fxFilePath = "Assets/shader/ShadowReciever.fx";
	m_shadowRecieverInitData.m_expandShaderResoruceView[0] = &m_shadowMap.GetRenderTargetTexture();
	m_shadowRecieverInitData.m_expandConstantBuffer = (void*)&m_lightCamera.GetViewProjectionMatrix();
	m_shadowRecieverInitData.m_expandConstantBufferSize = sizeof(m_lightCamera.GetViewProjectionMatrix());
	//tkmファイルのファイルパスを設定する
	m_shadowRecieverInitData.m_tkmFilePath = tkmFilePath;

	m_shadowReciever.Init(m_shadowRecieverInitData);
}

//描画関数
void ShadowModelRender::Render(RenderContext& rc)
{
	// シャドウマップにレンダリング
		// レンダリングターゲットをシャドウマップに変更する
	rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
	rc.SetRenderTargetAndViewport(m_shadowMap);
	rc.ClearRenderTargetView(m_shadowMap);

	//モデルのドローコールを実行し、表示する
	m_shadowModel.Draw(rc,m_lightCamera);

	// 書き込み完了待ち
	rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);

	// 通常レンダリング
	// レンダリングターゲットをフレームバッファーに戻す
	rc.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

	
	//モデルのドローコールを実行し、表示する
	m_shadowReciever.Draw(rc);
}