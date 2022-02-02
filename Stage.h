#pragma once
#include "LoadStage.h"
#include "Player.h"
#include "RVector.h"
#include <vector>
#include <stack>
#include "Audio.h"
#include "Player.h"
#include "Raki_Input.h"

class Stage
{
public: // サブクラス
	struct ClipBlock
	{
		/*1つ目*/
		int ReferencePoint1 = -1;
		int blockNumber1 = -1;
		vector<RVector3> vec1 = {};
		vector<int> gateNumber1 = {};
		/*2つ目*/
		int ReferencePoint2 = -1;
		int blockNumber2 = -1;
		vector<RVector3> vec2 = {};
		vector<int> gateNumber2 = {};
		/*プレイヤーの座標*/
		RVector3 playerPos = {};
		RVector3 playerStartPos = {};
		RVector3 playerEndPos = {};
		RVector3 vaniPos = {};
		/*挟めるブロックがあるかどうかのフラグ*/
		bool isClip = false;
		//
		bool isVani = false;
		float backPosZ = 0;
	};

private: // エイリアス
	template<class T> using stack = std::stack<T>;

public: // 定数
	static const size_t maxFlame;

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
	bool GetClipBlocksALL(int blocksArray[], const size_t& sizeArray);
private:
	// ブロックを挟む(2d版)
	int Clip2d(ClipBlock* clip);
	// ブロックを挟む(3d版)
	int Clip3d(ClipBlock* clip);

public: // メンバ変数
	LoadStage stage;
	SoundData clipSE;
	SoundData bottunSE;
	SoundData blockSE;
	SoundData stepBackSE;
	SoundData resetSE;

	bool backFlag;
	bool clipFlag;
private:
	Player* player;
	stack<ClipBlock> clipBlock;
	bool flag2d;

	vector<int> easeNumber;
	vector<RVector3> easeStartPos;
	size_t nowFlame;
	bool isEasing;
};
