#pragma once
#include <DirectXMath.h>
#include <vector>
#include "NY_Object3DMgr.h"
#include <d3d12.h>

enum BlockType
{
	NONE = -1,
	BLOCK,
	DONT_MOVE_BLOCK,
	GOAL,
	START
};

class LoadStage
{
private: // �G�C���A�X
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	template<class T> using vector = std::vector<T>;

public: // �����o�֐�
	// �R���X�g���N�^
	LoadStage();
	// �f�X�g���N�^
	~LoadStage() {}

	// �ǂݍ���
	int Load(const char* filePath);
	// �`��
	void Draw();

public: // �����o�ϐ�
	vector<XMFLOAT3> blockPos;    //�u���b�N�̏ꏊ
	vector<int> blockType;        //�u���b�N�̎��
	vector<XMFLOAT4> blockColors; //�u���b�N�̐F
private:
	NY_Model3D debugBox;
	UINT graph;
	vector<Object3d*> debugBoxObj;

};
