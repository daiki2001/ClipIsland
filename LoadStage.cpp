#include "LoadStage.h"
#include <stdio.h>
#include "TexManager.h"
#include "Raki_DX12B.h"

#define EoF (-1) // Error of function

LoadStage::LoadStage() :
	blocks{},
	blockColors{},
	debugBoxObj{},
	debugBoxNumber{},
	warpBlock{},
	startPosNumber(-1)
{
}

LoadStage::~LoadStage()
{
	for (size_t i = 0; i < debugBoxObj.size(); i++)
	{
		if (debugBoxObj[i] == nullptr)
		{
			continue;
		}

		DeleteObject3d(debugBoxObj[i]);
		debugBoxObj[i] = nullptr;
	}
}

int LoadStage::Load(const char* filePath)
{
	using namespace GameCommonData::BlockData;

	if (filePath == nullptr) { return EoF; }

	FILE* fileHandle;
	errno_t err;
	char string[256] = { 0 };

	err = fopen_s(&fileHandle, filePath, "r");
	if (err != 0)
	{
		return err;
	}

	bool isMinus = false;
	bool isBlock = false;
	int number = 0;
	int blockData[5] = { 0 };
	float blockColor = 0;

	while (fgets(string, 256, fileHandle) != nullptr)
	{
		isBlock = false;
		number = 0;

		for (size_t i = 0; i < sizeof(blockData) / sizeof(blockData[0]); i++)
		{
			blockData[i] = 0;
		}

		for (int i = 0; string[i] != '\0' || string[i] == '\n'; i++)
		{
			if (string[0] == 'B')
			{
				if (i == 0)
				{
					isBlock = true;
				}

				if (string[i] == ',' || string[i] == '\n')
				{
					if (number >= 0 && number < 3)
					{
						blockData[number] *= (int)blockSize;
					}

					// ŽŸ‚Ì”Žš‚Ö
					number++;
					isMinus = false;
				}
				else if (string[i] == '-')
				{
					isMinus = true;
				}
				else if (string[i] >= '0' && string[i] <= '9')
				{
					blockData[number] *= 10;

					if (isMinus == true)
					{
						blockData[number] -= string[i] - '0';
					}
					else
					{
						blockData[number] += string[i] - '0';
					}
				}
			}
			if (string[0] == 'C')
			{
				if (i == 0)
				{
					blockColors.push_back(XMFLOAT4());
				}

				if (string[i] == ',' || string[i] == '\n')
				{
					switch (number)
					{
					case 0:
						blockColors[blockColors.size() - 1].x = blockColor / 255.0f;
						break;
					case 1:
						blockColors[blockColors.size() - 1].y = blockColor / 255.0f;
						break;
					case 2:
						blockColors[blockColors.size() - 1].z = blockColor / 255.0f;
						break;
					default:
						blockColors[blockColors.size() - 1].w = blockColor / 255.0f;
						break;
					}

					// ŽŸ‚Ì”Žš‚Ö
					number++;
					isMinus = false;
					blockColor = 0.0f;
				}
				else if (string[i] == '-')
				{
					isMinus = true;
				}
				else if (string[i] >= '0' && string[i] <= '9')
				{
					blockColor *= 10;

					if (isMinus == true)
					{
						blockColor -= string[i] - '0';
					}
					else
					{
						blockColor += string[i] - '0';
					}
				}
			}
		}

		if (isBlock)
		{
			if (blockData[3] >= 0)
			{
				blocks.push_back(Data());
				blocks[blocks.size() - 1].pos = RVector3((float)blockData[0], (float)blockData[1], (float)blockData[2]);
				blocks[blocks.size() - 1].resetPos = blocks[blocks.size() - 1].pos;
				blocks[blocks.size() - 1].type = blockData[3];
				blocks[blocks.size() - 1].InitType = blocks[blocks.size() - 1].type;
				blocks[blocks.size() - 1].number = blockData[4];
				blocks[blocks.size() - 1].collision = Collision(
					{ -blockSize / 2.0f, -blockSize / 2.0f, -blockSize / 2.0f },
					{ blockSize / 2.0f, blockSize / 2.0f, blockSize / 2.0f },
					RVector3(0.0f, 0.0f, 0.0f));

				if (blocks[blocks.size() - 1].type == BlockType::WARP_CLOSE_BLOCK ||
					blocks[blocks.size() - 1].type == BlockType::WARP_OPEN_BLOCK)
				{
					warpBlock.push_back(Warp());
					warpBlock[warpBlock.size() - 1].blockNumber = (int)blocks.size() - 1;
					warpBlock[warpBlock.size() - 1].CreateObj(blocks[blocks.size() - 1].pos);
					warpBlock[warpBlock.size() - 1].isOpen = blocks[blocks.size() - 1].type == BlockType::WARP_OPEN_BLOCK;
				}
				else
				{
					debugBoxObj.push_back(CreateObject3d(&CommonData::boxModel));
					debugBoxNumber.push_back((int)blocks.size() - 1);
					debugBoxObj[debugBoxObj.size() - 1]->position = blocks[blocks.size() - 1].pos;
				}
			}

			if (blockData[3] == BlockType::START)
			{
				startPosNumber = (int)blocks.size() - 1;
			}
		}
	}

	size_t warpBlockCount = 0;
	size_t debugBlockCount = 0;

	for (size_t i = 0; i < debugBoxObj.size(); i++)
	{
		if (blocks[i].type >= blockColors.size())
		{
			if (blocks[i].type == BlockType::START)
			{
				debugBoxObj[debugBlockCount]->color = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
				debugBlockCount++;
			}

			continue;
		}

		if (blocks[i].type <= BlockType::NONE)
		{
			continue;
		}
		else
		{
			number = blocks[i].type;
		}

		if (blocks[i].type == BlockType::START)
		{
			debugBoxObj[debugBlockCount]->color = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
			debugBlockCount++;
		}
		else if (number == BlockType::WARP_CLOSE_BLOCK)
		{
			warpBlock[warpBlockCount].SetWarpCloseColor(blockColors[number]);
			warpBlockCount++;
		}
		else if (number == BlockType::WARP_OPEN_BLOCK)
		{
			warpBlock[warpBlockCount].SetWarpOpenColor(blockColors[number]);
			warpBlockCount++;
		}
		else
		{
			debugBoxObj[debugBlockCount]->color = blockColors[number];
			debugBlockCount++;
		}
	}

	return 0;
}

