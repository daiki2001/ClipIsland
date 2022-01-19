#pragma once
#include <DirectXMath.h>
#include <vector>
#include "NY_Object3DMgr.h"
#include <d3d12.h>
#include "Collision.h"
#include "BlockData.h"

class LoadStage
{
private: // エイリアス
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	template<class T> using vector = std::vector<T>;

public: // 定数

public: // メンバ関数
	// コンストラクタ
	LoadStage();
	// デストラクタ
	~LoadStage();

	// 読み込み
	int Load(const char* filePath);
	// 描画
	void Draw();

	// ブロックの場所のリセット
	void Reset();
	// ステージデータの削除
	void StageClear();
	// プレイヤーの初期位置の取得
	XMFLOAT3 GetStartPlayerPos();

public: // メンバ変数
	vector<BlockData::Data> blocks; //ブロックのデータ
	vector<XMFLOAT4> blockColors;  //ブロックの色
	vector<Object3d*> debugBoxObj; //ブロックのオブジェクト
private:
	int startPosNumber; //開始時のプレイヤーの座標(配列の要素番号)

	NY_Model3D debugBox; //ブロックのモデル
	UINT graph;          //ブロックのテクスチャ

};
