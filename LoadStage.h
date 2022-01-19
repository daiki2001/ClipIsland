#pragma once
#include <DirectXMath.h>
#include <vector>
#include "NY_Object3DMgr.h"
#include <d3d12.h>
#include "Collision.h"
#include "BlockData.h"

class LoadStage
{
private: // �G�C���A�X
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	template<class T> using vector = std::vector<T>;

public: // �萔

public: // �����o�֐�
	// �R���X�g���N�^
	LoadStage();
	// �f�X�g���N�^
	~LoadStage();

	// �ǂݍ���
	int Load(const char* filePath);
	// �`��
	void Draw();

	// �u���b�N�̏ꏊ�̃��Z�b�g
	void Reset();
	// �X�e�[�W�f�[�^�̍폜
	void StageClear();
	// �v���C���[�̏����ʒu�̎擾
	XMFLOAT3 GetStartPlayerPos();

public: // �����o�ϐ�
	vector<BlockData::Data> blocks; //�u���b�N�̃f�[�^
	vector<XMFLOAT4> blockColors;  //�u���b�N�̐F
	vector<Object3d*> debugBoxObj; //�u���b�N�̃I�u�W�F�N�g
private:
	int startPosNumber; //�J�n���̃v���C���[�̍��W(�z��̗v�f�ԍ�)

	NY_Model3D debugBox; //�u���b�N�̃��f��
	UINT graph;          //�u���b�N�̃e�N�X�`��

};
