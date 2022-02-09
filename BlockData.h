#pragma once
#include "RVector.h"
#include "Collision.h"
#include "NY_Object3DMgr.h"
#include <vector>
#include <utility>

namespace GameCommonData
{
namespace BlockData
{
const float blockSize = 20.0f;

struct Data
{
	int type = -1;                            //ブロックの種類
	int InitType = -1;                        //ブロックの種類(初期化用)
	RVector3 pos = { 0.0f, 0.0f, 0.0f };      //ブロックの座標
	RVector3 resetPos = { 0.0f, 0.0f, 0.0f }; //ブロックの座標(初期化用)
	int number = -1;                          //ブロックの塊
	Collision collision = Collision(
		RVector3(-blockSize / 2.0f, -blockSize / 2.0f, -blockSize / 2.0) + pos,
		RVector3(blockSize / 2.0f, blockSize / 2.0f, blockSize / 2.0f) + pos,
		pos);                   //ブロックの当たり判定
	//Object3d* object = nullptr; //ブロックのオブジェクト
};

enum BlockType
{
	NONE = -1,
	BLOCK,
	DONT_MOVE_BLOCK,
	GOAL,
	START,
	SWITCH,
	DOOR,
	WARP_CLOSE_BLOCK,
	WARP_OPEN_BLOCK
};

const std::vector <std::pair<BlockType, bool>> moveFlag =
{
	{BlockType::BLOCK, true},
	{BlockType::DONT_MOVE_BLOCK, false},
	{BlockType::GOAL, true},
	{BlockType::START, false},
	{BlockType::SWITCH, true},
	{BlockType::DOOR, false},
	{BlockType::WARP_CLOSE_BLOCK, true},
	{BlockType::WARP_OPEN_BLOCK, false}
};

// はさむときに引っかかるか
const std::vector <std::pair<BlockType, bool>> caughtFlag =
{
	{BlockType::BLOCK, true},
	{BlockType::DONT_MOVE_BLOCK, true},
	{BlockType::GOAL, true},
	{BlockType::START, false},
	{BlockType::SWITCH, true},
	{BlockType::DOOR, true},
	{BlockType::WARP_CLOSE_BLOCK, true},
	{BlockType::WARP_OPEN_BLOCK, false}
};
} //BlockData
} //GameCommonData
