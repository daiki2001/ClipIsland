#pragma once

#include <d3dx12.h>
#include <DirectXTex.h>
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")

//uv�I�t�Z�b�g�i�[�R���e�i
struct UVOFFSETS {
	DirectX::XMFLOAT2 offsetLT;	//����
	DirectX::XMFLOAT2 offsetRT;	//�E��
	DirectX::XMFLOAT2 offsetLB;	//����
	DirectX::XMFLOAT2 offsetRB;	//�E��
};

//uv�A�j���[�V�����p�f�[�^
class uvAnimData {
private:
	//uv�I�t�Z�b�g�i�[�R���e�i
	std::vector<UVOFFSETS> uvOffsets;

public:
	//�R���X�^���N�^�A�f�X�g���N�^
	uvAnimData() {
		usingNo = 0;
	};
	~uvAnimData() {};

	//�g�p���̃A�j���[�V�����ԍ�
	int usingNo;

	//�I�t�Z�b�g�쐬
	void AddOffsets(UVOFFSETS offset);
	//�I�t�Z�b�g�擾
	UVOFFSETS GetOffset();

	int Getsize();

};

//�}���`�p�X�����_�����O������Ƃ��ɂ���ƍ��オ�y���Ǝv���č����
//����
class TexManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	TexManager() {};
	~TexManager() {};

	static ID3D12Device *dev;

public:
	//�e�N�X�`���ꖇ�̃f�[�^
	struct texture {
		UINT									texNumber;
		Microsoft::WRL::ComPtr<ID3D12Resource>	texBuff;	//�e�N�X�`���o�b�t�@
		const DirectX::Image					*img;		
		DirectX::TexMetadata					metaData;
		DirectX::ScratchImage					scratchImg;
		//�A�j���[�V�����puv�Ǘ�
		std::vector<DirectX::XMFLOAT2>			uv_offsets;	//uv�l�I�t�Z�b�g�z��
	};
	//�e�N�X�`���f�[�^�z��
	static texture textureData[1024];
	//�f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> texDsvHeap;
	//�e�N�X�`���ő吔
	static const int MAX_TEXNUM;


public:
	/// <summary>
	/// TexManager�̏������A�f�B�X�N���v�^�q�[�v����
	/// </summary>
	static void InitTexManager();

	/// <summary>
	/// �e�N�X�`���̃��[�h
	/// </summary>
	/// <param name="filename">const char�^�ŉ摜�t�@�C���̖��O</param>
	/// <returns>�i�[�����o�b�t�@�̔ԍ�</returns>
	static UINT LoadTexture(const char *filename);

	static UINT LoadTexture(std::string filename);

	/// <summary>
	/// �e�N�X�`���̕����ǂݍ���
	/// </summary>
	/// <param name="filename">�摜�t�@�C���̖��O</param>
	/// <param name="numDivTex">�������i�E�����j</param>
	/// <param name="sizeX">1���̃T�C�Y</param>
	/// <returns>�i�[�����e�N�X�`���̏ꏊ��z���</returns>
	static UINT LoadDivTextureTest(uvAnimData *data, const char *filename, const int numDivTex, int sizeX);


};

