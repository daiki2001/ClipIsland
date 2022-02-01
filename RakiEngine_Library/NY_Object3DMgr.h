#pragma once
#include <iostream>
#include <vector>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXTex.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <vector>
#include <memory>

using namespace Microsoft::WRL;
using namespace std;
using namespace DirectX;

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")


#include "NY_Camera.h"
#include "NY_Object3D.h"
#include "NY_Model.h"
#include "RVector.h"


typedef struct Pipeline3D
{
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	ComPtr<ID3D12PipelineState> pipelinestate;
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootsignature;

}PipelineSet;

/// 12/14
///�f�[�^�̊Ǘ����}�l�[�W���[���ꊇ�ōs���悤�ɂ��邽�߂�
///NY_Object3d���\���̉����A�}�l�[�W���[���R���e�i�ŊǗ��ł���悤�ɂ���
/// 
/// �e�N�X�`���f�[�^��texManager�ŊǗ�����悤�ɕύX


extern class Object3d;
extern class NY_Camera;

/// <summary>
/// ������g����΂����̂ŃV���O���g���p�^�[��
/// 
/// </summary>
class NY_Object3DManager
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	//----------�p�C�v���C���X�e�[�g�I�u�W�F�N�g----------//

	//�W���p�C�v���C���X�e�[�g
	Pipeline3D object3dPipelineSet;
	//�����`��p�p�C�v���C��
	Pipeline3D quickDrawPipelineSet;


	//----------�p�C�v���C���X�e�[�g�\����--------------//

	//�p�C�v���C���X�e�[�g�\���̂𕡐�����āA�K�X���蓖�Ă�`�ł������̂��H

	//�O���t�B�b�N�X�p�C�v���C���X�e�[�g�\����
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipelineStateDesc;


	//----------�V�F�[�_�[�I�u�W�F�N�g----------//

	//�V�F�[�_�[�I�u�W�F�N�g�i�W���j
	ComPtr<ID3DBlob> obj_VS;
	ComPtr<ID3DBlob> obj_GS;
	ComPtr<ID3DBlob> obj_PS;

	//�V�F�[�_�[�I�u�W�F�N�g�i�����`��p�j
	ComPtr<ID3DBlob> qd_BoxGS;


	//----------object3d�R���e�i----------//
	vector<Object3d*> objects;


	//----------NY_Camera�N���X�̃|�C���^----------//


	//----------���̑�----------//

	//�f�t�H���g�e�N�X�`���̐F
	XMFLOAT4 defColor = { 1.0,0.0,0.0,1.0 };

	//�R���X�g���N�^�A�f�X�g���N�^
	NY_Object3DManager() {};
	~NY_Object3DManager() {};

