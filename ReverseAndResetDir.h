#pragma once

#include <RVector.h>

#include <TexManager.h>
#include <Sprite.h>

class ReverseAndResetDir final
{
private:
	//テクスチャリソース
	UINT backTex;
	UINT resetTex;
	//スプライト
	Sprite backSp;
	Sprite resetSp;
	//イージング目標座標
	RVector3 in_start;
	RVector3 in_end;
	RVector3 out_start;
	RVector3 out_end;
	//状態
	enum state {
		standby,
		in,
		wait,
		out,
	};
	//イージング移動フレーム
	const int MOVE_FRAME = 30;
	//画面中央待機フレーム
	const int WAIT_FRAME = 15;
	//フレームカウント
	int back_frameCount;
	int reset_frameCount;

	//コンストラクタ
	ReverseAndResetDir();
	~ReverseAndResetDir();

public:
	//現在の状態
	state now_backSp;
	state now_resetSp;

	//ゲッタ
	static ReverseAndResetDir *Get() {
		static ReverseAndResetDir ins;
		return &ins;
	}

	//初期化
	void Init();

	//戻る演出
	void Start_BackDir();
	//リセット演出
	void Start_ResetDir();

	//更新
	void Update();

	//描画
	void Draw();


};

