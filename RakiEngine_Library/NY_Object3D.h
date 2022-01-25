#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "NY_Object3DMgr.h"
#include "RVector.h"

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

//�萔�o�b�t�@�f�[�^�\����B0
struct ConstBufferDataB0
{
	XMMATRIX mat;
	XMFLOAT4 color;
};

//�萔�o�b�t�@�f�[�^�\����B1
struct ConstBufferDataB1
{
	XMFLOAT3 amdient;  //�A���r�G���g
	float pad1;//�p�f�B���O
	XMFLOAT3 diffuse;  //�f�B�t���[�Y
	float pad2;//�p�f�B���O
	XMFLOAT3 specular; //�X�y�L�����[
	float alpha;       //�A���t�@
};

//�r���{�[�h�p�[�e�B�N�����_�f�[�^
struct ParticleVertex
{
	XMFLOAT3 pos;
};

extern class NY_Model3D;
extern class NY_Camera;
extern class NY_Object3DManager;


class Object3d
{
public:
	//�I�u�W�F�N�g�̖��O
	string tagName;

	//�K�p���郊�\�[�X�̔ԍ�
	UINT resourceNumber;

	//���f���f�[�^
	NY_Model3D *model;

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB0;
	ComPtr<ID3D12Resource> constBuffB1;

	//�萔�o�b�t�@�r���[�n���h��(CPU)
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleCBV;

	//�萔�o�b�t�@�r���[�n���h��(GPU)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleCBV;

	//�A�t�B���ϊ����
	RVector3 scale    = { 1,1,1 };
	RVector3 rotation = { 0,0,0 };
	RVector3 position = { 0,0,0 };

	//���[���h�ϊ��s��
	XMMATRIX matWorld;

	//�e�q(�e�v�f�ւ̃|�C���^)
	Object3d *parent = nullptr;

	//�F���
	XMFLOAT4 color = { 1,1,1,1 };

	//�r���{�[�h�t���O
	bool isBillBoard;

	//�_�[�e�B�t���O
	bool isDirty;

public:
	//�R���X�g���N�^
	Object3d() {
		isBillBoard = false;
		isDirty = true;
	};

	//�I�u�W�F�N�g�̏�����
	//static void InitObject3D(Object3d *obj, ID3D12Device *dev);
	void InitObject3D(ID3D12Device *dev);

	//���[�h�σ��f���f�[�^�̐ݒ�
	//static void SetLoadedModelData(Object3d *obj, NY_Model3D *loadedModel);
	void SetLoadedModelData(NY_Model3D *loadedModel);

	//�I�u�W�F�N�g�X�V
	//static void UpdateObject3D(Object3d *obj, XMMATRIX &matview);
	void UpdateObject3D();

	//�r���{�[�h�X�V�i�J�����I�u�W�F�N�g�����̂܂܎�荞��ŁA�r���{�[�h�p�̍X�V�������s���j
	//static void UpdateBillBoard3D(Object3d *obj, NY_Camera cam);
	void UpdateBillBoard3D();

	//���f���f�[�^���g�p�����I�u�W�F�N�g�`��
	static void DrawModel3D(Object3d *obj, ID3D12GraphicsCommandList *cmd,ID3D12Device *dev);
	void DrawModel3D(ID3D12GraphicsCommandList *cmd, ID3D12Device *dev);


private:
	/// �v���C�x�[�g�����o�ϐ�

};





