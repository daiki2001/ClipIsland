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
	using StageBlockModels = GameCommonData::StageBlockModels;

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

	// スイッチのモデルデータの切り替え
	void ChangeSwitchModel(NY_Model3D* modelData);

	// プレイヤーのスタート位置の取得
	RVector3 GetStartPlayerPos();
	// ブロックの情報を種類を指定して全て取得
	void GetBlocksTypeAll(BlockType blockType, int blocksArray[], size_t arraySize);

	// ブロックを塊ごとでソートする関数
	void blockSortIsNumber(int start, int end);
private:
	// パーテーションを行う関数
	int PartitionIsBlockNumber(int p, int r);

public: // メンバ変数
	vector<Data> blocks;             //ブロックの情報
	vector<XMFLOAT4> blockColors;    //ブロックの色
	vector<int> multipleBlockNumber; //ブロックが複数個有るやつの番号

	vector<Warp*> warpBlock; //ワープブロックの情報
	size_t openGateCount;
private:
	int startPosNumber; //開始時のプレイヤーの座標(配列の要素番号)

	vector<Object3d*> moveBlockObj;   //動かせるブロックのオブジェクト
	vector<int> moveBlockNumber;      //動かせるブロックの番号
	vector<Object3d*> stayBlockObj;   //動かないブロックのオブジェクト
	vector<int> stayBlockNumber;      //動かないブロックの番号
	vector<Object3d*> goalBlockObj;   //ゴールブロックのオブジェクト
	vector<int> goalBlockNumber;      //ゴールブロックの番号
	vector<Object3d*> switchBlockObj; //スイッチのオブジェクト
	vector<int> switchBlockNumber;    //スイッチの番号
	vector<Object3d*> doorBlockObj; //スイッチで消えるブロックのオブジェクト
	vector<int> doorBlockNumber;    //スイッチで消えるブロックの番号

	vector<Object3d*> debugBoxObj; //デバッグ用のブロックのオブジェクト
	vector<int> debugBoxNumber;    //デバッグ用のブロックの番号

};
