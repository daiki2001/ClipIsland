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

	// �u���b�N�̏ꏊ�̃��Z�b�g
	void Reset();

public: // �����o�ϐ�
	vector<int> blockType;         //�u���b�N�̎��
	vector<XMFLOAT4> blockColors;  //�u���b�N�̐F
	vector<Object3d*> debugBoxObj; //�u���b�N�̃I�u�W�F�N�g
private:
	vector<XMFLOAT3> blockPos; //�u���b�N�̏ꏊ

	NY_Model3D debugBox; //�u���b�N�̃��f��
	UINT graph;          //�u���b�N�̃e�N�X�`��

};
