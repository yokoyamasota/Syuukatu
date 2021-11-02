#pragma once
class ShadowMap
{
public:
	void Init();

	void RenderToShadowMap(RenderContext& rc,Camera& lightCamera);

	/// <summary>
	/// 影モデルを追加。
	/// </summary>
	/// <param name="model"></param>
	void AddShadowModel(Model* model )
	{
		m_models.push_back(model);
	}

	Texture& GetShadowMap()
	{
		return m_shadowMapRender.GetRenderTargetTexture();
	}

private:
	std::vector<Model*> m_models;
	RenderTarget m_shadowMapRender;
	// シャドウマップに描画するモデルを初期化する
	ModelInitData shadowModelInitData;
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };//白
};

