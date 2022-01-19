#include "Stage.h"

#define EoF (-1) // Error of function

Stage::Stage(Player* player) :
	stage{},
	player(player),
	flag2d(false)
{
}

Stage::~Stage()
{
}

void Stage::Update()
{
	for (size_t i = 0; i < stage.debugBoxObj.size(); i++)
	{
		stage.blocks[i].collision.Update(stage.debugBoxObj[i]->position);
	}
}

void Stage::Draw()
{
	stage.Draw();
}

int Stage::Select(const char* filePath, const bool& flag2d)
{
	if (filePath == nullptr)
	{
		return EoF;
	}

	stage.StageClear();

	while (clipBlock.empty() == false)
	{
		clipBlock.pop();
	}

	this->flag2d = flag2d;

	int result = stage.Load(filePath);

	player->position = stage.GetStartPlayerPos();

	return result;
}

int Stage::Clip(bool flag)
{
	if (flag == false)
	{
		return EoF;
	}

	using namespace BlockData;

	ClipBlock clip = {};

	if (flag2d)
	{
		Clip2d(flag, &clip);
	}
	else
	{
		Clip3d(flag, &clip);
	}

	if (clip.vec1.x != 0)
	{
		clip.vec1.x += blockSize;
		clip.vec2.x -= blockSize;
	}
	if (clip.vec1.y != 0)
	{
		clip.vec1.y += blockSize;
		clip.vec2.y -= blockSize;
	}
	if (clip.vec1.z != 0)
	{
		clip.vec1.z += blockSize;
		clip.vec2.z -= blockSize;
	}

	bool isReturn = clip.blockNumber1 < 0 || clip.blockNumber2 < 0;
	isReturn = isReturn || (clip.vec1.length() == 0.0f && clip.vec2.length() == 0.0f);

	if (isReturn)
	{
		return EoF;
	}

	const size_t blockType[2] = { (size_t)stage.blocks[clip.blockNumber1].type, (size_t)stage.blocks[clip.blockNumber2].type };

	if (moveFlag[blockType[0]].second == true && moveFlag[blockType[1]].second == true)
	{
		clip.playerPos = player->position;
		clipBlock.push(clip);

		stage.debugBoxObj[clipBlock.top().blockNumber1]->position += clipBlock.top().vec1;
		stage.debugBoxObj[clipBlock.top().blockNumber2]->position += clipBlock.top().vec2;
	}

	return 0;
}

int Stage::StepBack()
{
	if (clipBlock.empty())
	{
		return EoF;
	}

	stage.debugBoxObj[clipBlock.top().blockNumber1]->position -= clipBlock.top().vec1;
	stage.debugBoxObj[clipBlock.top().blockNumber2]->position -= clipBlock.top().vec2;
	player->position = clipBlock.top().playerPos;

	clipBlock.pop();

	return 0;
}

void Stage::Reset()
{
	stage.Reset();

	while (clipBlock.empty() == false)
	{
		clipBlock.pop();
	}

	player->position = stage.GetStartPlayerPos();
}

