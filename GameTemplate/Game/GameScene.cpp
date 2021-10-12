#include "stdafx.h"
#include "GameScene.h"

namespace
{
	const int PRIORITY_0 = 0;	//優先度0

	const Vector3 SKIN_POS = { 0.0f, 0.0f,0.0f };		//表示位置
	const Vector3 SKIN_SCA = { 1.0f, 1.0f,1.0f };			//大きさ

	const Vector3 SKIN_POS1 = { 0.0f, 50.0f,0.0f };		//表示位置
}

bool GameScene::Start()
{
	//ライトオブジェクト生成
	m_light = NewGO<Light>(PRIORITY_0, "Light");
	//ディレクションライトライトをセット
	m_light->SetDirectionLightData();

	/*m_shadowModel = NewGO<ShadowModelRender>(PRIORITY_0, nullptr);
	m_shadowModel->Init("Assets/modelData/unityChan.tkm");*/

	m_skinModel = NewGO<SkinModelRender>(PRIORITY_0, nullptr);
	m_skinModel->Init("Assets/modelData/unityChan.tkm");

	/*m_bgModel = NewGO<ShadowModelRender>(PRIORITY_0, nullptr);
	m_bgModel->InitR("Assets/modelData/bg/bg.tkm");*/
	
	/*m_bgModel = NewGO<SkinModelRender>(PRIORITY_0, nullptr);
	m_bgModel->Init("Assets/modelData/bg/bg.tkm");*/

	m_skinModel->SetPosition({ SKIN_POS });
	m_skinModel->SetScale({ SKIN_SCA });

	/*m_shadowModel->SetPosition({ SKIN_POS1 });
	m_shadowModel->SetScale({ SKIN_SCA });*/

	/*m_bgModel->SetPosition({ SKIN_POS });
	m_bgModel->SetScale({ SKIN_SCA });*/

	//Start関数のreturn文
	return true;
}

GameScene::~GameScene()
{
	DeleteGO(m_light);

}

void GameScene::Update()
{
	if (g_pad[0]->IsPress(enButtonA))
	{
		m_pos.x++;
	}
	if (g_pad[0]->IsPress(enButtonB))
	{
		m_pos.x--;
	}

	m_skinModel->SetPosition(m_pos);
	/*m_shadowModel->SetPosition(m_pos);*/
	/*m_bgModel->SetPosition(m_pos);*/
	

}