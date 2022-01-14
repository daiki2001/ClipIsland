#pragma once
#include "LoadStage.h"
#include "RVector.h"
#include <stack>

class Stage
{
public: // サブクラス
	struct ClipBlock
	{
		/*1つ目*/
		int blockNumber1;
		RVector3 pos1;
		/*2つ目*/
		int blockNumber2;
		RVector3 pos2;
	};

private: // エイリアス
	template<class T> using stack = std::stack<T>;

public: // メンバ関数
	Stage();
	~Stage();

	// 更新
	void Update();
	// 描画
	void Draw();

	// 一手戻る
	void StepBack();
	// リセット
	void Reset();

private: // メンバ変数
	LoadStage* stage;
	stack<ClipBlock> clipBlock;

};
