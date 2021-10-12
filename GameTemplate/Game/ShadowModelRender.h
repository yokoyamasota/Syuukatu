#pragma once

class Model;
class Camera;

class ShadowModelRender : public IGameObject
{
private:
	/// <summary>
	/// �N���X�̃|�C���^
	/// </summary>

	//�e�`��p�̃��C�g�J����
	Camera m_lightCamera ;

	RenderTarget m_shadowMap;
	float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };


	ModelInitData m_shadowModelInitData;		// �V���h�E�}�b�v�ɕ`�悷�郂�f��������������
	ModelInitData m_shadowRecieverInitData;		// �V���h�E�}�b�v�ɕ`�悷�郂�f��������������
	Model m_shadowModel;						//���f���N���X���쐬
	Model m_shadowReciever;						//���f���N���X���쐬

	Vector3    m_pos = Vector3::Zero;			//�ʒu
	Quaternion m_rot = Quaternion::Identity;	//��]
	Vector3    m_sca = Vector3::One;			//�g�嗦

	bool Start()override final;
	~ShadowModelRender()override final;
	void Update()override final;

public:
	//���f��������������֐�
	void Init(const char* tkmFilePath);
	//���f��������������֐�
	void InitR(const char* tkmFilePath);
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
	Model* GetModel() { return &m_shadowModel; }
	Model* GetRModel() { return &m_shadowReciever; }
	RenderTarget* GetShadowMap() { return &m_shadowMap; }
};

