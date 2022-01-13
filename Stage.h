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
		RVector3 pos1;
		int type1;
		/*2つ目*/
		RVector3 pos2;
		int type2;
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

	// リセット
	void Reset() { stage->Reset(); }

private: // メンバ変数
	LoadStage* stage;
	stack<ClipBlock> clipBlock;

};
