#pragma once
#include <DirectXMath.h>
#include "RVector.h"
#include <vector>
#include "NY_Object3DMgr.h"
#include "Warp.h"
#include <d3d12.h>
#include "Collision.h"
#include "GameCommonData.h"

class LoadStage
{
private: // エイリアス
	using XMFLOAT4 = DirectX::XMFLOAT4;
	template<class T> using vector = std::vector<T>;
	using BlockType = GameCommonData::BlockData::BlockType;
	using Data = GameCommonData::BlockData::Data;
	using CommonData = GameCommonData::CommonData;

public: // メンバ関数
	// コンストラクタ
	LoadStage();
	// デストラクタ
	~LoadStage();

	// 読み込み
	int Load(const char* filePath);
	// 更新
	void Update();
	// 描画
	void Draw();

	// ブロックの場所のリセット
	void Reset();
	// ステージデータの削除
	void StageClear();

	// プレイヤーのスタート位置の取得
	RVector3 GetStartPlayerPos();
	// ブロックの情報を種類を指定して全て取得
	void GetBlocksTypeAll(BlockType blockType, int blocksArray[], size_t arraySize);

public: // メンバ変数
	vector<Data> blocks;          //ブロックの情報
	vector<XMFLOAT4> blockColors; //ブロックの色

	vector<Warp*> warpBlock; //ワープブロックの情報
	size_t openGateCount;
private:
	int startPosNumber; //開始時のプレイヤーの座標(配列の要素番号)

	vector<Object3d*> debugBoxObj; //デバッグ用のブロックのオブジェクト
	vector<int> debugBoxNumber;    //デバッグ用のブロックの番号

};
