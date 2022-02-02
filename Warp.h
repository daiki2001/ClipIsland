#pragma once
#include "RVector.h"
#include "NY_Object3DMgr.h"
#include <DirectXMath.h>

class Warp
{
public:
	Warp();
	Warp(const Warp& other);
	~Warp();

	// 描画用オブジェクトの生成
	void CreateObj(const RVector3& pos);
	// 描画
	void Draw();
	// オブジェクトの削除
	void DeleteObject();

	// ワープゲートを開く
	void WarpOpen(const RVector3& forwordVec);

	// オブジェクトの座標の取得
	inline RVector3& GetObjectPos() { return object->position; }
	// オブジェクトの座標の設定
	inline void SetObjectPos(const RVector3& pos) { object->position = pos; }
	// オブジェクトの色の設定
	inline void SetWarpOpenColor(const DirectX::XMFLOAT4& color) { openColor = color; object->color = openColor; }
	// オブジェクトの色の設定
	inline void SetWarpCloseColor(const DirectX::XMFLOAT4& color) { closeColor = color; object->color = closeColor; }

public:
	RVector3 forwordVec; //ワープが開いている方向
	bool isOpen; //ワープが開いているかどうか

	int blockNumber;     //ブロックの番号
	int twinBlockNumber; //対になるブロックの番号
	size_t gateNumber;   //何番目のゲートかどうか
private:
	DirectX::XMFLOAT4 openColor;  //ワープが開いている時の色
	DirectX::XMFLOAT4 closeColor; //ワープが閉じられている時の色

	static UINT warpGraph;       //ワープのテクスチャ
	static NY_Model3D warpModel; //ワープのモデル

	Object3d* object; //ワープのオブジェクト
};