void LoadStage::Update()
{
	size_t warpBlockCount = 0;
	size_t debugBlockCount = 0;

	for (size_t i = 0; i < blocks.size(); i++)
	{
		if (warpBlockCount < warpBlock.size() &&
			i == warpBlock[warpBlockCount].blockNumber)
		{
			warpBlock[warpBlockCount].SetObjectPos(blocks[i].pos);
			warpBlockCount++;
		}
		if (debugBlockCount < debugBoxNumber.size() &&
			i == debugBoxNumber[debugBlockCount])
		{
			debugBoxObj[debugBlockCount]->position = blocks[i].pos;
			debugBlockCount++;
		}
	}
}

void LoadStage::Draw()
{
	size_t warpBlockCount = 0;
	size_t debugBlockCount = 0;

	for (size_t i = 0; i < blocks.size(); i++)
	{
		switch (blocks[i].type)
		{
		case BlockType::BLOCK:
			DrawObject3d(debugBoxObj[debugBlockCount]);
			debugBlockCount++;
			break;
		case BlockType::DONT_MOVE_BLOCK:
			DrawObject3d(debugBoxObj[debugBlockCount]);
			debugBlockCount++;
			break;
		case BlockType::GOAL:
			DrawObject3d(debugBoxObj[debugBlockCount]);
			debugBlockCount++;
			break;
		case BlockType::SWITCH:
			DrawObject3d(debugBoxObj[debugBlockCount]);
			debugBlockCount++;
			break;
		case BlockType::DOOR:
			DrawObject3d(debugBoxObj[debugBlockCount]);
			debugBlockCount++;
			break;
		case BlockType::WARP_CLOSE_BLOCK:
		case BlockType::WARP_OPEN_BLOCK:
			warpBlock[warpBlockCount].Draw();
			warpBlockCount++;
			break;
		default:
			debugBlockCount++;
			break;
		}
	}
}

void LoadStage::Reset()
{
	size_t warpBlockCount = 0;
	size_t debugBlockCount = 0;

	for (size_t i = 0; i < blocks.size(); i++)
	{
		if (warpBlockCount < warpBlock.size() &&
			i == warpBlock[warpBlockCount].blockNumber)
		{
			warpBlock[warpBlockCount].SetObjectPos(blocks[i].resetPos);
			warpBlockCount++;
		}
		if (debugBlockCount < debugBoxNumber.size() &&
			i == debugBoxNumber[debugBlockCount])
		{
			if (debugBoxObj[debugBlockCount] == nullptr)
			{
				continue;
			}

			debugBoxObj[debugBlockCount]->position = blocks[i].resetPos;
			debugBlockCount++;
		}

		blocks[i].pos = blocks[i].resetPos;
		blocks[i].type = blocks[i].InitType;
	}
}

void LoadStage::StageClear()
{
	for (size_t i = 0; i < debugBoxObj.size(); i++)
	{
		if (debugBoxObj[i] == nullptr)
		{
			continue;
		}

		DeleteObject3d(debugBoxObj[i]);
		debugBoxObj[i] = nullptr;
	}

	blocks.clear();
	blockColors.clear();
	debugBoxObj.clear();

	blocks.shrink_to_fit();
	blockColors.shrink_to_fit();
	debugBoxObj.shrink_to_fit();

	startPosNumber = -1;
}

RVector3 LoadStage::GetStartPlayerPos()
{
	size_t num;

	if (startPosNumber < 0)
	{
		num = 0;
	}
	else
	{
		num = startPosNumber;
	}

	return debugBoxObj[num]->position;
}

void LoadStage::GetBlocksTypeAll(BlockType blockType, int blocksArray[], size_t arraySize)
{
	for (size_t i = 0; i < arraySize; i++)
	{
		blocksArray[i] = -1;
	}

	for (int i = 0, j = 0; i < blocks.size(); i++)
	{
		if (j >= arraySize)
		{
			break;
		}

		if (blocks[i].type == blockType)
		{
			blocksArray[j] = i;
			j++;
		}
	}
}
