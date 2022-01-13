#include "LoadStage.h"
#include <stdio.h>
#include "TexManager.h"
#include "Raki_DX12B.h"

#define EoF (-1) // Error of function

LoadStage::LoadStage() :
	debugBox{},
	graph(0)
{
	graph = TexManager::LoadTexture("./Resources/test.jpeg");
	debugBox.CreateBoxModel(10.0f, 1.0f, 1.0f, graph);
}

int LoadStage::Load(const char* filePath)
{
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
	int number = 0;
	int blockData = 0;
	float blockColor = 0;

	while (fgets(string, 256, fileHandle) != nullptr)
	{
		number = 0;

		for (int i = 0; string[i] != '\0' || string[i] == '\n'; i++)
		{
			if (string[0] == 'B')
			{
				if (i == 0)
				{
					blockPos.push_back(XMFLOAT3());
					blockType.push_back(BlockType());
					debugBoxObj.push_back(CreateObject3d(&debugBox));
				}

				if (string[i] == ',' || string[i] == '\n')
				{
					switch (number)
					{
					case 0:
						blockPos[blockPos.size() - 1].x = (float)blockData;
						debugBoxObj[debugBoxObj.size() - 1]->position.x = blockPos[blockPos.size() - 1].x * 20.0f;
						break;
					case 1:
						blockPos[blockPos.size() - 1].y = (float)blockData;
						debugBoxObj[debugBoxObj.size() - 1]->position.y = blockPos[blockPos.size() - 1].y * 20.0f;
						break;
					case 2:
						blockPos[blockPos.size() - 1].z = (float)blockData;
						debugBoxObj[debugBoxObj.size() - 1]->position.z = blockPos[blockPos.size() - 1].z * 20.0f;
						break;
					default:
						if (number >= BlockType::NONE && number <= BlockType::START)
						{
							blockType[blockType.size() - 1] = blockData;
						}
						break;
					}

					// ŽŸ‚Ì”Žš‚Ö
					number++;
					isMinus = false;
					blockData = 0;
				}
				else if (string[i] == '-')
				{
					isMinus = true;
				}
				else if (string[i] >= '0' && string[i] <= '9')
				{
					blockData *= 10;

					if (isMinus == true)
					{
						blockData -= string[i] - '0';
					}
					else
					{
						blockData += string[i] - '0';
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
		case BlockType::START:
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

		debugBoxObj[i]->position = blockPos[i];
	}
}
