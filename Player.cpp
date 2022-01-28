#include "Player.h"
#include "Raki_Input.h"
#include "LoadStage.h"

Player::Player() :
	position(0.0f, 0.0f, 0.0f),
	forwardVec(1.0f, 0.0f, 0.0f),
	playerRot(-90, 0, 0),
	model{},
	object(nullptr),
	playerCollision(RVector3(-5, -5, -5) + position, RVector3(5, 5, 5) + position, position),
	playerOldPos(0.0f, 0.0f, 0.0f)
{
	model.LoadObjModel("player");
	object = CreateObject3d(&model);
	object->position = RVector3(0.0f, 0.0f, 0.0f);
	object->scale = { 10.0f, 10.0f, 10.0f };
	object->rotation = RVector3(-90, 0, 0);
	object->color = { 1, 1, 1, 1 };
	goalFlag = false;
}

//時間計測に必要なデータ
long long startCount = 0;
long long Count = 0;
long long nowCount = 0;
long long elapsedCount = 0;
float maxTime = 5.0f;			//全体時間[s]
float timeRate;					//何％ 時間進んだか(率)

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
	using namespace GameCommonData::BlockData;

	playerOldPos = position;

	startCount = 0;
	Count++;

	if (Input::isKeyTrigger(DIK_W))
	{
		forwardVec = { 0.0f, 1.0f, 0.0f };
		position += forwardVec * blockSize * 1.0f;
		playerRot = RVector3(-90, 0, 0);
	}
	else if (Input::isKeyTrigger(DIK_A))
	{
		forwardVec = { -1.0f, 0.0f, 0.0f };
		position += forwardVec * blockSize * 1.0f;
		playerRot = RVector3(-180, 90, 270);
	}
	else if (Input::isKeyTrigger(DIK_S))
	{
		forwardVec = { 0.0f, -1.0f, 0.0f };
		position += forwardVec * blockSize * 1.0f;
		playerRot = RVector3(-270, 90, 270);
	}
	else if (Input::isKeyTrigger(DIK_D))
	{
		forwardVec = { 1.0f, 0.0f, 0.0f };
		position += forwardVec * blockSize * 1.0f;
		playerRot = RVector3(-180, -90, 90);
	}

	playerCollision.Update(position);

	object->position = position;
	object->rotation = playerRot;

	object->position += offsetPos;
}

void Player::Draw()
{

	DrawObject3d(object);
}

void Player::PushBack()
{

}

void Player::Reset()
{
	Player::position = RVector3(0.0f, 0.0f, 0.0f);
	Player::forwardVec = RVector3(1.0f, 0.0f, 0.0f);
	Player::playerRot = RVector3(-90, 0, 0);
	Player::playerCollision = Collision({ -5, -5, -5 }, { 5,5,5 }, position);
	Player::playerOldPos = RVector3(0.0f, 0.0f, 0.0f);
	object->position = RVector3(0.0f, 0.0f, 0.0f);
	object->scale = { 10.0f, 10.0f, 10.0f };
	object->rotation = RVector3(-90, 0, 0);
	object->color = { 1, 1, 1, 1 };
	goalFlag = false;
}

 //const RVector3 
// (const RVector3 &start, const RVector3 &end, const float t)
//{
////	return start * (1.0f - t) + end * t;
//}
