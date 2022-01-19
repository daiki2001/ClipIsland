#pragma once
#include <DirectXMath.h>
#include "RVector.h"

class Collision
{  
private: // �G�C���A�X
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	RVector3 max;		//�ő�l
	RVector3 min;		//�ŏ��l
	RVector3 oldPos;	//�O�̍��W�l

public:
	Collision();
	~Collision();

	Collision(RVector3 minValue, RVector3 maxValue, RVector3 actorPos);

	void Update(const RVector3 &actorPosition);
};

bool intersectAABB(const Collision &box1, const Collision &box2);