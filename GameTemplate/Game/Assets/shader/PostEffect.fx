/*!
 * @brief �u���[��
 */

cbuffer cb : register(b0)
{
    float4x4 mvp;       
    float4 mulColor;    
};

struct VSInput
{
    float4 pos : POSITION;
    float2 uv  : TEXCOORD0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};

/*!
 * @brief ���_�V�F�[�_�[
 */
PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}

Texture2D<float4> mainRenderTargetTexture : register(t0); // ���C�������_�����O�^�[�Q�b�g�̃e�N�X�`��
sampler Sampler : register(s0);

/////////////////////////////////////////////////////////
// �P�x���o�p
/////////////////////////////////////////////////////////
/*!
 * @brief �P�x���o�p�̃s�N�Z���V�F�[�_�[
 */
float4 PSSamplingLuminance(PSInput In) : SV_Target0
{
    //���C�������_�����O�^�[�Q�b�g����J���[���T���v�����O
    float4 color = mainRenderTargetTexture.Sample(Sampler, In.uv);

    //�T���v�����O�����J���[�̖��邳���v�Z
    float t = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f));

    //clip()�֐��͈����̒l���}�C�i�X�ɂȂ�ƁA�ȍ~�̏������X�L�b�v����
    //�Ȃ̂ŁA�}�C�i�X�ɂȂ�ƃs�N�Z���J���[�͏o�͂���Ȃ�
    //�J���[�̖��邳��1�ȉ��Ȃ�s�N�Z���L������
    clip(t - 1.0f);

    return color;
}