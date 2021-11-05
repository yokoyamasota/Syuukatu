#pragma once
#include "Bloom.h"

class RenderingEngine
{
public:
	void Init();
	void RenderEngine(RenderContext& rc);

	ShadowMap* GetShadowMap() {	
		return &shadowMap;
	}

	Camera* GetLightCamera() {
		return &lightCamera;
	}
		
	

private:

	//影描画用のライトカメラ
	Camera lightCamera;
	ShadowMap shadowMap;
	RenderTarget mainRenderTarget;
	Bloom bloom;

	SpriteInitData spriteInitData;
	Sprite copyToFrameBufferSprite;

};

