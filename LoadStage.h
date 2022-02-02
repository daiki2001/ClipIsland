#pragma once
#include <DirectXMath.h>
#include "RVector.h"
#include <vector>
#include "NY_Object3DMgr.h"
#include "Warp.h"
#include <d3d12.h>
#include "Collision.h"
#include "GameCommonData.h"

class LoadStage
{
private: // �G�C���A�X
	using XMFLOAT4 = DirectX::XMFLOAT4;
	template<class T> using vector = std::vector<T>;
	using BlockType = GameCommonData::BlockData::BlockType;
	using Data = GameCommonData::BlockData::Data;
	using StageBlockModels = GameCommonData::StageBlockModels;

public: // �����o�֐�
	// �R���X�g���N�^
	LoadStage();
	// �f�X�g���N�^
	~LoadStage();

	// �ǂݍ���
	int Load(const char* filePath);
	// �X�V
	void Update();
	// �`��
	void Draw();

	// �u���b�N�̏ꏊ�̃��Z�b�g
	void Reset();
	// �X�e�[�W�f�[�^�̍폜
	void StageClear();

	// �X�C�b�`�̃��f���f�[�^�̐؂�ւ�
	void ChangeSwitchModel(NY_Model3D* modelData);

	// �v���C���[�̃X�^�[�g�ʒu�̎擾
	RVector3 GetStartPlayerPos();
	// �u���b�N�̏�����ނ��w�肵�đS�Ď擾
	void GetBlocksTypeAll(BlockType blockType, int blocksArray[], size_t arraySize);

	// �u���b�N���򂲂ƂŃ\�[�g����֐�
	void blockSortIsNumber(int start, int end);
private:
	// �p�[�e�[�V�������s���֐�
	int PartitionIsBlockNumber(int p, int r);

public: // �����o�ϐ�
	vector<Data> blocks;             //�u���b�N�̏��
	vector<XMFLOAT4> blockColors;    //�u���b�N�̐F
	vector<int> multipleBlockNumber; //�u���b�N�������L���̔ԍ�

	vector<Warp*> warpBlock; //���[�v�u���b�N�̏��
	size_t openGateCount;
private:
	int startPosNumber; //�J�n���̃v���C���[�̍��W(�z��̗v�f�ԍ�)

	vector<Object3d*> moveBlockObj;   //��������u���b�N�̃I�u�W�F�N�g
	vector<int> moveBlockNumber;      //��������u���b�N�̔ԍ�
	vector<Object3d*> stayBlockObj;   //�����Ȃ��u���b�N�̃I�u�W�F�N�g
	vector<int> stayBlockNumber;      //�����Ȃ��u���b�N�̔ԍ�
	vector<Object3d*> goalBlockObj;   //�S�[���u���b�N�̃I�u�W�F�N�g
	vector<int> goalBlockNumber;      //�S�[���u���b�N�̔ԍ�
	vector<Object3d*> switchBlockObj; //�X�C�b�`�̃I�u�W�F�N�g
	vector<int> switchBlockNumber;    //�X�C�b�`�̔ԍ�
	vector<Object3d*> doorBlockObj; //�X�C�b�`�ŏ�����u���b�N�̃I�u�W�F�N�g
	vector<int> doorBlockNumber;    //�X�C�b�`�ŏ�����u���b�N�̔ԍ�

	vector<Object3d*> debugBoxObj; //�f�o�b�O�p�̃u���b�N�̃I�u�W�F�N�g
	vector<int> debugBoxNumber;    //�f�o�b�O�p�̃u���b�N�̔ԍ�

};
