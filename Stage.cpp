#include "Stage.h"
#include <vector>

#define EoF (-1) // Error of function

Stage::Stage(Player* player) :
	stage{},
	player(player),
	flag2d(false)
{
}
int DoorChange[50];
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
	using namespace BlockData;

	ClipBlock clip = {};
	int isReturn = 0;

	if (flag2d)
	{
		isReturn = Clip2d(&clip);
	}
	else
	{
		isReturn = Clip3d(&clip);
	}

	if (isReturn == EoF)
	{
		return EoF;
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

	if (clip.vec1.length() == 0.0f && clip.vec2.length() == 0.0f)
	{
		return EoF;
	}

	const size_t blockType[2] = { (size_t)stage.blocks[clip.ReferencePoint1].type, (size_t)stage.blocks[clip.ReferencePoint2].type };

	if (moveFlag[blockType[0]].second == true && moveFlag[blockType[1]].second == true)
	{
		clip.playerPos = player->position;
		clip.isClip = true;

		if (flag)
		{
			clipBlock.push(clip);
		}
	}

	if (flag)
	{
		for (size_t i = 0; i < stage.debugBoxObj.size(); i++)
		{
			if (stage.blocks[i].number == clipBlock.top().blockNumber1)
			{
				stage.debugBoxObj[i]->position += clipBlock.top().vec1;
			}
			if (stage.blocks[i].number == clipBlock.top().blockNumber2)
			{
				stage.debugBoxObj[i]->position += clipBlock.top().vec2;
			}
		}
	}

	return 0;
}

int Stage::StepBack()
{
	{
		if (clipBlock.empty())
		{
			return EoF;
		}

		for (size_t i = 0; i < stage.debugBoxObj.size(); i++)
		{
			if (stage.blocks[i].number == clipBlock.top().blockNumber1)
			{
				stage.debugBoxObj[i]->position -= clipBlock.top().vec1;
			}
			if (stage.blocks[i].number == clipBlock.top().blockNumber2)
			{
				stage.debugBoxObj[i]->position -= clipBlock.top().vec2;
			}

			stage.blocks[i].type = stage.blocks[i].InitType;
		}

		player->position = clipBlock.top().playerPos;

		clipBlock.pop();

		return 0;
	}
}

void Stage::Reset()
{
	{
		stage.Reset();

		while (clipBlock.empty() == false)
		{
			clipBlock.pop();
		}

		for (size_t i = 0; i < stage.debugBoxObj.size(); i++)
		{
			stage.blocks[i].type = stage.blocks[i].InitType;
		}

		player->position = stage.GetStartPlayerPos();
	}
}

void Stage::Change()
{
	stage.GetBlocksTypeAll(BlockData::BlockType::DOOR, DoorChange, 50);
	for (int i = 0; i < 50; i++)
	{
		if (DoorChange[i] < 0)
		{
			continue;
		}
		stage.blocks[DoorChange[i]].type = BlockData::BlockType::NONE;
	}
}

void Stage::GetClipBlocksReferencePoint(RVector3* pos1, RVector3* pos2)
{
	if (pos1 == nullptr || pos2 == nullptr)
	{
		return;
	}

	if (clipBlock.top().isClip == false)
	{
		return;
	}

	*pos1 = stage.debugBoxObj[clipBlock.top().ReferencePoint1]->position;
	*pos2 = stage.debugBoxObj[clipBlock.top().ReferencePoint2]->position;
}

void Stage::GetClipBlocksALL(int blocksArray[], const size_t& arraySize)
{
	if (clipBlock.top().isClip == false)
	{
		return;
	}

	for (size_t i = 0; i < arraySize; i++)
	{
		blocksArray[i] = -1;
	}

	for (size_t i = 0, j = 0; i < stage.blocks.size(); i++)
	{
		if (j >= arraySize)
		{
			break;
		}

		if (stage.blocks[i].number == clipBlock.top().blockNumber1 ||
			stage.blocks[i].number == clipBlock.top().blockNumber2)
		{
			blocksArray[j] = i;
			j++;
		}
	}
}

