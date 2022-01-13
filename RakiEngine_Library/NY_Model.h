#pragma once
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

#include "NY_Object3DMgr.h"

//���_�f�[�^
struct Vertex
{
	XMFLOAT3 pos;   //xyz
	XMFLOAT3 normal;//�@��
	XMFLOAT2 uv;    //uv
};

//�}�e���A���f�[�^�\����
struct Material
{
	string   mtlName;      //�}�e���A����(obj�t�@�C������ł͂Ȃ��ꍇ�A�g��Ȃ��Ă����H)
	XMFLOAT3 ambient;      //�A���r�G���g
	XMFLOAT3 diffuse;      //�f�B�t���[�Y
	XMFLOAT3 specurar;     //�X�y�L�����[
	float    alpha;        //�A���t�@
	string   texFileName;  //�e�N�X�`����
	UINT     texNumber;    //�e�N�X�`���ԍ�(�g�p����ꍇ�K���K�v)
	//�R���X�g���N�^
	Material() {
		ambient = { 0.3f,0.3f,0.3f };
		diffuse = { 0.0f,0.0f,0.0f };
		specurar = { 0.0f,0.0f,0.0f };
		alpha = 1.0f;
	}
};

class NY_Object3DManager;

class NY_Model3D
{
public:
	//���_�f�[�^
	vector<Vertex> vertices;
	//���_�C���f�b�N�X
	vector<unsigned short> indices;
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�C���f�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibview{};
	//�}�e���A��
	Material material;

public:

	NY_Model3D() {};

	//----------�t�@�C����ǂݍ���Ń��f���f�[�^�����֐��Q----------//

	//obj�t�@�C���ǂݍ��݁B�ǂݍ��񂾌�̃o�b�t�@�֘A�̏������s���B�e�N�X�`�������ނ��߁AObject3D�}�l�[�W���������Ŏ擾
	void LoadObjModel(const char *filename, ID3D12Device *dev, NY_Object3DManager *mgr);
	//�}�e���A���ǂݍ��݁BLoadObjModel���Ŏ��s����Amaterial�Ƀf�[�^���i�[
	void LoadMatarial(string fileName,string filepath, ID3D12Device *dev, NY_Object3DManager *mgr);

	//----------���f���f�[�^��1����쐬����֐��Q----------//

	//xy���ʂ��쐬�B�e�N�X�`���̃��[�h���K�{�Ȃ��ߎg���ɂ�������
	//void CreatePlaneModelXY(float x_size, float y_size, string useTexFileName, ID3D12Device *dev, NY_Object3DManager *mgr);

	//xy���ʂ��쐬�B���[�h�σe�N�X�`�����g�p�B�e�N�X�`���o�b�t�@�̔ԍ����w�肷�邱�Ɓi��̃o�b�t�@���w�肷��Ƃ��ƂŎ��ʁj
	void CreatePlaneModelXY(float x_size, float y_size, float uv_x, float uv_y, UINT useTexNum, ID3D12Device *dev);
	//xz����
	void CreatePlaneModelXZ(float x_size, float z_size, float uv_x, float uv_y, UINT useTexNum, ID3D12Device *dev);
	//yz����
	void CreatePlaneModelYZ(float y_size, float z_size, float uv_x, float uv_y, UINT useTexNum, ID3D12Device *dev);

	//�����̃��f���f�[�^�쐬
	void CreateBoxModel(float size, float uv_x, float uv_y, UINT useTexNum, ID3D12Device *dev);

	//object3D�Ŏg�p���钸�_�f�[�^���쐬(���쒆)
	//static SingleVertex GetSingleVertexData(ID3D12Device *dev);

private:

	//�W�I���g���p�O�p�`�����i���쒆�j
	//void CreateTriangleGeometoryTest(UINT usetexnum, ID3D12Device *dev);

};
