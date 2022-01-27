#include "Warp.h"
#include "GameCommonData.h"
#include "TexManager.h"

Warp::Warp() :
	forwordVec(1.0f, 0.0f, 0.0f),
	openColor(1.0f, 1.0f, 1.0f, 1.0f),
	closeColor(1.0f, 1.0f, 1.0f, 1.0f),
	blockNumber(-1),
	isOpen(false),
	object(nullptr)
{
	warpGraph = TexManager::LoadTexture("./Resources/warp.png");
	warpModel.CreateBoxModel(GameCommonData::BlockData::blockSize / 2.0f, 1.0f, 1.0f, warpGraph);
}

Warp::~Warp()
{
	if (object == nullptr)
	{
		return;
	}

	DeleteObject3d(object);
	object = nullptr;
}

void Warp::CreateObj(RVector3 pos)
{
	if (object != nullptr)
	{
		DeleteObject3d(object);
		object = nullptr;
	}

	this->object = CreateObject3d(&GameCommonData::CommonData::boxModel);
	this->object->position = pos;
}

void Warp::Draw()
{
	DrawObject3d(object);
}

void Warp::WarpOpen(const RVector3 pos, const RVector3& forwordVec)
{
	this->forwordVec = forwordVec;
	this->isOpen = true;

	if (object != nullptr)
	{
		DeleteObject3d(object);
		object = nullptr;
	}

	object = CreateObject3d(&warpModel);
	this->object->position = pos;
}