public:
	//�R�s�[�R���X�g���N�^������
	NY_Object3DManager(const NY_Object3DManager &obj) = delete;
	//������Z�q������
	NY_Object3DManager &operator=(const NY_Object3DManager &obj) = delete;

	//�C���X�^���X�擾
	static NY_Object3DManager *Get() {
		static NY_Object3DManager obj;
		return &obj;
	}

	//���ʎˉe�s��
	XMMATRIX matProjection{};
	//���ʃr���[�|�[�g�s��
	XMMATRIX matViewport{};
	//�e�N�X�`���p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descheap;
	//�e�N�X�`�����\�[�X�z��
	ComPtr<ID3D12Resource> texbuff[256];
	//�E�B���h�E�T�C�Y
	int window_width, window_height;

	/// <summary>
	/// <para>NY_Object3DManager�̐���</para>
	/// </summary>
	/// <param name="dev">ID3D12Device�̃|�C���^</param>
	/// <param name="window_width">�E�B���h�E����</param>
	/// <param name="window_height">�E�B���h�E�c��</param>
	/// <returns>����</returns>
	bool CreateObject3DManager(ID3D12Device *dev, int window_w, int window_h);

	/// <summary>
	/// �\��t����e�N�X�`���̓ǂݍ���
	/// </summary>
	/// <param name="texNumber">�e�N�X�`���ԍ����擾����ϐ�</param>
	/// <param name="filename">���[�h����e�N�X�`���̃p�X�i�j</param>
	/// <param name="dev"></param>
	void LoadObject3DTexture(UINT &texNumber, string filename, ID3D12Device *dev);

	/// <summary>
	/// �J�����̃Z�b�g
	/// </summary>
	/// <param name="cam">NY_Camera�ϐ�</param>

	/// <summary>
	/// object3d�̍쐬�A�R���e�i�Ɋi�[
	/// </summary>
	Object3d *CreateObject3d(NY_Model3D *modelData);

	/// <summary>
	/// �I�u�W�F�N�g�̍폜
	/// </summary>
	/// <param name="obj">����Object3D�̃|�C���^</param>
	void DeleteObject3d(Object3d *obj);

	/// <summary>
	/// �����σI�u�W�F�N�g�̈ꊇ�X�V
	/// </summary>
	void UpdateAllObjects();

	/// <summary>
	/// 3D�I�u�W�F�N�g�̕`������s���邽�߂̑O�������s��
	/// </summary>
	/// <param name="cmd">ID3D12GraphicsCommandList�̃|�C���^</param>
	void SetCommonBeginDrawObject3D();

	void SetCommonBeginDrawObject3D2MultiPassRenderResource();

	static ID3D12Device *GetDev() {
		return dev;
	}

private:

	//���̃t���[���ōX�V���s�������H
	bool isUpdated = false;

	//�f�o�C�X�̃|�C���^
	static ID3D12Device *dev;


	/// <summary>
	/// �����`��p�W�I���g���V�F�[�_�[�̃R���p�C���i�����֐��Ŏ��s�j
	/// </summary>
	/// <returns>�S�V�F�[�_�[�R���p�C���̐���</returns>
	HRESULT QuickDrawShadersCompile();

	/// <summary>
	/// �N���X���Ŏg�p����O���t�B�b�N�X�p�C�v���C���̐���
	/// </summary>
	/// <param name="dev">ID3D12Device�̃|�C���^</param>
	/// <returns>�쐬����Pipeline3D�\����</returns>
	Pipeline3D Create3DPipelineState(ID3D12Device *dev);


};

/// ----------- �L�q�ʂ̍팸�̂��߂̃��b�v�֐��Q ---------- ///

/// <summary>
/// NY_Object3DManager�̏�����
/// </summary>
/// <param name="dev">ID3D12Device�̃|�C���^</param>
/// <param name="window_w">�E�B���h�E����</param>
/// <param name="window_h">�E�B���h�E�c��</param>
inline void InitializeObject3DManager(ID3D12Device *dev, int window_w, int window_h) {

	//NY_Object3DManager�𐶐��A������
	NY_Object3DManager::Get()->CreateObject3DManager(dev, window_w, window_h);
}

/// <summary>
/// Object3d�̐����A�Ǘ��N���X�̃R���e�i�Ɋi�[
/// </summary>
/// <param name="modelData">NY_Model3D�̃��f���f�[�^</param>
/// <param name="pos">�����ʒu��ݒ肷��</param>
/// <returns>�������ꂽObject3d�������|�C���^</returns>
Object3d *CreateObject3d(NY_Model3D *modelData, RVector3 pos = { 0,0,0 });

/// <summary>
/// Object3d�f�[�^�̍폜
/// </summary>
/// <param name="obj"></param>
inline void DeleteObject3d(Object3d *obj) {

	NY_Object3DManager::Get()->DeleteObject3d(obj);
}

inline void UpdateObject() {
	//�S�I�u�W�F�N�g�̍X�V
	NY_Object3DManager::Get()->UpdateAllObjects();
}

//�`��֐�
void DrawObject3d(Object3d *obj);