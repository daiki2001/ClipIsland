#pragma once
#include "RVector.h"
#include "NY_Object3DMgr.h"
#include "Collision.h"
#include "Audio.h"

#include "ReverseAndResetDir.h"

class Player
{
public:
	Player();
	~Player();

	// 更新
	void Update();
	// 描画
	void Draw();

	// 押し戻し処理
	void PushBack();

	// リセット
	void Reset();

public:
	RVector3 position;   //プレイヤーの座標
	RVector3 playerOldPos;   //プレイヤーの前座標
	RVector3 forwardVec; //進行方向
	RVector3 playerRot;	 //プレイヤーの回転角
	bool goalFlag;
	bool moveFlag;
	RVector3 startPos;
	RVector3 endPos;

	float timeRate;

	RVector3 Tposition;
	bool playerFlag;
	bool easeFlag;	//動いてたらtrue
	bool stepbackFlag;	//動いてたらtrue

	Collision playerCollision;

	const RVector3 offsetPos = RVector3(0, 0, -20); //描画ずれを補正するために加算する
	Object3d *object; //プレイヤーのオブジェクト

	SoundData walkSE;
private:
	NY_Model3D model; //プレイヤーのモデル
};