/// <summary>
/// tkm�t�@�C���B
/// </summary>
/// <remarks>
/// tkm�t�@�C����3D���f���t�H�[�}�b�g�ł��B
/// ���̃N���X�𗘗p���邱�Ƃ�tkm�t�@�C�����������Ƃ��ł��܂��B�B
/// </remarks>

#pragma once

	
/// <summary>
/// tkm�t�@�C���N���X�B
/// </summary>
class  TkmFile {
public:
	/// <summary>
	/// �}�e���A��
	/// </summary>
	struct SMaterial {
		std::string albedoMapFileName;			//�A���x�h�}�b�v�̃t�@�C�����B
		std::string normalMapFileName;			//�@���}�b�v�̃t�@�C�����B
		std::string specularMapFileName;		//�X�y�L�����}�b�v�̃t�@�C�����B
		std::string reflectionMapFileName;		//���t���N�V�����}�b�v�̃t�@�C�����B
		std::string refractionMapFileName;		//���܃}�b�v�̃t�@�C�����B
		std::unique_ptr<char[]>	albedoMap;		//���[�h���ꂽ�A���x�h�}�b�v�B(dds�t�@�C��)
		unsigned int albedoMapSize;				//�A���x�h�}�b�v�̃T�C�Y�B(dds�t�@�C��)
		std::unique_ptr<char[]>	normalMap;		//���[�h���ꂽ�@���}�b�v�B(dds�t�@�C��)
		unsigned int normalMapSize;				//�@���}�b�v�̃T�C�Y�B
		std::unique_ptr<char[]>	specularMap;	//���[�h���ꂽ�X�y�L�����}�b�v�B(dds�t�@�C��)
		unsigned int specularMapSize;			//�X�y�L�����}�b�v�̃T�C�Y�B(dds�t�@�C��)
		std::unique_ptr<char[]>	reflectionMap;	//���[�h���ꂽ���t���N�V�����}�b�v�B(dds�t�@�C��)
		unsigned int reflectionMapSize;			//���t���N�V�����}�b�v�̃T�C�Y�B(dds�t�@�C��)
		std::unique_ptr<char[]>	refractionMap;	//���[�h���ꂽ���܃}�b�v�B(dds�t�@�C��)
		unsigned int refractionMapSize;			//���܃}�b�v�̃T�C�Y�B(dds�t�@�C��)
	};
	/// <summary>
	/// ���_�B
	/// </summary>
	/// <remarks>
	/// ������ҏW������A���C�g���̃V�F�[�_�[�Œ�`����Ă��钸�_�\���̂��ύX����K�v������B
	/// </remarks>
	struct SVertex {
		Vector3 pos;			//���W�B
		Vector3 normal;		//�@���B
		Vector3 tangent;		//�ڃx�N�g���B
		Vector3 binormal;		//�]�x�N�g���B
		Vector2 uv;			//UV���W�B
		int indices[4];			//�X�L���C���f�b�N�X�B
		Vector4 skinWeights;	//�X�L���E�F�C�g�B
	};
	/// <summary>
	/// 32�r�b�g�̃C���f�b�N�X�o�b�t�@�B
	/// </summary>
	struct SIndexBuffer32 {
		std::vector< uint32_t > indices;	//�C���f�b�N�X�B
	};
	/// <summary>
	/// 16�r�b�g�̃C���f�b�N�X�o�b�t�@�B
	/// </summary>
	struct SIndexbuffer16 {
		std::vector< uint16_t > indices;	//�C���f�b�N�X�B
	};
	/// <summary>
	/// ���b�V���p�[�c�B
	/// </summary>
	struct SMesh {
		bool isFlatShading;							//�t���b�g�V�F�[�f�B���O�H
		std::vector< SMaterial > materials;				//�}�e���A���̔z��B
		std::vector< SVertex >	vertexBuffer;			//���_�o�b�t�@�B
		std::vector<SIndexBuffer32> indexBuffer32Array;	//�C���f�b�N�X�o�b�t�@�̔z��B�}�e���A���̐��������C���f�b�N�X�o�b�t�@�͂����B
		std::vector< SIndexbuffer16> indexBuffer16Array;
	};
		
	/// <summary>
	/// 3D���f�������[�h�B
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X�B</param>
	void Load(const char* filePath);
		
	/// <summary>
	/// ���b�V���p�[�c�ɑ΂��ăN�G�����s���B
	/// </summary>
	/// <param name="func">�N�G���֐�</param>
	void QueryMeshParts(std::function<void(const SMesh& mesh)> func) const
	{
		for (auto& mesh : m_meshParts) {
			func(mesh);
		}
	}
	/// <summary>
	/// ���b�V���p�[�c���擾�B
	/// </summary>
	/// <returns></returns>
	const std::vector< SMesh>& GetMeshParts() const
	{
		return m_meshParts;
	}
	/// <summary>
	/// ���b�V���̐����擾�B
	/// </summary>
	/// <returns></returns>
	int GetNumMesh() const
	{
		return (int)(m_meshParts.size());
	}
private:
	/// <summary>
	/// �e�N�X�`���������[�h�B
	/// </summary>
	/// <param name="fp"></param>
	/// <returns></returns>
	std::string LoadTextureFileName(FILE* fp);
	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�����[�h�B
	/// </summary>
	template<class T>
	void LoadIndexBuffer(std::vector<T>& indexBuffer, int numIndex, FILE* fp);
	/// <summary>
	/// �}�e���A�����\�z�B
	/// </summary>
	/// <param name="tkmMat"></param>
	void BuildMaterial(SMaterial& tkmMat, FILE* fp, const char* filePath);
	/// <summary>
	/// �ڃx�N�g���Ə]�x�N�g�����v�Z����B
	/// </summary>
	/// <remarks>
	/// 3dsMaxScript�ł��ׂ��Ȃ񂾂낤���ǁA�f�o�b�O�������̂ō��͂�����ł��B
	/// </remarks>
	void BuildTangentAndBiNormal();
private:
	std::vector< SMesh>	m_meshParts;		//���b�V���p�[�c�B
};