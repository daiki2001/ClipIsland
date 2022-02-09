#include "ReverseAndResetDir.h"

ReverseAndResetDir::ReverseAndResetDir()
{
	//���\�[�X
	backTex = TexManager::LoadTexture("Resources/back.jpg");
	resetTex = TexManager::LoadTexture("Resources/reset.jpg");
	backSp.CreateSprite({ 0,0 }, { 0.5,0.5 }, backTex, true, nullptr);
	resetSp.CreateSprite({ 0,0 }, { 0.5f,0.5f }, resetTex, true, nullptr);
	//�C�[�W���O���W�ݒ�
	in_start = RVector3(1400, 360, 0);
	in_end = RVector3(640, 360, 0);
	out_start = in_end;
	out_end = RVector3(-60, 360, 0);
}

ReverseAndResetDir::~ReverseAndResetDir()
{
}

void ReverseAndResetDir::Init()
{
	//���W������
	resetSp.spdata.position = in_start;
	backSp.spdata.position = in_start;

	//���[�h������
	now_backSp = standby;
	now_resetSp = standby;

	//�o�߃t���[��������
	back_frameCount = 0;
	reset_frameCount = 0;
}

void ReverseAndResetDir::Start_BackDir()
{
	if (now_backSp != standby) {
		return;
	}
	//���[�h�ݒ�
	now_backSp = state::in;
}

void ReverseAndResetDir::Start_ResetDir()
{
	if (now_resetSp != standby) {
		return;
	}
	//���[�h�ݒ�
	now_resetSp = state::in;
}

void ReverseAndResetDir::Update()
{
	float back_t = (float)back_frameCount / (float)MOVE_FRAME;
	float reset_t = (float)reset_frameCount / (float)MOVE_FRAME;

	switch (now_backSp)
	{
	case ReverseAndResetDir::standby:
		//���W�͋�
		backSp.spdata.position = in_start;
		break;
	case ReverseAndResetDir::in:
		back_frameCount++;
		//���
		backSp.spdata.position = Rv3Ease::InQuad(in_start, in_end, back_t);
		//�K��t���[���o�߂őҋ@
		if (back_frameCount > MOVE_FRAME) { now_backSp = state::wait; }
		break;
	case ReverseAndResetDir::wait:
		//���W�͒���
		backSp.spdata.position = in_end;
		//�t���[���J�E���g
		back_frameCount++;
		//�K��t���[���o�߂őޏ�
		if (back_frameCount > MOVE_FRAME + WAIT_FRAME) {
			back_frameCount = 0;
			now_backSp = state::out;
		}
		break;
	case ReverseAndResetDir::out:
		back_frameCount++;
		//�C�[�W���O
		backSp.spdata.position = Rv3Ease::InQuad(out_start, out_end, back_t);
		//�K�莞�Ԍo��
		if (back_frameCount > MOVE_FRAME) {
			back_frameCount = 0;
			now_backSp = state::standby;
		}
		break;
	default:
		break;
	}


	switch (now_resetSp)
	{
	case ReverseAndResetDir::standby:
		//���W�͋�
		resetSp.spdata.position = in_start;
		break;
	case ReverseAndResetDir::in:
		reset_frameCount++;
		//���
		resetSp.spdata.position = Rv3Ease::InQuad(in_start, in_end, reset_t);
		//�K��t���[���o�߂őҋ@
		if (reset_frameCount > MOVE_FRAME) { now_resetSp = state::wait; }
		break;
	case ReverseAndResetDir::wait:
		//���W�͒���
		resetSp.spdata.position = in_end;
		//�t���[���J�E���g
		reset_frameCount++;
		//�K��t���[���o�߂őޏ�
		if (reset_frameCount > MOVE_FRAME + WAIT_FRAME) {
			reset_frameCount = 0;
			now_resetSp = state::out;
		}
		break;
	case ReverseAndResetDir::out:
		reset_frameCount++;
		//�C�[�W���O
		resetSp.spdata.position = Rv3Ease::InQuad(out_start, out_end, reset_t);
		//�K�莞�Ԍo��
		if (reset_frameCount > MOVE_FRAME) {
			reset_frameCount = 0;
			now_resetSp = state::standby;
		}
		break;
	default:
		break;
	}

	backSp.UpdateSprite();
	resetSp.UpdateSprite();
}

void ReverseAndResetDir::Draw()
{
	backSp.Draw();
	resetSp.Draw();
}
