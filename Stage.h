#pragma once
#include "LoadStage.h"
#include "RVector.h"
#include <stack>

class Stage
{
public: // �T�u�N���X
	struct ClipBlock
	{
		/*1��*/
		int blockNumber1;
		RVector3 pos1;
		/*2��*/
		int blockNumber2;
		RVector3 pos2;
	};

private: // �G�C���A�X
	template<class T> using stack = std::stack<T>;

public: // �����o�֐�
	Stage();
	~Stage();

	// �X�V
	void Update();
	// �`��
	void Draw();

	// ���߂�
	void StepBack();
	// ���Z�b�g
	void Reset();

private: // �����o�ϐ�
	LoadStage* stage;
	stack<ClipBlock> clipBlock;

};
