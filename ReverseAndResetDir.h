#pragma once

#include <RVector.h>

#include <TexManager.h>
#include <Sprite.h>

class ReverseAndResetDir final
{
private:
	//�e�N�X�`�����\�[�X
	UINT backTex;
	UINT resetTex;
	//�X�v���C�g
	Sprite backSp;
	Sprite resetSp;
	//�C�[�W���O�ڕW���W
	RVector3 in_start;
	RVector3 in_end;
	RVector3 out_start;
	RVector3 out_end;
	//���
	enum state {
		standby,
		in,
		wait,
		out,
	};
	//�C�[�W���O�ړ��t���[��
	const int MOVE_FRAME = 30;
	//��ʒ����ҋ@�t���[��
	const int WAIT_FRAME = 15;
	//�t���[���J�E���g
	int back_frameCount;
	int reset_frameCount;

	//�R���X�g���N�^
	ReverseAndResetDir();
	~ReverseAndResetDir();

public:
	//���݂̏��
	state now_backSp;
	state now_resetSp;

	//�Q�b�^
	static ReverseAndResetDir *Get() {
		static ReverseAndResetDir ins;
		return &ins;
	}

	//������
	void Init();

	//�߂鉉�o
	void Start_BackDir();
	//���Z�b�g���o
	void Start_ResetDir();

	//�X�V
	void Update();

	//�`��
	void Draw();


};

