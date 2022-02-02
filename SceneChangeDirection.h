#pragma once

#include <Sprite.h>
#include <RVector.h>

class SceneChangeDirection final
{
private:
	//挟むテクスチャ
	UINT tex1;
	UINT tex2;
	//挟むスプライト
	Sprite press1, press2;
	//挟み開始点、終了点
	RVector3 ps1s, ps1e;
	RVector3 ps2s, ps2e;
	//フレームカウント、演出最大フレーム数
	int frameCount;
	const int DIR_FRAME = 60;
	//どちらの演出か
	bool isStart = false;
	bool isEnd = false;
	//コンストラクタ、デストラクタ
	SceneChangeDirection();
	~SceneChangeDirection();

public:
	//ゲッタ
	static SceneChangeDirection *get() {
		static SceneChangeDirection ins;
		return &ins;
	}
	//演出終了フラグ
	bool isEndDir = false;

	//初期化
	void Init();
	//開始演出開始
	void Start_StartDir();
	//終了演出開始
	void Start_EndDir();
	//更新
	void Update();
	//描画
	void Draw();
};