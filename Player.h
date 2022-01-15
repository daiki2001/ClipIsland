#pragma once
#include "RVector.h"
#include "NY_Object3DMgr.h"

class Player
{
public:
	Player();
	~Player();

	// 更新
	void Update();
	// 描画
	void Draw();

public:
	RVector3 position;   //プレイヤーの座標
	RVector3 forwardVec; //進行方向
private:
	NY_Model3D model; //プレイヤーのモデル
	Object3d* object; //プレイヤーのオブジェクト

};
