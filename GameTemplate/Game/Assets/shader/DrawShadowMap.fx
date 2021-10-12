/*!
 * @brief �V���h�E�}�b�v�`��p�̃V�F�[�_�[
 */

 // ���f���p�̒萔�o�b�t�@�[
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

// ���_�V�F�[�_�[�ւ̓���
struct SVSIn
{
    float4 pos : POSITION;  // ���f���̒��_���W
    float3 normal : NORMAL; // �@��
    float2 uv : TEXCOORD0;  // UV���W
};

// �s�N�Z���V�F�[�_�[�ւ̓���
struct SPSIn
{
    float4 pos : SV_POSITION;   // �X�N���[����Ԃł̃s�N�Z���̍��W
    float3 normal : NORMAL;     // �@��
    float2 uv : TEXCOORD0;      // uv���W
};

///////////////////////////////////////////////////
// �O���[�o���ϐ�
///////////////////////////////////////////////////

Texture2D<float4> g_albedo : register(t0);      // �A���x�h�}�b�v
Texture2D<float4> g_shadowMap : register(t10);  // �V���h�E�}�b�v
sampler g_sampler : register(s0);               // �T���v���[�X�e�[�g

/// <summary>
/// ���_�V�F�[�_�[
/// <summary>
SPSIn VSMain(SVSIn vsIn)
{
    // �V���h�E�}�b�v�`��p�̒��_�V�F�[�_�[������
    SPSIn psIn;
    psIn.pos = mul(mWorld, vsIn.pos);
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);
    psIn.uv = vsIn.uv;
    psIn.normal = mul(mWorld, vsIn.normal);
    return psIn;
}

/// <summary>
/// �V���h�E�}�b�v�`��p�̃s�N�Z���V�F�[�_�[
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    // �V���h�E�}�b�v��Z�l��`������
    return float4(psIn.pos.z, psIn.pos.z, psIn.pos.z, 1.0f);
}
