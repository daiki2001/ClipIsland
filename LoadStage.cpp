#include "LoadStage.h"
#include <stdio.h>
#include "TexManager.h"
#include "Raki_DX12B.h"

#define EoF (-1) // Error of function

LoadStage::LoadStage() :
	blocks{},
	blockColors{},
	multipleBlockNumber{},
	moveBlockObj{},
	moveBlockNumber{},
	stayBlockObj{},
	stayBlockNumber{},
	goalBlockObj{},
	goalBlockNumber{},
	switchBlockObj{},
	switchBlockNumber{},
	doorBlockObj{},
	doorBlockNumber{},
	debugBoxObj{},
	debugBoxNumber{},
	warpBlock{},
	openGateCount(0),
	startPosNumber(-1)
{
}

LoadStage::~LoadStage()
{
	for (size_t i = 0; i < moveBlockObj.size(); i++)
	{
		if (moveBlockObj[i] == nullptr)
		{
			continue;
		}

		DeleteObject3d(moveBlockObj[i]);
		moveBlockObj[i] = nullptr;
	}
	for (size_t i = 0; i < stayBlockObj.size(); i++)
	{
		if (stayBlockObj[i] == nullptr)
		{
			continue;
		}

		DeleteObject3d(stayBlockObj[i]);
		stayBlockObj[i] = nullptr;
	}
	for (size_t i = 0; i < goalBlockObj.size(); i++)
	{
		if (goalBlockObj[i] == nullptr)
		{
			continue;
		}

		DeleteObject3d(goalBlockObj[i]);
		goalBlockObj[i] = nullptr;
	}
	for (size_t i = 0; i < switchBlockObj.size(); i++)
	{
		if (switchBlockObj[i] == nullptr)
		{
			continue;
		}

		DeleteObject3d(switchBlockObj[i]);
		switchBlockObj[i] = nullptr;
	}
	for (size_t i = 0; i < doorBlockObj.size(); i++)
	{
		if (doorBlockObj[i] == nullptr)
		{
			continue;
		}

		DeleteObject3d(doorBlockObj[i]);
		doorBlockObj[i] = nullptr;
	}
	for (size_t i = 0; i < warpBlock.size(); i++)
	{
		if (warpBlock[i] == nullptr)
		{
			continue;
		}

		delete warpBlock[i];
		warpBlock[i] = nullptr;
	}
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
	using namespace GameCommonData;
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

				switch (blocks[blocks.size() - 1].type)
				{
				case BlockType::BLOCK:
					moveBlockObj.push_back(CreateObject3d(&StageBlockModels::simpleBlockModel));
					moveBlockNumber.push_back((int)blocks.size() - 1);
					moveBlockObj[moveBlockObj.size() - 1]->position = blocks[blocks.size() - 1].pos;
					break;
				case BlockType::DONT_MOVE_BLOCK:
					stayBlockObj.push_back(CreateObject3d(&StageBlockModels::simpleBlockModel));
					stayBlockNumber.push_back((int)blocks.size() - 1);
					stayBlockObj[stayBlockObj.size() - 1]->position = blocks[blocks.size() - 1].pos;
					break;
				case BlockType::GOAL:
					goalBlockObj.push_back(CreateObject3d(&StageBlockModels::goalModel));
					goalBlockNumber.push_back((int)blocks.size() - 1);
					goalBlockObj[goalBlockObj.size() - 1]->position = blocks[blocks.size() - 1].pos;
					goalBlockObj[goalBlockObj.size() - 1]->rotation = StageBlockModels::modelRot;
					goalBlockObj[goalBlockObj.size() - 1]->scale = ScaleXYZ(StageBlockModels::modelScale);
					break;
				case BlockType::START:
					startPosNumber = (int)blocks.size() - 1;
					break;
				case BlockType::SWITCH:
					switchBlockObj.push_back(CreateObject3d(&StageBlockModels::switchOffModel));
					switchBlockNumber.push_back((int)blocks.size() - 1);
					switchBlockObj[switchBlockObj.size() - 1]->position = blocks[blocks.size() - 1].pos;
					switchBlockObj[switchBlockObj.size() - 1]->rotation = StageBlockModels::modelRot;
					switchBlockObj[switchBlockObj.size() - 1]->scale = ScaleXYZ(StageBlockModels::modelScale);
					break;
				case BlockType::DOOR:
					doorBlockObj.push_back(CreateObject3d(&StageBlockModels::simpleBlockModel));
					doorBlockNumber.push_back((int)blocks.size() - 1);
					doorBlockObj[doorBlockObj.size() - 1]->position = blocks[blocks.size() - 1].pos;
					break;
				case BlockType::WARP_CLOSE_BLOCK:
				case BlockType::WARP_OPEN_BLOCK:
					warpBlock.push_back(new Warp());
					warpBlock[warpBlock.size() - 1]->blockNumber = (int)blocks.size() - 1;
					warpBlock[warpBlock.size() - 1]->CreateObj(blocks[blocks.size() - 1].pos);
					warpBlock[warpBlock.size() - 1]->isOpen = blocks[blocks.size() - 1].type == BlockType::WARP_OPEN_BLOCK;
					break;
				default:
					break;
				}
			}
		}
	}

	size_t moveBlockCount = 0;
	size_t stayBlockCount = 0;
	size_t goalBlockCount = 0;
	size_t switchBlockCount = 0;
	size_t doorBlockCount = 0;
	size_t warpBlockCount = 0;
	size_t debugBlockCount = 0;

	for (size_t i = 0; i < blocks.size(); i++)
	{
		if (blocks[i].type <= BlockType::NONE)
		{
			continue;
		}
		else
		{
			number = blocks[i].type;
		}

		switch (number)
		{
		case BlockType::BLOCK:
			moveBlockObj[moveBlockCount]->color = blockColors[number];
			moveBlockCount++;
			break;
		case BlockType::DONT_MOVE_BLOCK:
			stayBlockObj[stayBlockCount]->color = blockColors[number];
			stayBlockCount++;
			break;
		case BlockType::GOAL:
			goalBlockObj[goalBlockCount]->color = { 1.0f, 1.0f, 1.0f, 1.0f };
			goalBlockCount++;
			break;
		case BlockType::SWITCH:
			switchBlockObj[switchBlockCount]->color = { 1.0f, 1.0f, 1.0f, 1.0f };
			switchBlockCount++;
			break;
		case BlockType::DOOR:
			doorBlockObj[doorBlockCount]->color = blockColors[number];
			doorBlockCount++;
			break;
		case BlockType::WARP_CLOSE_BLOCK:
			warpBlock[warpBlockCount]->SetWarpCloseColor(blockColors[number]);
			warpBlockCount++;
			break;
		case BlockType::WARP_OPEN_BLOCK:
			warpBlock[warpBlockCount]->SetWarpOpenColor(blockColors[number]);
			warpBlockCount++;
			break;
		default:
			break;
		}
	}

	blockSortIsNumber(0, (int)blocks.size() - 1);

	int num = INT_MIN;
	static bool isHit = false;
	for (size_t i = 0; i < blocks.size(); i++)
	{
		if (num == blocks[i].number)
		{
			isHit = false;
			for (size_t j = 0; j < multipleBlockNumber.size(); j++)
			{
				if (num == multipleBlockNumber[j])
				{
					isHit = true;
					break;
				}
			}
			if (isHit == false)
			{
				multipleBlockNumber.push_back(num);
			}
		}
		else
		{
			num = blocks[i].number;
		}
	}

	for (size_t i = 0; i < multipleBlockNumber.size(); i++)
	{
		moveBlockCount = 0;
		for (size_t j = 0; j < blocks.size(); j++)
		{
			if (blocks[j].type == BlockType::BLOCK)
			{
				if (blocks[j].number == multipleBlockNumber[i])
				{
					moveBlockObj[moveBlockCount]->color = blockColors[BlockType::START];
				}
				moveBlockCount++;
			}
		}
	}

	return 0;
}

