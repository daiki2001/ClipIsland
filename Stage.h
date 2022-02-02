#pragma once
#include "LoadStage.h"
#include "Player.h"
#include "RVector.h"
#include <vector>
#include <stack>
#include "Audio.h"
#include "Player.h"
#include "Raki_Input.h"

class Stage
{
public: // �T�u�N���X
	struct ClipBlock
	{
		/*1��*/
		int ReferencePoint1 = -1;
		int blockNumber1 = -1;
		vector<RVector3> vec1 = {};
		vector<int> gateNumber1 = {};
		/*2��*/
		int ReferencePoint2 = -1;
		int blockNumber2 = -1;
		vector<RVector3> vec2 = {};
		vector<int> gateNumber2 = {};
		/*�v���C���[�̍��W*/
		RVector3 playerPos = {};
		RVector3 playerStartPos = {};
		RVector3 playerEndPos = {};
		RVector3 vaniPos = {};
		/*���߂�u���b�N�����邩�ǂ����̃t���O*/
		bool isClip = false;
		//
		bool isVani = false;
		float backPosZ = 0;
	};

private: // �G�C���A�X
	template<class T> using stack = std::stack<T>;

public: // �萔
	static const size_t maxFlame;

public: // �����o�֐�
	Stage() = delete;
	Stage(Player* player);
	~Stage();

	// �`��
	void Update();

	// �`��
	void Draw();

	// �X�e�[�W�I��
	int Select(const char* filePath, const bool& flag2d);
	// �u���b�N������
	int Clip(bool flag);
	// ���߂�
	int StepBack();
	// ���Z�b�g
	void Reset();
	// �`�F���W
	void Change();

	// ���߂�u���b�N�B�̊�̃u���b�N�̍��W���擾
	void GetClipBlocksReferencePoint(RVector3* pos1, RVector3* pos2);
	// ���߂�u���b�N�B�̍��W��S�Ď擾
	bool GetClipBlocksALL(int blocksArray[], const size_t& sizeArray);
private:
	// �u���b�N������(2d��)
	int Clip2d(ClipBlock* clip);
	// �u���b�N������(3d��)
	int Clip3d(ClipBlock* clip);

public: // �����o�ϐ�
	LoadStage stage;
	SoundData clipSE;
	SoundData bottunSE;
	SoundData blockSE;
	SoundData stepBackSE;
	SoundData resetSE;

	bool backFlag;
	bool clipFlag;
private:
	Player* player;
	stack<ClipBlock> clipBlock;
	bool flag2d;

	vector<int> easeNumber;
	vector<RVector3> easeStartPos;
	size_t nowFlame;
	bool isEasing;
};
