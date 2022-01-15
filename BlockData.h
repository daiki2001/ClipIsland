#pragma once
#include <vector>
#include <utility>

namespace BlockData
{
enum BlockType
{
	NONE = -1,
	BLOCK,
	DONT_MOVE_BLOCK,
	GOAL,
	START
};

const std::vector <std::pair<BlockType, bool>> moveFlag =
{
	{BlockType::BLOCK, true},
	{BlockType::DONT_MOVE_BLOCK, false},
	{BlockType::GOAL, true},
	{BlockType::START, false}
};
}
