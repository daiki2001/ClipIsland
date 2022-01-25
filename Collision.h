#pragma once
#include <DirectXMath.h>
#include "RVector.h"

class Collision
{  
private: // エイリアス
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	RVector3 max;		//最大値
	RVector3 min;		//最小値
	RVector3 oldPos;	//前の座標値

public:
	Collision();
	~Collision();

	Collision(RVector3 minValue, RVector3 maxValue, RVector3 actorPos);

	void Update(const RVector3 &actorPosition);
};

bool intersectAABB(const Collision &box1, const Collision &box2);