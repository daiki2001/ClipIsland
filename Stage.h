#pragma once
#include "LoadStage.h"
#include "Player.h"
#include "RVector.h"
#include <stack>

class Stage
{
public: // �T�u�N���X
	struct ClipBlock
	{
		/*1��*/
		int ReferencePoint1 = -1;
		int blockNumber1 = -1;
		RVector3 vec1 = {};
		/*2��*/
		int ReferencePoint2 = -1;
		int blockNumber2 = -1;
		RVector3 vec2 = {};
		/*�v���C���[�̍��W*/
		XMFLOAT3 playerPos = {};
	};

private: // �G�C���A�X
	template<class T> using stack = std::stack<T>;

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
private:
	// �u���b�N������(2d��)
	int Clip2d(ClipBlock* clip);
	// �u���b�N������(3d��)
	int Clip3d(ClipBlock* clip);

public: // �����o�ϐ�
	LoadStage stage; //�ǂݍ��񂾃X�e�[�W�̃f�[�^
private:
	Player* player;           //�v���C���[�̃f�[�^

	stack<ClipBlock> clipBlock; //���񂾎��̏��
	bool flag2d; //�X�e�[�W���񎟌����ǂ����̃t���O

};
