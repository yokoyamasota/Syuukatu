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
	//モデルの位置、回転、拡大を更新
	m_model.UpdateWorldMatrix(m_pos, m_rot, m_sca);
	m_shadowModel.UpdateWorldMatrix(m_pos, m_rot, m_sca);
}

//モデルを初期化する関数
void SkinModelRender::Init(const char* tkmFilePath, bool m_shadowRecieverFlg,ShadowMap* shadowMap,Camera* m_lightCamera)
{

	if (m_shadowRecieverFlg == false) {
		m_light = FindGO<Light>("Light");

		////カメラの位置を設定。これはライトの位置。
		//m_lightCamera.SetPosition(0, 100, 0);
		////カメラの注視点を設定。これがライトが照らしている場所。
		//m_lightCamera.SetTarget(0, 50, 0);
		////方向を設定
		//m_lightCamera.SetUp({ 1.0f,0.0f,0.0f });
		////画角
		//m_lightCamera.SetViewAngle(Math::DegToRad(50.0f));
		////ライトビュープロジェクション行列を計算している。
		//m_lightCamera.Update();

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

		//tkmファイルのファイルパスを設定する
		m_shadowModelInitData.m_tkmFilePath = tkmFilePath;
		//使用するシェーダーファイルパスを設定する
		m_shadowModelInitData.m_fxFilePath = "Assets/shader/DrawShadowMap.fx";
		// カラーバッファのフォーマットに変更が入ったので、こちらも変更する。
		m_shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;
		//初期化情報を使ってモデル表示処理を初期化する
		m_shadowModel.Init(m_shadowModelInitData);

		shadowMap->AddShadowModel(&m_shadowModel);

	}
	if (m_shadowRecieverFlg == true)
	{

		/*m_shadowMap.AddShadowModel(&m_shadowModel);*/

		////カメラの位置を設定。これはライトの位置。
		//m_lightCamera.SetPosition(0, 100, 0);
		////カメラの注視点を設定。これがライトが照らしている場所。
		//m_lightCamera.SetTarget(0, 50, 0);
		////方向を設定
		//m_lightCamera.SetUp({ 1.0f,0.0f,0.0f });
		////画角
		//m_lightCamera.SetViewAngle(Math::DegToRad(50.0f));
		////ライトビュープロジェクション行列を計算している。
		//m_lightCamera.Update();

		//tkmファイルのファイルパスを設定する
		m_shadowRecieverInitData.m_tkmFilePath = tkmFilePath;
		m_shadowRecieverInitData.m_fxFilePath = "Assets/shader/ShadowReciever.fx";
		m_shadowRecieverInitData.m_expandShaderResoruceView[0] = &shadowMap->GetShadowMap();
		m_shadowRecieverInitData.m_expandConstantBuffer = (void*)&m_lightCamera->GetViewProjectionMatrix();
		m_shadowRecieverInitData.m_expandConstantBufferSize = sizeof(m_lightCamera->GetViewProjectionMatrix());

		m_model.Init(m_shadowRecieverInitData);
	}

}

//描画関数
void SkinModelRender::Render(RenderContext& rc)
{
	//shadowMap.RenderToShadowMap(rc, m_lightCamera);
	//モデルのドローコールを実行し、表示する
	m_model.Draw(rc);
	/*m_shadowReciever.Draw(rc);*/
}