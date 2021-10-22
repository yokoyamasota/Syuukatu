#pragma once
#include "GaussianBlur.h"

class Bloom
{
public:
	void Init(RenderTarget& mainRenderTarget);
	void RenderToLuminnceRenderTarget(RenderContext& rc, RenderTarget& mainRenderTarget);

private:
	RenderTarget luminnceRenderTarget;
	GaussianBlur gaussianBlur;

	SpriteInitData luminanceSpriteInitData;
	SpriteInitData finalSpriteInitData;
	SpriteInitData spriteInitData;

	Sprite luminanceSprite;
	Sprite finalSprite;
	Sprite copyToFrameBufferSprite;

};

