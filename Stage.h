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
		int ReferencePoint1 = -1;
		int blockNumber1 = -1;
		RVector3 vec1 = {};
		/*2つ目*/
		int ReferencePoint2 = -1;
		int blockNumber2 = -1;
		RVector3 vec2 = {};
		/*プレイヤーの座標*/
		RVector3 playerPos = {};
		/*挟めるブロックがあるかどうかのフラグ*/
		bool isClip = false;
		//
		bool isVani = false;
		float backPosZ = 0;
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
	// チェンジ
	void Change();

	// 挟めるブロック達の基準のブロックの座標を取得
	void GetClipBlocksReferencePoint(RVector3* pos1, RVector3* pos2);
	// 挟めるブロック達の座標を全て取得
	void GetClipBlocksALL(int blocksArray[], const size_t& sizeArray);
private:
	// ブロックを挟む(2d版)
	int Clip2d(ClipBlock* clip);
	// ブロックを挟む(3d版)
	int Clip3d(ClipBlock* clip);

private: // メンバ変数
	Player* player;
	stack<ClipBlock> clipBlock;
	bool flag2d;
public:
	LoadStage stage;
};
