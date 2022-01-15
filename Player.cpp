#include "Player.h"
#include "Raki_Input.h"
#include "LoadStage.h"

Player::Player() :
	position(0.0f, 0.0f, 0.0f),
	forwardVec(1.0f, 0.0f, 0.0f),
	model{},
	object(nullptr)
{
	model.LoadObjModel("player");
	object = CreateObject3d(&model);

	object->position.z = -20.0f;
	object->scale = { 10.0f, 10.0f, 10.0f };
}

Player::~Player()
{
	if (object != nullptr)
	{
		DeleteObject3d(object);
		object = nullptr;
	}
}

void Player::Update()
{
	if (Input::isKeyTrigger(DIK_W))
	{
		forwardVec = { 0.0f, 1.0f, 0.0f };
		position += forwardVec * LoadStage::blockSize;
	}
	if (Input::isKeyTrigger(DIK_A))
	{
		forwardVec = { -1.0f, 0.0f, 0.0f };
		position += forwardVec * LoadStage::blockSize;
	}
	if (Input::isKeyTrigger(DIK_S))
	{
		forwardVec = { 0.0f, -1.0f, 0.0f };
		position += forwardVec * LoadStage::blockSize;
	}
	if (Input::isKeyTrigger(DIK_D))
	{
		forwardVec = { 1.0f, 0.0f, 0.0f };
		position += forwardVec * LoadStage::blockSize;
	}

	object->position = position;
}

void Player::Draw()
{
	DrawObject3d(object);
}