void LoadStage::Update()
{
	size_t moveBlockCount = 0;
	size_t stayBlockCount = 0;
	size_t goalBlockCount = 0;
	size_t switchBlockCount = 0;
	size_t doorBlockCount = 0;
	size_t warpBlockCount = 0;
	size_t debugBlockCount = 0;

	for (size_t i = 0; i < blocks.size(); i++)
	{
		if (moveBlockCount < moveBlockObj.size() &&
			i == moveBlockNumber[moveBlockCount])
		{
			moveBlockObj[moveBlockCount]->position = blocks[i].pos;
			moveBlockCount++;
		}
		if (stayBlockCount < stayBlockObj.size() &&
			i == stayBlockNumber[stayBlockCount])
		{
			stayBlockObj[stayBlockCount]->position = blocks[i].pos;
			stayBlockCount++;
		}
		if (goalBlockCount < goalBlockObj.size() &&
			i == goalBlockNumber[goalBlockCount])
		{
			goalBlockObj[goalBlockCount]->position = blocks[i].pos;
			goalBlockCount++;
		}
		if (switchBlockCount < switchBlockObj.size() &&
			i == switchBlockNumber[switchBlockCount])
		{
			switchBlockObj[switchBlockCount]->position = blocks[i].pos;
			switchBlockCount++;
		}
		if (doorBlockCount < doorBlockObj.size() &&
			i == doorBlockNumber[doorBlockCount])
		{
			doorBlockObj[doorBlockCount]->position = blocks[i].pos;
			doorBlockCount++;
		}
		if (warpBlockCount < warpBlock.size() &&
			i == warpBlock[warpBlockCount]->blockNumber)
		{
			warpBlock[warpBlockCount]->SetObjectPos(blocks[i].pos);
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
	using namespace GameCommonData::BlockData;

	size_t moveBlockCount = 0;
	size_t stayBlockCount = 0;
	size_t goalBlockCount = 0;
	size_t switchBlockCount = 0;
	size_t doorBlockCount = 0;
	size_t warpBlockCount = 0;
	size_t debugBlockCount = 0;

	for (size_t i = 0; i < blocks.size(); i++)
	{
		switch (blocks[i].type)
		{
		case BlockType::BLOCK:
			DrawObject3d(moveBlockObj[moveBlockCount]);
			moveBlockCount++;
			break;
		case BlockType::DONT_MOVE_BLOCK:
			DrawObject3d(stayBlockObj[stayBlockCount]);
			stayBlockCount++;
			break;
		case BlockType::GOAL:
			DrawObject3d(goalBlockObj[goalBlockCount]);
			goalBlockCount++;
			break;
		case BlockType::SWITCH:
			DrawObject3d(switchBlockObj[switchBlockCount]);
			switchBlockCount++;
			break;
		case BlockType::DOOR:
			DrawObject3d(doorBlockObj[doorBlockCount]);
			doorBlockCount++;
			break;
		case BlockType::WARP_CLOSE_BLOCK:
		case BlockType::WARP_OPEN_BLOCK:
			warpBlock[warpBlockCount]->Draw();
			warpBlockCount++;
			break;
		default:
			break;
		}
	}
}

void LoadStage::Reset()
{
	openGateCount = 0;

	size_t moveBlockCount = 0;
	size_t stayBlockCount = 0;
	size_t goalBlockCount = 0;
	size_t doorBlockCount = 0;
	size_t warpBlockCount = 0;
	size_t debugBlockCount = 0;

	for (size_t i = 0; i < blocks.size(); i++)
	{
		blocks[i].pos = blocks[i].resetPos;
		blocks[i].type = blocks[i].InitType;

		if (moveBlockCount < moveBlockNumber.size() &&
			i == moveBlockNumber[moveBlockCount])
		{
			if (moveBlockObj[moveBlockCount] == nullptr)
			{
				continue;
			}

			moveBlockObj[moveBlockCount]->position = blocks[i].resetPos;
			moveBlockCount++;
		}
		if (stayBlockCount < stayBlockNumber.size() &&
			i == stayBlockNumber[stayBlockCount])
		{
			if (stayBlockObj[stayBlockCount] == nullptr)
			{
				continue;
			}

			stayBlockObj[stayBlockCount]->position = blocks[i].resetPos;
			stayBlockCount++;
		}
		if (goalBlockCount < goalBlockNumber.size() &&
			i == goalBlockNumber[goalBlockCount])
		{
			if (goalBlockObj[goalBlockCount] == nullptr)
			{
				continue;
			}

			goalBlockObj[goalBlockCount]->position = blocks[i].resetPos;
			goalBlockCount++;
		}
		if (doorBlockCount < doorBlockNumber.size() &&
			i == doorBlockNumber[doorBlockCount])
		{
			if (doorBlockObj[doorBlockCount] == nullptr)
			{
				continue;
			}

			doorBlockObj[doorBlockCount]->position = blocks[i].resetPos;
			doorBlockCount++;
		}
		if (warpBlockCount < warpBlock.size() &&
			i == warpBlock[warpBlockCount]->blockNumber)
		{
			warpBlock[warpBlockCount]->CreateObj(blocks[i].resetPos);
			warpBlock[warpBlockCount]->gateNumber=(size_t)-1;
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
	}

	ChangeSwitchModel(&GameCommonData::StageBlockModels::switchOffModel);
}

void LoadStage::StageClear()
{
	for (size_t i = 0; i < moveBlockObj.size(); i++)
	{
		if (moveBlockObj[i] == nullptr)
		{
			continue;
		}

		DeleteObject3d(moveBlockObj[i]);
		moveBlockObj[i] = nullptr;
	}
	for (size_t i = 0; i < stayBlockObj.size(); i++)
	{
		if (stayBlockObj[i] == nullptr)
		{
			continue;
		}

		DeleteObject3d(stayBlockObj[i]);
		stayBlockObj[i] = nullptr;
	}
	for (size_t i = 0; i < goalBlockObj.size(); i++)
	{
		if (goalBlockObj[i] == nullptr)
		{
			continue;
		}

		DeleteObject3d(goalBlockObj[i]);
		goalBlockObj[i] = nullptr;
	}
	for (size_t i = 0; i < switchBlockObj.size(); i++)
	{
		if (switchBlockObj[i] == nullptr)
		{
			continue;
		}

		DeleteObject3d(switchBlockObj[i]);
		switchBlockObj[i] = nullptr;
	}
	for (size_t i = 0; i < doorBlockObj.size(); i++)
	{
		if (doorBlockObj[i] == nullptr)
		{
			continue;
		}

		DeleteObject3d(doorBlockObj[i]);
		doorBlockObj[i] = nullptr;
	}
	for (size_t i = 0; i < warpBlock.size(); i++)
	{
		if (warpBlock[i] == nullptr)
		{
			continue;
		}

		warpBlock[i]->DeleteObject();
		delete warpBlock[i];
		warpBlock[i] = nullptr;
	}
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
	moveBlockObj.clear();
	moveBlockNumber.clear();
	stayBlockObj.clear();
	stayBlockNumber.clear();
	goalBlockObj.clear();
	goalBlockNumber.clear();
	switchBlockObj.clear();
	switchBlockNumber.clear();
	doorBlockObj.clear();
	doorBlockNumber.clear();
	warpBlock.clear();
	debugBoxObj.clear();
	debugBoxNumber.clear();

	blocks.shrink_to_fit();
	blockColors.shrink_to_fit();
	moveBlockObj.shrink_to_fit();
	moveBlockNumber.shrink_to_fit();
	stayBlockObj.shrink_to_fit();
	stayBlockNumber.shrink_to_fit();
	goalBlockObj.shrink_to_fit();
	goalBlockNumber.shrink_to_fit();
	switchBlockObj.shrink_to_fit();
	switchBlockNumber.shrink_to_fit();
	doorBlockObj.shrink_to_fit();
	doorBlockNumber.shrink_to_fit();
	warpBlock.shrink_to_fit();
	debugBoxObj.shrink_to_fit();
	debugBoxNumber.shrink_to_fit();

	startPosNumber = -1;
}

void LoadStage::ChangeSwitchModel(NY_Model3D* modelData)
{
	for (size_t i = 0; i < switchBlockObj.size(); i++)
	{
		if (switchBlockObj[i] == nullptr)
		{
			continue;
		}

		DeleteObject3d(switchBlockObj[i]);
		switchBlockObj[i] = nullptr;

		switchBlockObj[i] = CreateObject3d(modelData);
		switchBlockObj[i]->position = blocks[switchBlockNumber[i]].pos;
		switchBlockObj[i]->rotation = StageBlockModels::modelRot;
		switchBlockObj[i]->scale = GameCommonData::ScaleXYZ(StageBlockModels::modelScale);
		switchBlockObj[i]->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}
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

	return blocks[num].pos;
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

void LoadStage::blockSortIsNumber(int start, int end)
{
	int x = start;

	if (start < end)
	{
		x = PartitionIsBlockNumber(start, end);
		blockSortIsNumber(start, x - 1);
		blockSortIsNumber(x + 1, end);
	}
}

int LoadStage::PartitionIsBlockNumber(int p, int r)
{
	for (size_t i = p; i < r; i++)
	{
		if (blocks[i].number <= blocks[r].number)
		{
			GameCommonData::Swap(blocks[p], blocks[i]);
			p++;
		}
	}

	GameCommonData::Swap(blocks[r], blocks[p]);

	return p;
}
