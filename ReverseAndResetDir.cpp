#include "ReverseAndResetDir.h"

ReverseAndResetDir::ReverseAndResetDir()
{
	//リソース
	backTex = TexManager::LoadTexture("Resources/back.jpg");
	resetTex = TexManager::LoadTexture("Resources/reset.jpg");
	backSp.CreateSprite({ 0,0 }, { 0.5,0.5 }, backTex, true, nullptr);
	resetSp.CreateSprite({ 0,0 }, { 0.5f,0.5f }, resetTex, true, nullptr);
	//イージング座標設定
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
	//座標初期化
	resetSp.spdata.position = in_start;
	backSp.spdata.position = in_start;

	//モード初期化
	now_backSp = standby;
	now_resetSp = standby;

	//経過フレーム初期化
	back_frameCount = 0;
	reset_frameCount = 0;
}

void ReverseAndResetDir::Start_BackDir()
{
	if (now_backSp != standby) {
		return;
	}
	//モード設定
	now_backSp = state::in;
}

void ReverseAndResetDir::Start_ResetDir()
{
	if (now_resetSp != standby) {
		return;
	}
	//モード設定
	now_resetSp = state::in;
}

void ReverseAndResetDir::Update()
{
	float back_t = (float)back_frameCount / (float)MOVE_FRAME;
	float reset_t = (float)reset_frameCount / (float)MOVE_FRAME;

	switch (now_backSp)
	{
	case ReverseAndResetDir::standby:
		//座標は橋
		backSp.spdata.position = in_start;
		break;
	case ReverseAndResetDir::in:
		back_frameCount++;
		//補間
		backSp.spdata.position = Rv3Ease::InQuad(in_start, in_end, back_t);
		//規定フレーム経過で待機
		if (back_frameCount > MOVE_FRAME) { now_backSp = state::wait; }
		break;
	case ReverseAndResetDir::wait:
		//座標は中央
		backSp.spdata.position = in_end;
		//フレームカウント
		back_frameCount++;
		//規定フレーム経過で退場
		if (back_frameCount > MOVE_FRAME + WAIT_FRAME) {
			back_frameCount = 0;
			now_backSp = state::out;
		}
		break;
	case ReverseAndResetDir::out:
		back_frameCount++;
		//イージング
		backSp.spdata.position = Rv3Ease::InQuad(out_start, out_end, back_t);
		//規定時間経過
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
		//座標は橋
		resetSp.spdata.position = in_start;
		break;
	case ReverseAndResetDir::in:
		reset_frameCount++;
		//補間
		resetSp.spdata.position = Rv3Ease::InQuad(in_start, in_end, reset_t);
		//規定フレーム経過で待機
		if (reset_frameCount > MOVE_FRAME) { now_resetSp = state::wait; }
		break;
	case ReverseAndResetDir::wait:
		//座標は中央
		resetSp.spdata.position = in_end;
		//フレームカウント
		reset_frameCount++;
		//規定フレーム経過で退場
		if (reset_frameCount > MOVE_FRAME + WAIT_FRAME) {
			reset_frameCount = 0;
			now_resetSp = state::out;
		}
		break;
	case ReverseAndResetDir::out:
		reset_frameCount++;
		//イージング
		resetSp.spdata.position = Rv3Ease::InQuad(out_start, out_end, reset_t);
		//規定時間経過
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