int Stage::Clip2d(bool flag, ClipBlock* clip)
{
	if (clip == nullptr)
	{
		return EoF;
	}

	using namespace BlockData;

	auto& tmp = stage.debugBoxObj;

	if (player->forwardVec.x != 0.0f)
	{
		for (int i = 0; i < tmp.size(); i++)
		{
			if (tmp[i]->position.x != player->position.x || tmp[i]->position.y == player->position.y)
			{
				continue;
			}

			if (stage.blocks[i].type == BlockType::START)
			{
				continue;
			}

			if ((player->position.y - tmp[i]->position.y) < 0.0f)
			{
				if (clip->blockNumber1 == -1)
				{
					clip->blockNumber1 = i;
					clip->vec1 = player->position - tmp[i]->position;
					clip->vec1.z = 0.0f;
				}
				else if ((player->position.y - tmp[i]->position.y) > clip->vec1.y)
				{
					clip->blockNumber1 = i;
					clip->vec1 = player->position - tmp[i]->position;
					clip->vec1.z = 0.0f;
				}
			}
			else
			{
				if (clip->blockNumber2 == -1)
				{
					clip->blockNumber2 = i;
					clip->vec2 = player->position - tmp[i]->position;
					clip->vec2.z = 0.0f;
				}
				else if ((player->position.y - tmp[i]->position.y) < clip->vec2.y)
				{
					clip->blockNumber2 = i;
					clip->vec2 = player->position - tmp[i]->position;
					clip->vec2.z = 0.0f;
				}
			}
		}
	}
	else if (player->forwardVec.y != 0.0f)
	{
		for (int i = 0; i < tmp.size(); i++)
		{
			if (tmp[i]->position.x == player->position.x || tmp[i]->position.y != player->position.y)
			{
				continue;
			}

			if (stage.blocks[i].type == BlockType::START)
			{
				continue;
			}

			if ((player->position.x - tmp[i]->position.x) < 0.0f)
			{
				if (clip->blockNumber1 == -1)
				{
					clip->blockNumber1 = i;
					clip->vec1 = player->position - tmp[i]->position;
					clip->vec1.z = 0.0f;
				}
				else if ((player->position.x - tmp[i]->position.x) > clip->vec1.x)
				{
					clip->blockNumber1 = i;
					clip->vec1 = player->position - tmp[i]->position;
					clip->vec1.z = 0.0f;
				}
			}
			else
			{
				if (clip->blockNumber2 == -1)
				{
					clip->blockNumber2 = i;
					clip->vec2 = player->position - tmp[i]->position;
					clip->vec2.z = 0.0f;
				}
				else if ((player->position.x - tmp[i]->position.x) < clip->vec2.x)
				{
					clip->blockNumber2 = i;
					clip->vec2 = player->position - tmp[i]->position;
					clip->vec2.z = 0.0f;
				}
			}
		}
	}

	return 0;
}

int Stage::Clip3d(bool flag, ClipBlock* clip)
{
	if (clip == nullptr)
	{
		return EoF;
	}

	using namespace BlockData;

	auto& tmp = stage.debugBoxObj;

	if (player->forwardVec.x != 0.0f)
	{
		for (int i = 0; i < (int)tmp.size(); i++)
		{
			if (tmp[i]->position.x != player->position.x || tmp[i]->position.y != player->position.y || tmp[i]->position.z == player->position.z)
			{
				continue;
			}

			if (stage.blocks[i].type == BlockType::START)
			{
				continue;
			}

			if ((player->position.z - tmp[i]->position.z) < 0.0f)
			{
				if (clip->blockNumber1 == -1)
				{
					clip->blockNumber1 = i;
					clip->vec1 = player->position - tmp[i]->position;
				}
				else if ((player->position.z - tmp[i]->position.z) > clip->vec1.z)
				{
					clip->blockNumber1 = i;
					clip->vec1 = player->position - tmp[i]->position;
				}
			}
			else
			{
				if (clip->blockNumber2 == -1)
				{
					clip->blockNumber2 = i;
					clip->vec2 = player->position - tmp[i]->position;
				}
				else if ((player->position.z - tmp[i]->position.z) < clip->vec2.z)
				{
					clip->blockNumber2 = i;
					clip->vec2 = player->position - tmp[i]->position;
				}
			}
		}
	}
	else if (player->forwardVec.z != 0.0f)
	{
		for (int i = 0; i < (int)tmp.size(); i++)
		{
			if (tmp[i]->position.x == player->position.x || tmp[i]->position.y != player->position.y || tmp[i]->position.z != player->position.z)
			{
				continue;
			}

			if (stage.blocks[i].type == BlockType::START)
			{
				continue;
			}

			if ((player->position.x - tmp[i]->position.x) < 0.0f)
			{
				if (clip->blockNumber1 == -1)
				{
					clip->blockNumber1 = i;
					clip->vec1 = player->position - tmp[i]->position;
				}
				else if ((player->position.x - tmp[i]->position.x) > clip->vec1.x)
				{
					clip->blockNumber1 = i;
					clip->vec1 = player->position - tmp[i]->position;
				}
			}
			else
			{
				if (clip->blockNumber2 == -1)
				{
					clip->blockNumber2 = i;
					clip->vec2 = player->position - tmp[i]->position;
				}
				else if ((player->position.x - tmp[i]->position.x) < clip->vec2.x)
				{
					clip->blockNumber2 = i;
					clip->vec2 = player->position - tmp[i]->position;
				}
			}
		}
	}

	return 0;
}
