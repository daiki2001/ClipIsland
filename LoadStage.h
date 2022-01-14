#pragma once
#include <DirectXMath.h>
#include <vector>
#include "NY_Object3DMgr.h"
#include <d3d12.h>

enum BlockType
{
	NONE = -1,
	BLOCK,
	DONT_MOVE_BLOCK,
	GOAL,
	START
};

class LoadStage
{
private: // エイリアス
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	template<class T> using vector = std::vector<T>;

public: // メンバ関数
	// コンストラクタ
	LoadStage();
	// デストラクタ
	~LoadStage() {}

	// 読み込み
	int Load(const char* filePath);
	// 描画
	void Draw();

	// ブロックの場所のリセット
	void Reset();

public: // メンバ変数
	vector<int> blockType;         //ブロックの種類
	vector<XMFLOAT4> blockColors;  //ブロックの色
	vector<Object3d*> debugBoxObj; //ブロックのオブジェクト
private:
	vector<XMFLOAT3> blockPos; //ブロックの場所

	NY_Model3D debugBox; //ブロックのモデル
	UINT graph;          //ブロックのテクスチャ

};
