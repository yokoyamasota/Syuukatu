#include "stdafx.h"
#include "system/system.h"
#include "GameScene.h"


// 関数宣言
void InitRootSignature(RootSignature& rs);


namespace
{
	const int PRIORITY_0 = 0;	//優先度0
}

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// ここから初期化を行うコードを記述する。
	//////////////////////////////////////
	RootSignature rs;
	InitRootSignature(rs);

	//影描画用のライトカメラ
	Camera lightCamera;
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

	//シャドウマップ描画用のレンダリングターゲット
	//カラーバッファのクリアカラー
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };//白
	RenderTarget shadowMap;
	shadowMap.Create(
		1024,//レンダリングターゲットの横幅
		1024,//レンダリングターゲットの縦幅
		1,
		1,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);

	// シャドウマップに描画するモデルを初期化する
	ModelInitData shadowModelInitData;

	// シャドウマップ描画用のシェーダーを指定する
	shadowModelInitData.m_fxFilePath = "Assets/shader/DrawShadowMap.fx";
	shadowModelInitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	// カラーバッファのフォーマットに変更が入ったので、こちらも変更する。
	shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;
	Model shadowModel;
	shadowModel.Init(shadowModelInitData);
	Vector3 m_pos;
	m_pos = { 0,0,0 };
	

	// 影を受ける背景モデルを初期化
	ModelInitData bgModelInitData;
	bgModelInitData.m_fxFilePath = "Assets/shader/ShadowReciever.fx";
	bgModelInitData.m_expandShaderResoruceView[0] = &shadowMap.GetRenderTargetTexture();
	bgModelInitData.m_expandConstantBuffer = (void*)&lightCamera.GetViewProjectionMatrix();
	bgModelInitData.m_expandConstantBufferSize = sizeof(lightCamera.GetViewProjectionMatrix());
	bgModelInitData.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";

	Model bgModel;
	bgModel.Init(bgModelInitData);

	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();

	g_camera3D->SetPosition({ 0.0f, 130.0f, 600.0f });
	g_camera3D->SetTarget({ 0.0f, 130.0f, 0.0f });

	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();


	//ゲーム開始
	GameScene* m_gameScene = NewGO<GameScene>(PRIORITY_0, "GameScene");

	/*m_gameScene->m_renderContext(renderContext);

	void GameScene::ehehe(RenderContext& rc)
	{
		rc.WaitUntilToPossibleSetRenderTarget(shadowMap);
	}*/


	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//レンダリング開始。
		g_engine->BeginFrame();
		
		Quaternion qAddRot;
		qAddRot.SetRotationDegX(g_pad[0]->GetLStickYF());
		g_camera3D->RotateOriginTarget(qAddRot);

		/*if (g_pad[0]->IsPress(enButtonA))
		{
			m_pos.x++;
		}
		SkinModelRender * m_skinModel = nullptr;
		m_skinModel->SetPosition(m_pos);*/

		shadowModel.UpdateWorldMatrix(
			m_pos,
			g_quatIdentity,
			g_vec3One
		);
		if (g_pad[0]->IsPress(enButtonA))
		{
			m_pos.x++;
		}
		if (g_pad[0]->IsPress(enButtonB))
		{
			m_pos.x--;
		}

		//////////////////////////////////////
		//ここから絵を描くコードを記述する。
		//////////////////////////////////////
		
		GameObjectManager::GetInstance()->ExecuteUpdate();
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		
		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		// 
		// シャドウマップにレンダリング
		// レンダリングターゲットをシャドウマップに変更する
		renderContext.WaitUntilToPossibleSetRenderTarget(shadowMap);
		renderContext.SetRenderTargetAndViewport(shadowMap);
		renderContext.ClearRenderTargetView(shadowMap);

		// 影モデルを描画
		shadowModel.Draw(renderContext, lightCamera);

		// 書き込み完了待ち
		renderContext.WaitUntilFinishDrawingToRenderTarget(shadowMap);

		// 通常レンダリング
		// レンダリングターゲットをフレームバッファーに戻す
		renderContext.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		renderContext.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

		//影を受ける背景を描画
		bgModel.Draw(renderContext);

		g_engine->EndFrame();
	}
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	return 0;
}

// ルートシグネチャの初期化
void InitRootSignature(RootSignature& rs)
{
	rs.Init(D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP);
}