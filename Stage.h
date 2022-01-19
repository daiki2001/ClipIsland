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
		size_t blockNumber1 = (size_t)-1;
		RVector3 vec1 = {};
		/*2��*/
		size_t blockNumber2 = (size_t)-1;
		RVector3 vec2 = {};
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
	int Clip2d(bool flag, ClipBlock* clip);
	// �u���b�N������(3d��)
	int Clip3d(bool flag, ClipBlock* clip);

private: // �����o�ϐ�
	Player* player;
	stack<ClipBlock> clipBlock;
	bool flag2d;
public:
	LoadStage stage;
};
