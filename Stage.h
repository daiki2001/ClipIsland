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
		RVector3 pos1;
		int type1;
		/*2��*/
		RVector3 pos2;
		int type2;
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

	// ���Z�b�g
	void Reset() { stage->Reset(); }

private: // �����o�ϐ�
	LoadStage* stage;
	stack<ClipBlock> clipBlock;

};