int Stage::Clip2d(ClipBlock* clip)
{
	if (clip == nullptr)
	{
		return EoF;
	}

	using namespace BlockData;

	auto& tmp = stage.debugBoxObj;
	std::vector<float> dontMoveBlocksPos; //プレイヤーと同軸上にある不動ブロックの場所

	// 挟む軸がy軸の時
	if (player->forwardVec.x != 0.0f)
	{
		for (int i = 0; i < tmp.size(); i++)
		{
			if (tmp[i]->position.x != player->position.x || tmp[i]->position.y == player->position.y)
			{
				// ブロックが同軸上に無い時は無視する
				continue;
			}

			if (stage.blocks[i].type < 0 ||
				(caughtFlag[stage.blocks[i].type].second == false && moveFlag[stage.blocks[i].type].second == false))
			{
				// ブロックの処理が無い場合は無視する
				continue;
			}

			// 不動ブロックの時の処理
			if (moveFlag[stage.blocks[i].type].second == false)
			{
				dontMoveBlocksPos.push_back(stage.blocks[i].pos.y);
				continue;
			}

			// ブロックがプレイヤーより+の方向にある時の処理
			if ((player->position.y - tmp[i]->position.y) < 0.0f)
			{
				if (clip->ReferencePoint1 == -1)
				{
					clip->ReferencePoint1 = i;
					clip->vec1 = player->position - tmp[i]->position;
					clip->vec1.z = 0.0f;
				}
				else if ((player->position.y - tmp[i]->position.y) > clip->vec1.y)
				{
					clip->ReferencePoint1 = i;
					clip->vec1 = player->position - tmp[i]->position;
					clip->vec1.z = 0.0f;
				}
			}
			// ブロックがプレイヤーより-の方向にある時の処理
			else
			{
				if (clip->ReferencePoint2 == -1)
				{
					clip->ReferencePoint2 = i;
					clip->vec2 = player->position - tmp[i]->position;
					clip->vec2.z = 0.0f;
				}
				else if ((player->position.y - tmp[i]->position.y) < clip->vec2.y)
				{
					clip->ReferencePoint2 = i;
					clip->vec2 = player->position - tmp[i]->position;
					clip->vec2.z = 0.0f;
				}
			}
		}

		float space = 0.0f;

		// 引っかかるブロックがあるかどうか
		for (size_t i = 0; i < dontMoveBlocksPos.size(); i++)
		{
			space = player->position.y - dontMoveBlocksPos[i];

			if (space < 0)
			{
				if (space > clip->vec1.y)
				{
					clip->vec1.y -= space;
				}
			}
			else
			{
				if (space < clip->vec2.y)
				{
					clip->vec2.y -= space;
				}
			}
		}
	}
	// 挟む軸がx軸の時
	else if (player->forwardVec.y != 0.0f)
	{
		for (int i = 0; i < tmp.size(); i++)
		{
			if (tmp[i]->position.x == player->position.x || tmp[i]->position.y != player->position.y)
			{
				// ブロックが同軸上に無い時は無視する
				continue;
			}

			if (stage.blocks[i].type < 0 ||
				(caughtFlag[stage.blocks[i].type].second == false && moveFlag[stage.blocks[i].type].second == false))
			{
				// ブロックの処理が無い場合は無視する
				continue;
			}

			// 不動ブロックの時の処理
			if (moveFlag[stage.blocks[i].type].second == false)
			{
				dontMoveBlocksPos.push_back(stage.blocks[i].pos.x);
				continue;
			}

			// ブロックがプレイヤーより+の方向にある時の処理
			if ((player->position.x - tmp[i]->position.x) < 0.0f)
			{
				if (clip->ReferencePoint1 == -1)
				{
					clip->ReferencePoint1 = i;
					clip->vec1 = player->position - tmp[i]->position;
					clip->vec1.z = 0.0f;
				}
				else if ((player->position.x - tmp[i]->position.x) > clip->vec1.x)
				{
					clip->ReferencePoint1 = i;
					clip->vec1 = player->position - tmp[i]->position;
					clip->vec1.z = 0.0f;
				}
			}
			// ブロックがプレイヤーより-の方向にある時の処理
			else
			{
				if (clip->ReferencePoint2 == -1)
				{
					clip->ReferencePoint2 = i;
					clip->vec2 = player->position - tmp[i]->position;
					clip->vec2.z = 0.0f;
				}
				else if ((player->position.x - tmp[i]->position.x) < clip->vec2.x)
				{
					clip->ReferencePoint2 = i;
					clip->vec2 = player->position - tmp[i]->position;
					clip->vec2.z = 0.0f;
				}
			}
		}

		float space = 0.0f;

		// 引っかかるブロックがあるかどうか
		for (size_t i = 0; i < dontMoveBlocksPos.size(); i++)
		{
			space = player->position.x - dontMoveBlocksPos[i];

			if (space < 0)
			{
				if (space > clip->vec1.x)
				{
					clip->vec1.x -= space;
				}
			}
			else
			{
				if (space < clip->vec2.x)
				{
					clip->vec2.x -= space;
				}
			}
		}
	}

	if (clip->ReferencePoint1 < 0 || clip->ReferencePoint2 < 0)
	{
		// 挟むブロックが無ければリターン
		return EoF;
	}

	if (stage.blocks[clip->ReferencePoint1].number == stage.blocks[clip->ReferencePoint2].number)
	{
		// 同じ塊の中のブロックの場合リターン
		return EoF;
	}

	clip->blockNumber1 = stage.blocks[clip->ReferencePoint1].number;
	clip->blockNumber2 = stage.blocks[clip->ReferencePoint2].number;

	RVector3 keepVec = RVector3();
	bool isEnd = false;

	for (size_t i = 0; i < stage.blocks.size(); i++)
	{
		if (i == clip->ReferencePoint1 || i == clip->ReferencePoint2)
		{
			continue;
		}

		if (stage.blocks[i].type == BlockType::START)
		{
			continue;
		}

		if (stage.blocks[i].number == clip->blockNumber1)
		{
			if (clip->vec1.length() == 0.0f)
			{
				continue;
			}

			keepVec = clip->vec1;
			keepVec = keepVec.norm();

			for (size_t j = 1; keepVec * j != clip->vec1; j++)
			{
				for (size_t k = 0; k < stage.blocks.size(); k++)
				{
					if (stage.blocks[k].number == stage.blocks[i].number)
					{
						continue;
					}

					if (stage.blocks[i].type == BlockType::START)
					{
						continue;
					}

					if (tmp[k]->position == tmp[i]->position ||
						tmp[k]->position != tmp[i]->position + keepVec * blockSize * j)
					{
						continue;
					}

					if (clip->vec1.x != 0.0f)
					{
						if (tmp[k]->position.x == tmp[i]->position.x || tmp[k]->position.y != tmp[i]->position.y)
						{
							continue;
						}
						if ((tmp[k]->position.x - tmp[i]->position.x) > clip->vec1.x)
						{
							isEnd = true;
							clip->vec1 = tmp[k]->position - tmp[i]->position;
							break;
						}
					}
					if (clip->vec1.y != 0.0f)
					{
						if (tmp[k]->position.x != tmp[i]->position.x || tmp[k]->position.y == tmp[i]->position.y)
						{
							continue;
						}
						if ((tmp[k]->position.y - tmp[i]->position.y) > clip->vec1.y)
						{
							isEnd = true;
							clip->vec1 = tmp[k]->position - tmp[i]->position;
							break;
						}
					}
				}
				if (isEnd)
				{
					isEnd = false;
					break;
				}
			}
		}
		if (stage.blocks[i].number == clip->blockNumber2)
		{
			if (clip->vec2.length() == 0.0f)
			{
				continue;
			}

			keepVec = clip->vec2;
			keepVec = keepVec.norm();

			for (size_t j = 1; keepVec * j != clip->vec2; j++)
			{
				for (size_t k = 0; k < stage.blocks.size(); k++)
				{
					if (stage.blocks[k].number == stage.blocks[i].number)
					{
						continue;
					}

					if (stage.blocks[k].type == BlockType::START)
					{
						continue;
					}

					if (tmp[k]->position == tmp[i]->position ||
						tmp[k]->position != tmp[i]->position + keepVec * blockSize * j)
					{
						continue;
					}

					if (clip->vec2.x != 0.0f)
					{
						if (tmp[k]->position.x == tmp[i]->position.x || tmp[k]->position.y != tmp[i]->position.y)
						{
							continue;
						}
						if ((tmp[k]->position.x - tmp[i]->position.x) < clip->vec2.x)
						{
							isEnd = true;
							clip->vec2 = tmp[k]->position - tmp[i]->position;
							break;
						}
					}
					if (clip->vec2.y != 0.0f)
					{
						if (tmp[k]->position.x != tmp[i]->position.x || tmp[k]->position.y == tmp[i]->position.y)
						{
							continue;
						}
						if ((tmp[k]->position.y - tmp[i]->position.y) < clip->vec2.y)
						{
							isEnd = true;
							clip->vec2 = tmp[k]->position - tmp[i]->position;
							break;
						}
					}
				}
				if (isEnd)
				{
					isEnd = false;
					break;
				}
			}
		}
	}

	return 0;
}

