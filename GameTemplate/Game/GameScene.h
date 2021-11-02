#pragma once

class GameScene : public IGameObject
{
private:

	/// <summary>
	/// クラスのポインタ
	/// </summary>
	
	SkinModelRender * m_skinModel = nullptr;
	SkinModelRender* m_bgModel = nullptr;
	/*Light* m_light = nullptr;*/

	Vector3 m_pos;


public:

	bool Start()override;
	~GameScene()override;
	void Update()override;

};

