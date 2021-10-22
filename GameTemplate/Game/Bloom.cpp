#include "stdafx.h"
#include "Bloom.h"
//#include "GaussianBlur.h"

void Bloom::Init(RenderTarget& mainRenderTarget)
{
	//�𑜓x�A�~�b�v�}�b�v���x��
	luminnceRenderTarget.Create(
		mainRenderTarget.GetWidth(),		//�𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ����B
		mainRenderTarget.GetHeight(),		//�𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ����B
		1,
		1,
		/*DXGI_FORMAT_R32G32B32A32_FLOAT,*/
		mainRenderTarget.GetColorBufferFormat(),
		DXGI_FORMAT_D32_FLOAT
	);

	
	//�P�x���o�p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��B
	luminanceSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect.fx";
	//���_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
	luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
	//�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
	luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
	//�X�v���C�g�̕��ƍ�����luminnceRenderTarget�Ɠ����B
	luminanceSpriteInitData.m_width = mainRenderTarget.GetWidth();
	luminanceSpriteInitData.m_height = mainRenderTarget.GetHeight();
	//�e�N�X�`���̓��C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�B
	luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	//�`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��B
	luminanceSpriteInitData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();

	
	luminanceSprite.Init(luminanceSpriteInitData);


	//�K�E�V�A���u���[��������
	gaussianBlur.Init(&luminnceRenderTarget.GetRenderTargetTexture());

	
	finalSpriteInitData.m_textures[0] = &gaussianBlur.GetBokeTexture();
	//�𑜓x��mainRenderTarget�̕��ƍ����B
	finalSpriteInitData.m_width = mainRenderTarget.GetWidth();
	finalSpriteInitData.m_height = mainRenderTarget.GetHeight();
	//�ڂ������摜���A�ʏ��2D�Ƃ��ă��C�������_�����O�^�[�Q�b�g�ɕ`�悷��̂ŁA
	//2D�p�̃V�F�[�_�[���g�p����B
	finalSpriteInitData.m_fxFilePath = "Assets/shader/2D.fx";
	//�������A���Z�����ŕ`�悷��̂ŁA�A���t�@�u�����f�B���O���[�h�����Z�ɂ���B
	finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
	//�J���[�o�b�t�@�̃t�H�[�}�b�g�͗�ɂ���āA32�r�b�g���������_�o�b�t�@�B
	finalSpriteInitData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();

	//�������������ɉ��Z�����p�̃X�v���C�g������������B
	finalSprite.Init(finalSpriteInitData);

	
	//�e�N�X�`����mainRenderTarget�̃J���[�o�b�t�@�B
	spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	spriteInitData.m_width = mainRenderTarget.GetWidth();
	spriteInitData.m_height = mainRenderTarget.GetHeight();
	//���m�N���p�̃V�F�[�_�[���w�肷��B
	spriteInitData.m_fxFilePath = "Assets/shader/2D.fx";

	
	copyToFrameBufferSprite.Init(spriteInitData);


}

void Bloom::RenderToLuminnceRenderTarget(RenderContext& rc, RenderTarget& mainRenderTarget)
{
	//�P�x���o
		//�P�x���o�p�̃����_�����O�^�[�Q�b�g�ɕύX�B
	rc.WaitUntilToPossibleSetRenderTarget(luminnceRenderTarget);
	//�����_�����O�^�[�Q�b�g��ݒ�B
	rc.SetRenderTargetAndViewport(luminnceRenderTarget);
	//�����_�����O�^�[�Q�b�g���N���A�B
	rc.ClearRenderTargetView(luminnceRenderTarget);
	//�P�x���o���s���B
	luminanceSprite.Draw(rc);
	//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
	rc.WaitUntilFinishDrawingToRenderTarget(luminnceRenderTarget);


	//�K�E�V�A���u���[�����s����
	gaussianBlur.ExecuteOnGPU(rc, 1);

	//�{�P�摜�����C�������_�����O�^�[�Q�b�g�ɉ��Z����
	//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂B
	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	//�����_�����O�^�[�Q�b�g��ݒ�B
	rc.SetRenderTargetAndViewport(mainRenderTarget);
	//�ŏI�����B
	finalSprite.Draw(rc);
	//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

	// ���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�[�ɃR�s�[
	rc.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

	copyToFrameBufferSprite.Draw(rc);

}