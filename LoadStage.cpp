#include "LoadStage.h"
#include <stdio.h>
#include "TexManager.h"
#include "Raki_DX12B.h"

#define EoF (-1) // Error of function

const float LoadStage::blockSize = 20.0f;

LoadStage::LoadStage() :
	debugBox{},
	graph(0),
	BlockCollision{}/*({ -20 / 2,-20 / 2,-20 / 2 }, { 20 / 2,20 / 2,20 / 2 }, RVector3{ 0,0,0 })*/
{
	graph = TexManager::LoadTexture("./Resources/test.jpeg");
	debugBox.CreateBoxModel(blockSize / 2.0f, 1.0f, 1.0f, graph);
	BlockCollision.push_back(Collision({ -20 / 2,-20 / 2,-20 / 2 }, { 20 / 2,20 / 2,20 / 2 }, RVector3{ 0,0,0 }));//Collisionê∂ê¨
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

					// éüÇÃêîéöÇ÷
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

					// éüÇÃêîéöÇ÷
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
				blockPos.push_back(XMFLOAT3((float)blockData[0], (float)blockData[1], (float)blockData[2]));
				blockType.push_back(blockData[3]);
				blockNumber.push_back(blockData[4]);
				debugBoxObj.push_back(CreateObject3d(&debugBox));
				debugBoxObj[debugBoxObj.size() - 1]->position = blockPos[blockPos.size() - 1];
			}
		}
	}

	for (size_t i = 0; i < debugBoxObj.size(); i++)
	{
		if (blockType[i] >= blockColors.size())
		{
			continue;
		}

		if (blockType[i] <= BlockType::NONE)
		{
			continue;
		}
		else
		{
			number = blockType[i];
		}

		debugBoxObj[i]->color = blockColors[number];
	}

	return 0;
}

void LoadStage::Draw()
{
	using namespace BlockData;

	for (size_t i = 0; i < blockPos.size(); i++)
	{
		switch (blockType[i])
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

		debugBoxObj[i]->position.x = blockPos[i].x;
		debugBoxObj[i]->position.y = blockPos[i].y;
		debugBoxObj[i]->position.z = blockPos[i].z;
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

	blockPos.clear();
	blockType.clear();
	blockNumber.clear();
	blockColors.clear();
	debugBoxObj.clear();
}
