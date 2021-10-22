#pragma once

class Model;
class Light;

//class GaussianBlur;

class SkinModelRender : public IGameObject
{
private:
	/// <summary>
	/// �N���X�̃|�C���^
	/// </summary>
	

	Light* m_light = nullptr;

	/*GaussianBlur* gaussianBlur = nullptr;*/

	//�e�`��p�̃��C�g�J����
	Camera m_lightCamera;

	RenderTarget m_shadowMap;
	float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };

	ModelInitData m_modelInitData;		//���f���C�j�b�g�f�[�^�\���̂̃f�[�^���쐬
	Model m_model;						//���f���N���X���쐬
	RenderTarget mainRenderTarget;
	RenderTarget luminnceRenderTarget;

	ModelInitData m_shadowModelInitData;		// �V���h�E�}�b�v�ɕ`�悷�郂�f��������������
	Model m_shadowModel;						//���f���N���X���쐬

	ModelInitData m_shadowRecieverInitData;		// �V���h�E�}�b�v�ɕ`�悷�郂�f��������������
	Model m_shadowReciever;						//���f���N���X���쐬

	SpriteInitData luminanceSpriteInitData;
	SpriteInitData finalSpriteInitData;
	SpriteInitData spriteInitData;

	Sprite luminanceSprite;
	Sprite finalSprite;
	Sprite copyToFrameBufferSprite;

	Vector3    m_pos = Vector3::Zero;			//�ʒu
	Quaternion m_rot = Quaternion::Identity;	//��]
	Vector3    m_sca = Vector3::One;			//�g�嗦
	bool m_shadowRecieverFlg = false;

	bool Start()override final;
	~SkinModelRender()override final;
	void Update()override final;

public:
	//���f��������������֐�
	void Init(const char* tkmFilePath, bool shadowFlg);
	//�`��֐�
	void Render(RenderContext& rc);

	/// <summary>
	/// �Z�b�^�[
	/// </summary>
	//���f���̈ʒu��ݒ肷��֐�
	void SetPosition(const Vector3 pos) { m_pos = pos; }
	//���f���̉�]��ݒ肷��֐�
	void SetRotation(const Quaternion rot) { m_rot = rot; }
	//���f���̊g�嗦��ݒ肷��֐�
	void SetScale(const Vector3 sca) { m_sca = sca; }
	/// <summary>
	/// �Q�b�^�[
	/// </summary>
	//���f���̈ʒu���擾����֐�
	Vector3 GetPosition()const { return m_pos; }
	/// <summary>
	/// ��]�N�H�[�^�j�I�����擾�B
	/// </summary>
	/// <returns>��]�N�H�[�^�j�I��</returns>
	Quaternion GetRotation()const { return m_rot; }
	//���f���̃A�h���X���擾����֐�
	Model* GetModel() { return &m_model; }
	RenderTarget* GetMainRenderTarget() { return &mainRenderTarget; }
};

