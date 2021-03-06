#include "Player.h"
#include "Raki_Input.h"
#include "LoadStage.h"

Player::Player() :
	position(0.0f, 0.0f, 0.0f),
	startPos(0.0f, 0.0f, 0.0f),
	endPos(0.0f, 0.0f, 0.0f),
	forwardVec(1.0f, 0.0f, 0.0f),
	playerRot(-90, 0, 0),
	model{},
	object(nullptr),
	playerCollision(RVector3(-5, -5, -5) + position, RVector3(5, 5, 5) + position, position),
	playerOldPos(0.0f, 0.0f, 0.0f),
	Tposition(0.0f, 0.0f, 0.0f)
{
	model.LoadObjModel("player");
	object = CreateObject3d(&model);
	object->position = RVector3(0.0f, 0.0f, 0.0f);
	object->scale = { 6.0f, 6.0f, 6.0f };
	object->rotation = RVector3(-90, 0, 0);
	object->color = { 1, 1, 1, 1 };
	goalFlag = false;
	moveFlag = false;
	playerFlag = true;
	easeFlag = false;
	stepbackFlag = false;
	timeRate = 0;
	walkSE=Audio::LoadSound_wav("./Resources/Sound/walkSE.wav");

	ReverseAndResetDir::Get()->Init();
}

//時間計測に必要なデータ
const float maxFrame = 10;
float nowFrame = 100;


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
	if (moveFlag == false && easeFlag == false && stepbackFlag == false);
	{
		if (Input::isKeyTrigger(DIK_W))
		{
			moveFlag = true;
			nowFrame = 0;

			playerRot = RVector3(-90, 0, 0);
			forwardVec = { 0.0f, 1.0f, 0.0f };


			startPos = playerOldPos;
			endPos = playerOldPos + forwardVec * blockSize;

			Tposition = endPos;

			Audio::PlayLoadedSound(walkSE);

		}
		else if (Input::isKeyTrigger(DIK_A))
		{
			moveFlag = true;
			nowFrame = 0;

			forwardVec = { -1.0f, 0.0f, 0.0f };

			playerRot = RVector3(-180, 90, 270);
			startPos = playerOldPos;
			endPos = playerOldPos + forwardVec * blockSize;

			Tposition = endPos;

			Audio::PlayLoadedSound(walkSE);
		}
		else if (Input::isKeyTrigger(DIK_S))
		{
			moveFlag = true;
			nowFrame = 0;

			forwardVec = { 0.0f, -1.0f, 0.0f };

			playerRot = RVector3(-270, 90, 270);
			startPos = playerOldPos;
			endPos = playerOldPos + forwardVec * blockSize;

			Tposition = endPos;

			Audio::PlayLoadedSound(walkSE);
		}
		else if (Input::isKeyTrigger(DIK_D))
		{
			moveFlag = true;
			nowFrame = 0;

			forwardVec = { 1.0f, 0.0f, 0.0f };

			playerRot = RVector3(-180, -90, 90);
			startPos = playerOldPos;
			endPos = playerOldPos + forwardVec * blockSize;

			Tposition = endPos;

			Audio::PlayLoadedSound(walkSE);
		}

	}

	if (nowFrame < maxFrame)
	{
		nowFrame++;
	}
	else
	{
		startPos = position;
		moveFlag = false;
	}

	timeRate = nowFrame / maxFrame;

	if (moveFlag == true)
	{
		position = Rv3Ease::lerp(startPos, endPos, timeRate);
	} 

	playerCollision.Update(Tposition);

	if (playerFlag == true)
	{
		object->position = position;
		object->rotation = playerRot;
	}
	else
	{
		position = startPos;
		object->position = startPos;
		object->rotation = playerRot;
	}

	//if (moveFlag == true)
	//{
	//	position = Rv3Ease::lerp(startPos, endPos, timeRate);
	//}

	//object->position = position;
	//object->rotation = playerRot;

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
	Player::forwardVec = RVector3(0.0f, 1.0f, 0.0f);
	Player::playerRot = RVector3(-90, 0, 0);
	Player::playerCollision = Collision({ -5, -5, -5 }, { 5,5,5 }, position);
	Player::playerOldPos = position;
	Player::startPos = position;
	Player::endPos = position;
	Player::Tposition = position;
	object->position = position;
	object->rotation = playerRot;
	object->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	goalFlag = false;
	moveFlag = false;
	timeRate = 0.0f;
}

