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

	//�e�`��p�̃��C�g�J����
	Camera lightCamera;
	ShadowMap shadowMap;
	RenderTarget mainRenderTarget;
	Bloom bloom;

	SpriteInitData spriteInitData;
	Sprite copyToFrameBufferSprite;

};

