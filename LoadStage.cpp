#include "LoadStage.h"
#include <stdio.h>
#include "TexManager.h"
#include "Raki_DX12B.h"

#define EoF (-1) // Error of function

const float LoadStage::blockSize = 20.0f;

LoadStage::LoadStage() :
	debugBox{},
	graph(0),
	startPosNumber(-1)
{
	graph = TexManager::LoadTexture("./Resources/test.jpeg");
	debugBox.CreateBoxModel(blockSize / 2.0f, 1.0f, 1.0f, graph);
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
	using namespace BlockData;

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

		for (size_t i = 0; i < 4; i++)
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
				blocks[blocks.size() - 1].pos = XMFLOAT3((float)blockData[0], (float)blockData[1], (float)blockData[2]);
				blocks[blocks.size() - 1].type = blockData[3];
				blocks[blocks.size() - 1].number = blockData[4];
				blocks[blocks.size() - 1].collision = Collision(
					{ -blockSize / 2.0f, -blockSize / 2.0f, -blockSize / 2.0f },
					{ blockSize / 2.0f, blockSize / 2.0f, blockSize / 2.0f },
					RVector3(0.0f, 0.0f, 0.0f));
				debugBoxObj.push_back(CreateObject3d(&debugBox));
				debugBoxObj[debugBoxObj.size() - 1]->position = blocks[blocks.size() - 1].pos;
			}

			if (blockData[3] == BlockData::BlockType::START)
			{
				startPosNumber = (int)blocks.size() - 1;
			}
		}
	}

	for (size_t i = 0; i < debugBoxObj.size(); i++)
	{
		if (blocks[i].type >= blockColors.size())
		{
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

		debugBoxObj[i]->color = blockColors[number];
	}

	return 0;
}

void LoadStage::Draw()
{
	using namespace BlockData;

	for (size_t i = 0; i < blocks.size(); i++)
	{
		switch (blocks[i].type)
		{
		case BlockType::BLOCK:
			DrawObject3d(debugBoxObj[i]);
			break;
		case BlockType::DONT_MOVE_BLOCK:
			DrawObject3d(debugBoxObj[i]);
			break;
		case BlockType::GOAL:
			DrawObject3d(debugBoxObj[i]);
			break;
		default:
			break;
		}
	}
}

void LoadStage::Reset()
{
	for (size_t i = 0; i < debugBoxObj.size(); i++)
	{
		if (debugBoxObj[i] == nullptr)
		{
			continue;
		}

		debugBoxObj[i]->position.x = blocks[i].pos.x;
		debugBoxObj[i]->position.y = blocks[i].pos.y;
		debugBoxObj[i]->position.z = blocks[i].pos.z;
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

	startPosNumber = -1;
}

XMFLOAT3 LoadStage::GetStartPlayerPos()
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
