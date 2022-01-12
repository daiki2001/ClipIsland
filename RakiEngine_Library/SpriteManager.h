#pragma once
#include <iostream>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <d3dcompiler.h>
#include <wrl.h>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")

//�X�v���C�g�p���_�\����
typedef struct SpriteVertex
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
}SPVertex;

//�萔�o�b�t�@�f�[�^�\����
typedef struct SpConstBufferData
{
	XMFLOAT4 color;//�F(rgba)
	XMMATRIX mat;
}SPConstBuffer;

//�X�v���C�g�ꖇ���̃f�[�^
typedef struct SpriteData
{
	ComPtr<ID3D12Resource> vertBuff;//���_�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW vbView;//���_�o�b�t�@�r���[
	ComPtr<ID3D12Resource> constBuff;//�萔�o�b�t�@
	UINT texNumber;//�}�l�[�W���[�ɕۑ����ꂽ���\�[�X�̔ԍ�

	XMFLOAT2 size;//�X�v���C�g�T�C�Y

	float rotation = 0.0f;//z����]�p
	XMFLOAT3 position = { 0.0f,0.0f,0.0f };//���W
	XMMATRIX matWorld;//���[���h�ϊ��s��

	XMFLOAT2 anchorPoint = { 0.0f,0.0f };//�A���J�[�|�C���g

	XMFLOAT2 screenPos = { 0.0f,0.0f };//�X�N���[�����W

}SPData;

//�X�v���C�g���ʊǗ��N���X
//Winmain�ł�spritemanager�̎��̂�1�̂ݍ�邱�ƁI

//�d�l
/*
	DrawSprite()�̈�����Sprite�N���X�̃C���X�^���X����荞�ނ��Ƃŕ`��
*/

//�X�v���C�g�p�e�N�X�`���̍ő�ǂݍ��ݐ�
const int MAX_TEX_NUM = 512;

class SpriteManager
{
	//template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	//�X�v���C�g�p�O���t�B�b�N�X�p�C�v���C���Z�b�g
	ComPtr<ID3D12PipelineState> pipelinestate;
	ComPtr<ID3D12RootSignature> rootsignature;

	SpriteManager(ID3D12Device *dev, int window_width, int window_height) {
		//�E�B���h�E�T�C�Y�ݒ�
		this->window_width = window_width;
		this->window_height = window_height;
		//�r���[�|�[�g�s�񏉊���
		matViewport.r[0].m128_f32[0] = window_width / 2;
		matViewport.r[1].m128_f32[1] = -window_height / 2;
		matViewport.r[3].m128_f32[0] = window_width / 2;
		matViewport.r[3].m128_f32[1] = window_height / 2;
		//�p�C�v���C������
		CreateSpritePipeline(dev);
		matProjection = XMMatrixOrthographicOffCenterLH(0.0f, (float)this->window_width, (float)this->window_height, 0.0f, 0.0f, 1.0f);
	};
	SpriteManager() {};
	~SpriteManager() {};

public: 
	//���ʎˉe�s��
	XMMATRIX matProjection{};
	//���ʃr���[�|�[�g�s��
	XMMATRIX matViewport{};
	//�e�N�X�`���p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descheap;
	//�e�N�X�`�����\�[�X�z��
	ComPtr<ID3D12Resource> texbuff[MAX_TEX_NUM];
	//�E�B���h�E�T�C�Y
	int window_width = 1280, window_height = 720;


	void CreateSpriteManager(ID3D12Device *dev, int window_w, int window_h);
	//�X�v���C�g�̃O���t�B�b�N�X�p�C�v���C���𐶐�
	void CreateSpritePipeline(ID3D12Device *dev);
	//�X�v���C�g�̃e�N�X�`���ǂݍ���
	void LoadSpriteTexture(UINT texNumber, const wchar_t *fliename, ID3D12Device *dev);
	//�X�v���C�g���ʂ̃O���t�B�b�N�X�R�}���h���Z�b�g
	void SetCommonBeginDraw(ID3D12GraphicsCommandList *cmd);

	//�C���X�^���X�擾
	static SpriteManager *Get() {
		static SpriteManager mgr;
		return &mgr;
	}


	//�R�s�[�R���X�g���N�^�A������Z�q������
	SpriteManager &operator=(const SpriteManager &obj) = delete;
	SpriteManager(const SpriteManager &obj) = delete;

};

