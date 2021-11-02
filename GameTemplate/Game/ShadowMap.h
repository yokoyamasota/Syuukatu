#pragma once
class ShadowMap
{
public:
	void Init();

	void RenderToShadowMap(RenderContext& rc,Camera& lightCamera);

	/// <summary>
	/// �e���f����ǉ��B
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
	// �V���h�E�}�b�v�ɕ`�悷�郂�f��������������
	ModelInitData shadowModelInitData;
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };//��
};

