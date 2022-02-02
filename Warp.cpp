#include "Warp.h"
#include "GameCommonData.h"
#include "TexManager.h"

UINT Warp::warpGraph = UINT(-1);
NY_Model3D Warp::warpModel = {};

Warp::Warp() :
	forwordVec(1.0f, 0.0f, 0.0f),
	isOpen(false),
	blockNumber(-1),
	twinBlockNumber(-1),
	gateNumber(size_t(-1)),
	openColor(1.0f, 1.0f, 1.0f, 1.0f),
	closeColor(1.0f, 1.0f, 1.0f, 1.0f),
	object(nullptr)
{
	if (warpGraph == UINT(-1))
	{
		warpGraph = TexManager::LoadTexture("./Resources/warp.png");
		warpModel.CreateBoxModel(GameCommonData::BlockData::blockSize / 2.0f, 1.0f, 1.0f, warpGraph);
	}
}

Warp::Warp(const Warp& other)
{
	*this = other;

	if (warpGraph == UINT(-1))
	{
		warpGraph = TexManager::LoadTexture("./Resources/warp.png");
		warpModel.CreateBoxModel(GameCommonData::BlockData::blockSize / 2.0f, 1.0f, 1.0f, warpGraph);
	}
}

Warp::~Warp()
{
	DeleteObject();
}

void Warp::CreateObj(const RVector3& pos)
{
	if (object != nullptr)
	{
		DeleteObject3d(object);
		object = nullptr;
	}

	this->object = CreateObject3d(&GameCommonData::StageBlockModels::debugBoxModel);
	this->object->position = pos;
	this->object->color = closeColor;
	this->isOpen = false;
}

void Warp::Draw()
{
	DrawObject3d(object);
}

void Warp::DeleteObject()
{
	if (object == nullptr)
	{
		return;
	}

	DeleteObject3d(object);
	object = nullptr;
}

void Warp::WarpOpen(const RVector3& forwordVec)
{
	this->forwordVec = forwordVec;
	this->isOpen = true;

	if (this->object != nullptr)
	{
		DeleteObject3d(this->object);
		this->object = nullptr;
	}

	this->object = CreateObject3d(&warpModel);

	if (this->forwordVec == RVector3(0.0f, 1.0f, 0.0f))
	{
		this->object->rotation = RVector3(0.0f, 0.0, 0.0f);
	}
	else if (this->forwordVec == RVector3(1.0f, 0.0f, 0.0f))
	{
		this->object->rotation = RVector3(0.0f, 0.0, 270.0f);
	}
	else if (this->forwordVec == RVector3(0.0f, -1.0f, 0.0f))
	{
		this->object->rotation = RVector3(0.0f, 0.0, 180.0f);
	}
	else if (this->forwordVec == RVector3(-1.0f, 0.0f, 0.0f))
	{
		this->object->rotation = RVector3(0.0f, 0.0, 90.0f);
	}
}
