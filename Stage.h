#pragma once
#include "LoadStage.h"
#include "Player.h"
#include "RVector.h"
#include <stack>

class Stage
{
public: // サブクラス
	struct ClipBlock
	{
		/*1つ目*/
		int blockNumber1 = -1;
		RVector3 vec1 = {};
		/*2つ目*/
		int blockNumber2 = -1;
		RVector3 vec2 = {};
		/*プレイヤーの座標*/
		XMFLOAT3 playerPos = {};
	};

private: // エイリアス
	template<class T> using stack = std::stack<T>;

public: // メンバ関数
	Stage() = delete;
	Stage(Player* player);
	~Stage();

	// 描画
	void Update();

	// 描画
	void Draw();

	// ステージ選択
	int Select(const char* filePath, const bool& flag2d);
	// ブロックを挟む
	int Clip(bool flag);
	// 一手戻る
	int StepBack();
	// リセット
	void Reset();
private:
	// ブロックを挟む(2d版)
	int Clip2d(bool flag, ClipBlock* clip);
	// ブロックを挟む(3d版)
	int Clip3d(bool flag, ClipBlock* clip);

public: // メンバ変数
	LoadStage stage; //読み込んだステージのデータ
private:
	Player* player;           //プレイヤーのデータ

	stack<ClipBlock> clipBlock; //挟んだ時の情報
	bool flag2d; //ステージが二次元かどうかのフラグ

};
