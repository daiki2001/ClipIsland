#pragma once
#include "RVector.h"
#include "Collision.h"
#include "NY_Object3DMgr.h"
#include <vector>
#include <utility>

namespace BlockData
{
const float blockSize = 20.0f;

struct Data
{
	int type = -1;                       //�u���b�N�̎��
	RVector3 pos = { 0.0f, 0.0f, 0.0f }; //�u���b�N�̍��W
	int number = -1;                     //�u���b�N�̉�
	Collision collision = Collision(
		RVector3(-blockSize / 2.0f, -blockSize / 2.0f, -blockSize / 2.0) + pos,
		RVector3(blockSize / 2.0f, blockSize / 2.0f, blockSize / 2.0f) + pos,
		pos);                   //�u���b�N�̓����蔻��
	//Object3d* object = nullptr; //�u���b�N�̃I�u�W�F�N�g
};

enum BlockType
{
	NONE = -1,
	BLOCK,
	DONT_MOVE_BLOCK,
	GOAL,
	START,
	WARP_BLOCK,
	WARP_BLOCK_SET
};

const std::vector <std::pair<BlockType, bool>> moveFlag =
{
	{BlockType::BLOCK, true},
	{BlockType::DONT_MOVE_BLOCK, false},
	{BlockType::GOAL, true},
	{BlockType::START, false},
	{BlockType::WARP_BLOCK, true},
	{BlockType::WARP_BLOCK_SET, false}
};

// ���ގ��Ɉ��������邩�ǂ����̃t���O
const std::vector <std::pair<BlockType, bool>> caughtFlag =
{
	{BlockType::BLOCK, false},
	{BlockType::DONT_MOVE_BLOCK, true},
	{BlockType::GOAL, true},
	{BlockType::START, false},
	{BlockType::WARP_BLOCK, true},
	{BlockType::WARP_BLOCK_SET, false}
};
}