int Stage::Clip3d(ClipBlock* clip)
{
	if (clip == nullptr)
	{
		return EoF;
	}

	using namespace BlockData;

	auto& tmp = stage.debugBoxObj;
	std::vector<float> dontMoveBlocksPos; //プレイヤーと同軸上にある不動ブロックの場所

	// 挟む軸がz軸の時(上方向ベクトルはy軸固定)
	if (player->forwardVec.x != 0.0f)
	{
		for (int i = 0; i < (int)tmp.size(); i++)
		{
			if (tmp[i]->position.x != player->position.x || tmp[i]->position.y != player->position.y || tmp[i]->position.z == player->position.z)
			{
				// ブロックが同軸上に無い時は無視する
				continue;
			}

			if (stage.blocks[i].type < 0 ||
				(caughtFlag[stage.blocks[i].type].second == false && moveFlag[stage.blocks[i].type].second == false))
			{
				// ブロックの処理が無い場合は無視する
				continue;
			}

			// 不動ブロックの時の処理
			if (moveFlag[stage.blocks[i].type].second == false)
			{
				dontMoveBlocksPos.push_back(stage.blocks[i].pos.x);
				continue;
			}

			// ブロックがプレイヤーより+の方向にある時の処理
			if ((player->position.z - tmp[i]->position.z) < 0.0f)
			{
				if (clip->ReferencePoint1 == -1)
				{
					clip->ReferencePoint1 = i;
					clip->vec1 = player->position - tmp[i]->position;
				}
				else if ((player->position.z - tmp[i]->position.z) > clip->vec1.z)
				{
					clip->ReferencePoint1 = i;
					clip->vec1 = player->position - tmp[i]->position;
				}
			}
			// ブロックがプレイヤーより-の方向にある時の処理
			else
			{
				if (clip->ReferencePoint2 == -1)
				{
					clip->ReferencePoint2 = i;
					clip->vec2 = player->position - tmp[i]->position;
				}
				else if ((player->position.z - tmp[i]->position.z) < clip->vec2.z)
				{
					clip->ReferencePoint2 = i;
					clip->vec2 = player->position - tmp[i]->position;
				}
			}
		}

		float space = 0.0f;

		// 引っかかるブロックがあるかどうか
		for (size_t i = 0; i < dontMoveBlocksPos.size(); i++)
		{
			space = player->position.z - dontMoveBlocksPos[i];

			if (space < 0)
			{
				if (space > clip->vec1.z)
				{
					clip->vec1.z -= space;
				}
			}
			else
			{
				if (space < clip->vec2.z)
				{
					clip->vec2.z -= space;
				}
			}
		}
	}
	// 挟む軸がx軸の時(上方向ベクトルはy軸固定)
	else if (player->forwardVec.z != 0.0f)
	{
		for (int i = 0; i < (int)tmp.size(); i++)
		{
			if (tmp[i]->position.x == player->position.x || tmp[i]->position.y != player->position.y || tmp[i]->position.z != player->position.z)
			{
				// ブロックが同軸上に無い時は無視する
				continue;
			}

			if (stage.blocks[i].type < 0 ||
				(caughtFlag[stage.blocks[i].type].second == false && moveFlag[stage.blocks[i].type].second == false))
			{
				// ブロックの処理が無い場合は無視する
				continue;
			}

			// 不動ブロックの時の処理
			if (moveFlag[stage.blocks[i].type].second == false)
			{
				dontMoveBlocksPos.push_back(stage.blocks[i].pos.x);
				continue;
			}

			// ブロックがプレイヤーより+の方向にある時の処理
			if ((player->position.x - tmp[i]->position.x) < 0.0f)
			{
				if (clip->ReferencePoint1 == -1)
				{
					clip->ReferencePoint1 = i;
					clip->vec1 = player->position - tmp[i]->position;
				}
				else if ((player->position.x - tmp[i]->position.x) > clip->vec1.x)
				{
					clip->ReferencePoint1 = i;
					clip->vec1 = player->position - tmp[i]->position;
				}
			}
			// ブロックがプレイヤーより-の方向にある時の処理
			else
			{
				if (clip->ReferencePoint2 == -1)
				{
					clip->ReferencePoint2 = i;
					clip->vec2 = player->position - tmp[i]->position;
				}
				else if ((player->position.x - tmp[i]->position.x) < clip->vec2.x)
				{
					clip->ReferencePoint2 = i;
					clip->vec2 = player->position - tmp[i]->position;
				}
			}
		}

		float space = 0.0f;

		// 引っかかるブロックがあるかどうか
		for (size_t i = 0; i < dontMoveBlocksPos.size(); i++)
		{
			space = player->position.x - dontMoveBlocksPos[i];

			if (space < 0)
			{
				if (space > clip->vec1.x)
				{
					clip->vec1.x -= space;
				}
			}
			else
			{
				if (space < clip->vec2.x)
				{
					clip->vec2.x -= space;
				}
			}
		}
	}

	if (clip->ReferencePoint1 < 0 || clip->ReferencePoint2 < 0)
	{
		// 挟むブロックが無ければリターン
		return EoF;
	}

	if (stage.blocks[clip->ReferencePoint1].number == stage.blocks[clip->ReferencePoint2].number)
	{
		// 同じ塊の中のブロックの場合リターン
		return EoF;
	}

	clip->blockNumber1 = stage.blocks[clip->ReferencePoint1].number;
	clip->blockNumber2 = stage.blocks[clip->ReferencePoint2].number;

	RVector3 keepVec = RVector3();
	bool isEnd = false;

	for (size_t i = 0; i < stage.blocks.size(); i++)
	{
		if (i == clip->ReferencePoint1 || i == clip->ReferencePoint2)
		{
			continue;
		}

		if (stage.blocks[i].type == BlockType::START)
		{
			continue;
		}

		if (stage.blocks[i].number == clip->blockNumber1)
		{
			if (clip->vec1.length() == 0.0f)
			{
				continue;
			}

			keepVec = clip->vec1;
			keepVec = keepVec.norm();

			for (size_t j = 1; keepVec * j != clip->vec1; j++)
			{
				for (size_t k = 0; k < stage.blocks.size(); k++)
				{
					if (stage.blocks[k].number == stage.blocks[i].number)
					{
						continue;
					}

					if (stage.blocks[i].type == BlockType::START)
					{
						continue;
					}

					if (tmp[k]->position == tmp[i]->position ||
						tmp[k]->position != tmp[i]->position + keepVec * blockSize * j)
					{
						continue;
					}

					if (clip->vec1.x != 0.0f)
					{
						if (tmp[k]->position.x == tmp[i]->position.x || tmp[k]->position.y != tmp[i]->position.y || tmp[k]->position.y != tmp[i]->position.y)
						{
							continue;
						}
						if ((tmp[k]->position.x - tmp[i]->position.x) > clip->vec1.x)
						{
							isEnd = true;
							clip->vec1 = tmp[k]->position - tmp[i]->position;
							break;
						}
					}
					if (clip->vec1.z != 0.0f)
					{
						if (tmp[k]->position.x != tmp[i]->position.x || tmp[k]->position.y != tmp[i]->position.y || tmp[k]->position.y == tmp[i]->position.y)
						{
							continue;
						}
						if ((tmp[k]->position.z - tmp[i]->position.z) > clip->vec1.z)
						{
							isEnd = true;
							clip->vec1 = tmp[k]->position - tmp[i]->position;
							break;
						}
					}
				}
				if (isEnd)
				{
					isEnd = false;
					break;
				}
			}
		}
		if (stage.blocks[i].number == clip->blockNumber2)
		{
			if (clip->vec2.length() == 0.0f)
			{
				continue;
			}

			keepVec = clip->vec2;
			keepVec = keepVec.norm();

			for (size_t j = 1; keepVec * j != clip->vec2; j++)
			{
				for (size_t k = 0; k < stage.blocks.size(); k++)
				{
					if (stage.blocks[k].number == stage.blocks[i].number)
					{
						continue;
					}

					if (stage.blocks[k].type == BlockType::START)
					{
						continue;
					}

					if (tmp[k]->position == tmp[i]->position ||
						tmp[k]->position != tmp[i]->position + keepVec * blockSize * j)
					{
						continue;
					}

					if (clip->vec2.x != 0.0f)
					{
						if (tmp[k]->position.x == tmp[i]->position.x || tmp[k]->position.y != tmp[i]->position.y || tmp[k]->position.z != tmp[i]->position.z)
						{
							continue;
						}
						if ((tmp[k]->position.x - tmp[i]->position.x) < clip->vec2.x)
						{
							isEnd = true;
							clip->vec2 = tmp[k]->position - tmp[i]->position;
							break;
						}
					}
					if (clip->vec2.z != 0.0f)
					{
						if (tmp[k]->position.x != tmp[i]->position.x || tmp[k]->position.y == tmp[i]->position.y || tmp[k]->position.z == tmp[i]->position.z)
						{
							continue;
						}
						if ((tmp[k]->position.z - tmp[i]->position.z) < clip->vec2.z)
						{
							isEnd = true;
							clip->vec2 = tmp[k]->position - tmp[i]->position;
							break;
						}
					}
				}
				if (isEnd)
				{
					isEnd = false;
					break;
				}
			}
		}
	}

	return 0;
}
