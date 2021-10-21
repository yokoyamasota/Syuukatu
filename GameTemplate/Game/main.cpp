#include "stdafx.h"
#include "system/system.h"
#include "GameScene.h"


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

	RenderTarget mainRenderTarget;
	mainRenderTarget.Create(
		1280,
		720,
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);


	RenderTarget luminnceRenderTarget;
	//解像度、ミップマップレベル
	luminnceRenderTarget.Create(
		1280,		//解像度はメインレンダリングターゲットと同じ。
		720,		//解像度はメインレンダリングターゲットと同じ。
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
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
	

	// 影を受ける背景モデルを初期化
	ModelInitData bgModelInitData;
	bgModelInitData.m_fxFilePath = "Assets/shader/ShadowReciever.fx";
	bgModelInitData.m_expandShaderResoruceView[0] = &shadowMap.GetRenderTargetTexture();
	bgModelInitData.m_expandConstantBuffer = (void*)&lightCamera.GetViewProjectionMatrix();
	bgModelInitData.m_expandConstantBufferSize = sizeof(lightCamera.GetViewProjectionMatrix());
	bgModelInitData.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";

	Model bgModel;
	bgModel.Init(bgModelInitData);

	SpriteInitData luminanceSpriteInitData;
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

	Sprite luminanceSprite;
	luminanceSprite.Init(luminanceSpriteInitData);

	//ガウシアンブラーオブジェクト生成
	GaussianBlur gaussianBlur;
	//ガウシアンブラーを初期化
	gaussianBlur.Init(&luminnceRenderTarget.GetRenderTargetTexture());

	SpriteInitData finalSpriteInitData;
	finalSpriteInitData.m_textures[0] = &gaussianBlur.GetBokeTexture();
	//解像度はmainRenderTargetの幅と高さ。
	finalSpriteInitData.m_width = 1280;
	finalSpriteInitData.m_height = 720;
	//ぼかした画像を、通常の2Dとしてメインレンダリングターゲットに描画するので、
	//2D用のシェーダーを使用する。
	finalSpriteInitData.m_fxFilePath = "Assets/shader/2D.fx";
	//ただし、加算合成で描画するので、アルファブレンディングモードを加算にする。
	finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
	//カラーバッファのフォーマットは例によって、32ビット浮動小数点バッファ。
	finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	Sprite finalSprite;
	//初期化情報を元に加算合成用のスプライトを初期化する。
	finalSprite.Init(finalSpriteInitData);

	SpriteInitData spriteInitData;
	//テクスチャはmainRenderTargetのカラーバッファ。
	spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	spriteInitData.m_width = 1280;
	spriteInitData.m_height = 720;
	//モノクロ用のシェーダーを指定する。
	spriteInitData.m_fxFilePath = "Assets/shader/2D.fx";

	Sprite copyToFrameBufferSprite;
	copyToFrameBufferSprite.Init(spriteInitData);

	Vector3 m_pos;
	m_pos = { 0,0,0 };

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
	GameScene* m_gameScene = NewGO<GameScene>(PRIORITY_0, "GameScene");

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//レンダリング開始。
		g_engine->BeginFrame();
		
		/*Quaternion qAddRot;
		qAddRot.SetRotationDegX(g_pad[0]->GetLStickYF());
		g_camera3D->RotateOriginTarget(qAddRot);*/

		//SkinModelRender * m_skinModel = nullptr;
		//m_skinModel->SetPosition(m_pos);

		shadowModel.UpdateWorldMatrix(
			m_pos,
			g_quatIdentity,
			g_vec3One
		);
		float LStickXF;
		float LStickYF;

		LStickXF = g_pad[0]->GetLStickXF();
		LStickYF = g_pad[0]->GetLStickYF();
		m_pos.x -= LStickXF;
		m_pos.z -= LStickYF;
		/*if (g_pad[0]->GetLStickXF())
		{
			m_pos.x++;
		}
		if (g_pad[0]->GetLStickYF())
		{
			m_pos.z++;
		}*/

		//////////////////////////////////////
		//ここから絵を描くコードを記述する。
		//////////////////////////////////////
		
		GameObjectManager::GetInstance()->ExecuteUpdate();
	
		
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


		/*renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);*/
		//レンダリングターゲットを設定。
		renderContext.SetRenderTargetAndViewport(mainRenderTarget);
		//レンダリングターゲットをクリア。
		renderContext.ClearRenderTargetView(mainRenderTarget);

		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		bgModel.Draw(renderContext);

		//レンダリングターゲットへの書き込み終了待ち。
		renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

		// ブルームの処理

		//輝度抽出
		//輝度抽出用のレンダリングターゲットに変更。
		renderContext.WaitUntilToPossibleSetRenderTarget(luminnceRenderTarget);
		//レンダリングターゲットを設定。
		renderContext.SetRenderTargetAndViewport(luminnceRenderTarget);
		//レンダリングターゲットをクリア。
		renderContext.ClearRenderTargetView(luminnceRenderTarget);
		//輝度抽出を行う。
		luminanceSprite.Draw(renderContext);
		//レンダリングターゲットへの書き込み終了待ち。
		renderContext.WaitUntilFinishDrawingToRenderTarget(luminnceRenderTarget);


		//ガウシアンブラーを実行する
		gaussianBlur.ExecuteOnGPU(renderContext, 1);

		//ボケ画像をメインレンダリングターゲットに加算合成
		//レンダリングターゲットとして利用できるまで待つ。
		renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		//レンダリングターゲットを設定。
		renderContext.SetRenderTargetAndViewport(mainRenderTarget);
		//最終合成。
		finalSprite.Draw(renderContext);
		//レンダリングターゲットへの書き込み終了待ち。
		renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

		// メインレンダリングターゲットの絵をフレームバッファーにコピー
		renderContext.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		renderContext.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
		
		//影を受ける背景を描画
		/*GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		bgModel.Draw(renderContext);*/
		copyToFrameBufferSprite.Draw(renderContext);

		g_engine->EndFrame();
	}
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	return 0;
}
