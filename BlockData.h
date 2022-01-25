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

// ‹²‚ÞŽž‚Éˆø‚Á‚©‚©‚é‚©‚Ç‚¤‚©‚Ìƒtƒ‰ƒO
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
