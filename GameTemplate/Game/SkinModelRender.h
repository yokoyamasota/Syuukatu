#pragma once

class Model;
class Light;

//class GaussianBlur;

class SkinModelRender : public IGameObject
{
private:
	/// <summary>
	/// クラスのポインタ
	/// </summary>
	

	Light* m_light = nullptr;

	/*GaussianBlur* gaussianBlur = nullptr;*/

	//影描画用のライトカメラ
	Camera m_lightCamera;

	RenderTarget m_shadowMap;
	float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };

	ModelInitData m_modelInitData;		//モデルイニットデータ構造体のデータを作成
	Model m_model;						//モデルクラスを作成
	RenderTarget mainRenderTarget;
	RenderTarget luminnceRenderTarget;

	ModelInitData m_shadowModelInitData;		// シャドウマップに描画するモデルを初期化する
	Model m_shadowModel;						//モデルクラスを作成

	ModelInitData m_shadowRecieverInitData;		// シャドウマップに描画するモデルを初期化する
	Model m_shadowReciever;						//モデルクラスを作成

	SpriteInitData luminanceSpriteInitData;
	SpriteInitData finalSpriteInitData;
	SpriteInitData spriteInitData;

	Sprite luminanceSprite;
	Sprite finalSprite;
	Sprite copyToFrameBufferSprite;

	Vector3    m_pos = Vector3::Zero;			//位置
	Quaternion m_rot = Quaternion::Identity;	//回転
	Vector3    m_sca = Vector3::One;			//拡大率
	bool m_shadowRecieverFlg = false;

	bool Start()override final;
	~SkinModelRender()override final;
	void Update()override final;

public:
	//モデルを初期化する関数
	void Init(const char* tkmFilePath, bool shadowFlg);
	//描画関数
	void Render(RenderContext& rc);

	/// <summary>
	/// セッター
	/// </summary>
	//モデルの位置を設定する関数
	void SetPosition(const Vector3 pos) { m_pos = pos; }
	//モデルの回転を設定する関数
	void SetRotation(const Quaternion rot) { m_rot = rot; }
	//モデルの拡大率を設定する関数
	void SetScale(const Vector3 sca) { m_sca = sca; }
	/// <summary>
	/// ゲッター
	/// </summary>
	//モデルの位置を取得する関数
	Vector3 GetPosition()const { return m_pos; }
	/// <summary>
	/// 回転クォータニオンを取得。
	/// </summary>
	/// <returns>回転クォータニオン</returns>
	Quaternion GetRotation()const { return m_rot; }
	//モデルのアドレスを取得する関数
	Model* GetModel() { return &m_model; }
	RenderTarget* GetMainRenderTarget() { return &mainRenderTarget; }
};

