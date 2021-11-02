#include "stdafx.h"
#include "system/system.h"
#include "GameScene.h"
#include "ShadowMap.h"
//#include "GaussianBlur.h"
#include "Bloom.h"


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
	//
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

	//シャドウマップの初期化
	ShadowMap shadowMap;
	shadowMap.Init();


	RenderTarget mainRenderTarget;
	mainRenderTarget.Create(
		1280,
		720,
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

	//// シャドウマップに描画するモデルを初期化する
	//ModelInitData shadowModelInitData;

	//// シャドウマップ描画用のシェーダーを指定する
	//shadowModelInitData.m_fxFilePath = "Assets/shader/DrawShadowMap.fx";
	//shadowModelInitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	//// カラーバッファのフォーマットに変更が入ったので、こちらも変更する。
	//shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;
	//Model shadowModel;
	//shadowModel.Init(shadowModelInitData);
	//shadowMap.AddShadowModel(&shadowModel);

	

	//// 影を受ける背景モデルを初期化
	//ModelInitData bgModelInitData;
	//bgModelInitData.m_fxFilePath = "Assets/shader/ShadowReciever.fx";
	//bgModelInitData.m_expandShaderResoruceView[0] = &shadowMap.GetShadowMap();
	//bgModelInitData.m_expandConstantBuffer = (void*)&lightCamera.GetViewProjectionMatrix();
	//bgModelInitData.m_expandConstantBufferSize = sizeof(lightCamera.GetViewProjectionMatrix());
	//bgModelInitData.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";

	//Model bgModel;
	//bgModel.Init(bgModelInitData);

	//ブルームの初期化
	Bloom bloom;
	bloom.Init(mainRenderTarget);

	SpriteInitData spriteInitData;
	//テクスチャはmainRenderTargetのカラーバッファ。
	spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	spriteInitData.m_width = mainRenderTarget.GetWidth();
	spriteInitData.m_height = mainRenderTarget.GetHeight();
	//モノクロ用のシェーダーを指定する。
	spriteInitData.m_fxFilePath = "Assets/shader/2D.fx";

	Sprite copyToFrameBufferSprite;
	copyToFrameBufferSprite.Init(spriteInitData);

	/*Vector3 m_pos;
	m_pos = { 0,0,0 };*/

	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();

	/*g_camera3D->SetPosition({ 0.0f, 130.0f, 600.0f });
	g_camera3D->SetTarget({ 0.0f, 130.0f, 0.0f });*/

	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();


	//ゲーム開始
	/*GameScene* m_gameScene = NewGO<GameScene>(PRIORITY_0, "GameScene");*/
	SkinModelRender* m_skinModelRender = NewGO<SkinModelRender>(PRIORITY_0, nullptr);
	Light* m_light = NewGO<Light>(PRIORITY_0, "Light");
	SkinModelRender* m_skinModel =nullptr;
	SkinModelRender* m_bgModel = nullptr;
	//////ライトオブジェクト生成
	////Light* m_light = nullptr;
	//ディレクションライトライトをセット
	m_light->SetDirectionLightData();

	m_skinModel = NewGO<SkinModelRender>(PRIORITY_0, nullptr);
	m_skinModel->Init("Assets/modelData/unityChan.tkm", false, &shadowMap,&lightCamera);
	m_bgModel = NewGO<SkinModelRender>(1, nullptr);
	m_bgModel->Init("Assets/modelData/bg/bg.tkm", true, &shadowMap,&lightCamera);
	//shadowMap.AddShadowModel(m_skinModelRender->GetShadowModel());

	//// 影を受ける背景モデルを初期化
	//ModelInitData bgModelInitData;
	//bgModelInitData.m_fxFilePath = "Assets/shader/ShadowReciever.fx";
	//bgModelInitData.m_expandShaderResoruceView[0] = &shadowMap.GetShadowMap();
	//bgModelInitData.m_expandConstantBuffer = (void*)&lightCamera.GetViewProjectionMatrix();
	//bgModelInitData.m_expandConstantBufferSize = sizeof(lightCamera.GetViewProjectionMatrix());
	//bgModelInitData.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";

	//Model bgModel;
	//bgModel.Init(bgModelInitData);
	

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//レンダリング開始。
		g_engine->BeginFrame();
		
		/*Quaternion qAddRot;
		qAddRot.SetRotationDegX(g_pad[0]->GetLStickYF());
		g_camera3D->RotateOriginTarget(qAddRot);*/
		
		/*shadowModel.UpdateWorldMatrix(
			m_pos,
			g_quatIdentity,
			g_vec3One
		);
		float LStickXF;
		float LStickYF;

		LStickXF = g_pad[0]->GetLStickXF();
		LStickYF = g_pad[0]->GetLStickYF();
		m_pos.x -= LStickXF;
		m_pos.z -= LStickYF;*/

		//////////////////////////////////////
		//ここから絵を描くコードを記述する。
		//////////////////////////////////////
		
		GameObjectManager::GetInstance()->ExecuteUpdate();
	
		
		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		
		// シャドウマップにレンダリング
		/*shadowMap.RenderToShadowMap(renderContext, m_skinModelRender->GetLightCamera());*/
		shadowMap.RenderToShadowMap(renderContext, lightCamera);
		


		/*renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);*/
		//レンダリングターゲットを設定。
		renderContext.SetRenderTargetAndViewport(mainRenderTarget);
		//レンダリングターゲットをクリア。
		renderContext.ClearRenderTargetView(mainRenderTarget);

		GameObjectManager::GetInstance()->ExecuteRender(renderContext);

		//レンダリングターゲットへの書き込み終了待ち。
		renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

		// ブルームの処理
		bloom.RenderToLuminnceRenderTarget(renderContext, mainRenderTarget);

		// メインレンダリングターゲットの絵をフレームバッファーにコピー
		renderContext.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		renderContext.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

		copyToFrameBufferSprite.Draw(renderContext);

		g_engine->EndFrame();
	}
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	return 0;
}
