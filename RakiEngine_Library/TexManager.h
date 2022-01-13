#pragma once

#include <d3dx12.h>
#include <DirectXTex.h>
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")

//�}���`�p�X�����_�����O������Ƃ��ɂ���ƍ��オ�y���Ǝv���č����
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

};
