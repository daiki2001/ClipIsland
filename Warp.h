#pragma once
#include "RVector.h"
#include "NY_Object3DMgr.h"
#include <DirectXMath.h>

class Warp
{
public:
	Warp();
	~Warp();

	// 描画用オブジェクトの生成
	void CreateObj(RVector3 pos);
	// 描画
	void Draw();

	// ワープゲートを開く
	void WarpOpen(const RVector3 pos, const RVector3& forwordVec);

	// オブジェクトの座標の取得
	inline RVector3& GetObjectPos() { return object->position; }
	// オブジェクトの座標の設定
	inline void SetObjectPos(const RVector3& pos) { object->position = pos; }
	// オブジェクトの色の設定
	inline void SetWarpOpenColor(const DirectX::XMFLOAT4& color) { openColor = color; }
	// オブジェクトの色の設定
	inline void SetWarpCloseColor(const DirectX::XMFLOAT4& color) { closeColor = color; }

public:
	RVector3 forwordVec; //ワープが開いている方向

	bool isOpen; //ワープが開いているかどうか

	int blockNumber; //ブロックの番号
private:
	DirectX::XMFLOAT4 openColor;  //ワープが開いている時の色
	DirectX::XMFLOAT4 closeColor; //ワープが閉じられている時の色

	NY_Model3D boxModel; //ワープが設置出来る箱のモデル

	UINT warpGraph;       //ワープのテクスチャ
	NY_Model3D warpModel; //ワープのモデル

	Object3d* object;    //ワープのオブジェクト
};
