#include "Stage.h"
#include <vector>

#define EoF (-1) // Error of function

Stage::Stage(Player *player) :
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
	stage.Update();

	for (size_t i = 0; i < stage.blocks.size(); i++)
	{
		stage.blocks[i].collision.Update(stage.blocks[i].pos);
	}
}

void Stage::Draw()
{
	stage.Draw();
}

int Stage::Select(const char *filePath, const bool &flag2d)
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
	using namespace GameCommonData::BlockData;

	ClipBlock clip = {};
	//ClipBlock swi = {};

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
		for (size_t i = 0; i < stage.blocks.size(); i++)
		{
			if (stage.blocks[i].number == clipBlock.top().blockNumber1)
			{
				stage.blocks[i].pos += clipBlock.top().vec1;
			}
			if (stage.blocks[i].number == clipBlock.top().blockNumber2)
			{
				stage.blocks[i].pos += clipBlock.top().vec2;
			}
		}
	}

	return 0;
}

int Stage::StepBack()
{
	if (clipBlock.empty())
	{
		return EoF;
	}

	for (size_t i = 0; i < stage.blocks.size(); i++)
	{

		if (clipBlock.top().isVani == true)
		{
			stage.blocks[i].type = stage.blocks[i].InitType;
			stage.blocks[i].pos.z = clipBlock.top().backPosZ;
		}

		if (clipBlock.top().isVani == false)
		{
			if (stage.blocks[i].number == clipBlock.top().blockNumber1)
			{
				stage.blocks[i].pos -= clipBlock.top().vec1;
			}
			if (stage.blocks[i].number == clipBlock.top().blockNumber2)
			{
				stage.blocks[i].pos -= clipBlock.top().vec2;
			}
		}
	}

	player->position = clipBlock.top().playerPos;

	clipBlock.pop();

	return 0;

}

void Stage::Reset()
{
	{
		stage.Reset();

		while (clipBlock.empty() == false)
		{
			clipBlock.pop();
		}

		for (size_t i = 0; i < stage.blocks.size(); i++)
		{
			stage.blocks[i].type = stage.blocks[i].InitType;
		}

		player->position = stage.GetStartPlayerPos();
	}
}

void Stage::Change()
{
	using namespace GameCommonData::BlockData;

	ClipBlock swi = {};
	bool isFlag = false;

	stage.GetBlocksTypeAll(BlockType::DOOR, DoorChange, 50);
	for (int i = 0; i < 50; i++)
	{
		if (DoorChange[i] < 0)
		{
			continue;
		}
		stage.blocks[DoorChange[i]].type = BlockType::NONE;
		isFlag = true;
	}
	if (isFlag == true)
	{
		swi.isVani = true;
		swi.backPosZ = 0;

		clipBlock.push(swi);
	}
}

void Stage::GetClipBlocksReferencePoint(RVector3 *pos1, RVector3 *pos2)
{
	if (pos1 == nullptr || pos2 == nullptr)
	{
		return;
	}

	if (clipBlock.top().isClip == false)
	{
		return;
	}

	*pos1 = stage.blocks[clipBlock.top().ReferencePoint1].pos;
	*pos2 = stage.blocks[clipBlock.top().ReferencePoint2].pos;
}

void Stage::GetClipBlocksALL(int blocksArray[], const size_t &arraySize)
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

int Stage::Clip2d(ClipBlock *clip)
{
	if (clip == nullptr)
	{
		return EoF;
	}

	using namespace GameCommonData::BlockData;

	auto &tmp = stage.blocks;
	std::vector<float> dontMoveBlocksPos; //プレイヤーと同軸上にある不動ブロックの場所

	// 挟む軸がy軸の時
	if (player->forwardVec.x != 0.0f)
	{
		for (int i = 0; i < tmp.size(); i++)
		{
			if (tmp[i].pos.x != player->position.x || tmp[i].pos.y == player->position.y)
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
			if ((player->position.y - tmp[i].pos.y) < 0.0f)
			{
				if (clip->ReferencePoint1 == -1)
				{
					clip->ReferencePoint1 = i;
					clip->vec1 = player->position - tmp[i].pos;
					clip->vec1.z = 0.0f;
				}
				else if ((player->position.y - tmp[i].pos.y) > clip->vec1.y)
				{
					clip->ReferencePoint1 = i;
					clip->vec1 = player->position - tmp[i].pos;
					clip->vec1.z = 0.0f;
				}
			}
			// ブロックがプレイヤーより-の方向にある時の処理
			else
			{
				if (clip->ReferencePoint2 == -1)
				{
					clip->ReferencePoint2 = i;
					clip->vec2 = player->position - tmp[i].pos;
					clip->vec2.z = 0.0f;
				}
				else if ((player->position.y - tmp[i].pos.y) < clip->vec2.y)
				{
					clip->ReferencePoint2 = i;
					clip->vec2 = player->position - tmp[i].pos;
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
			if (tmp[i].pos.x == player->position.x || tmp[i].pos.y != player->position.y)
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
			if ((player->position.x - tmp[i].pos.x) < 0.0f)
			{
				if (clip->ReferencePoint1 == -1)
				{
					clip->ReferencePoint1 = i;
					clip->vec1 = player->position - tmp[i].pos;
					clip->vec1.z = 0.0f;
				}
				else if ((player->position.x - tmp[i].pos.x) > clip->vec1.x)
				{
					clip->ReferencePoint1 = i;
					clip->vec1 = player->position - tmp[i].pos;
					clip->vec1.z = 0.0f;
				}
			}
			// ブロックがプレイヤーより-の方向にある時の処理
			else
			{
				if (clip->ReferencePoint2 == -1)
				{
					clip->ReferencePoint2 = i;
					clip->vec2 = player->position - tmp[i].pos;
					clip->vec2.z = 0.0f;
				}
				else if ((player->position.x - tmp[i].pos.x) < clip->vec2.x)
				{
					clip->ReferencePoint2 = i;
					clip->vec2 = player->position - tmp[i].pos;
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

		if (stage.blocks[i].type < 0 ||
			(moveFlag[stage.blocks[i].type].second == false && caughtFlag[stage.blocks[i].type].second == false))
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

					if (stage.blocks[k].type < 0 || caughtFlag[stage.blocks[k].type].second == false)
					{
						continue;
					}

					if (tmp[k].pos == tmp[i].pos ||
						tmp[k].pos != tmp[i].pos + keepVec * blockSize * j)
					{
						continue;
					}

					if (clip->vec1.x != 0.0f)
					{
						if (tmp[k].pos.x == tmp[i].pos.x || tmp[k].pos.y != tmp[i].pos.y)
						{
							continue;
						}
						if ((tmp[k].pos.x - tmp[i].pos.x) > clip->vec1.x)
						{
							isEnd = true;
							clip->vec1 = tmp[k].pos - tmp[i].pos;
							break;
						}
					}
					if (clip->vec1.y != 0.0f)
					{
						if (tmp[k].pos.x != tmp[i].pos.x || tmp[k].pos.y == tmp[i].pos.y)
						{
							continue;
						}
						if ((tmp[k].pos.y - tmp[i].pos.y) > clip->vec1.y)
						{
							isEnd = true;
							clip->vec1 = tmp[k].pos - tmp[i].pos;
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

					if (stage.blocks[k].type < 0 || caughtFlag[stage.blocks[k].type].second == false)
					{
						continue;
					}

					if (tmp[k].pos == tmp[i].pos ||
						tmp[k].pos != tmp[i].pos + keepVec * blockSize * j)
					{
						continue;
					}

					if (clip->vec2.x != 0.0f)
					{
						if (tmp[k].pos.x == tmp[i].pos.x || tmp[k].pos.y != tmp[i].pos.y)
						{
							continue;
						}
						if ((tmp[k].pos.x - tmp[i].pos.x) < clip->vec2.x)
						{
							isEnd = true;
							clip->vec2 = tmp[k].pos - tmp[i].pos;
							break;
						}
					}
					if (clip->vec2.y != 0.0f)
					{
						if (tmp[k].pos.x != tmp[i].pos.x || tmp[k].pos.y == tmp[i].pos.y)
						{
							continue;
						}
						if ((tmp[k].pos.y - tmp[i].pos.y) < clip->vec2.y)
						{
							isEnd = true;
							clip->vec2 = tmp[k].pos - tmp[i].pos;
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

int Stage::Clip3d(ClipBlock *clip)
{
	if (clip == nullptr)
	{
		return EoF;
	}

	using namespace GameCommonData::BlockData;

	auto &tmp = stage.blocks;
	std::vector<float> dontMoveBlocksPos; //プレイヤーと同軸上にある不動ブロックの場所

	// 挟む軸がz軸の時(上方向ベクトルはy軸固定)
	if (player->forwardVec.x != 0.0f)
	{
		for (int i = 0; i < (int)tmp.size(); i++)
		{
			if (tmp[i].pos.x != player->position.x || tmp[i].pos.y != player->position.y || tmp[i].pos.z == player->position.z)
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
			if ((player->position.z - tmp[i].pos.z) < 0.0f)
			{
				if (clip->ReferencePoint1 == -1)
				{
					clip->ReferencePoint1 = i;
					clip->vec1 = player->position - tmp[i].pos;
				}
				else if ((player->position.z - tmp[i].pos.z) > clip->vec1.z)
				{
					clip->ReferencePoint1 = i;
					clip->vec1 = player->position - tmp[i].pos;
				}
			}
			// ブロックがプレイヤーより-の方向にある時の処理
			else
			{
				if (clip->ReferencePoint2 == -1)
				{
					clip->ReferencePoint2 = i;
					clip->vec2 = player->position - tmp[i].pos;
				}
				else if ((player->position.z - tmp[i].pos.z) < clip->vec2.z)
				{
					clip->ReferencePoint2 = i;
					clip->vec2 = player->position - tmp[i].pos;
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
			if (tmp[i].pos.x == player->position.x || tmp[i].pos.y != player->position.y || tmp[i].pos.z != player->position.z)
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
			if ((player->position.x - tmp[i].pos.x) < 0.0f)
			{
				if (clip->ReferencePoint1 == -1)
				{
					clip->ReferencePoint1 = i;
					clip->vec1 = player->position - tmp[i].pos;
				}
				else if ((player->position.x - tmp[i].pos.x) > clip->vec1.x)
				{
					clip->ReferencePoint1 = i;
					clip->vec1 = player->position - tmp[i].pos;
				}
			}
			// ブロックがプレイヤーより-の方向にある時の処理
			else
			{
				if (clip->ReferencePoint2 == -1)
				{
					clip->ReferencePoint2 = i;
					clip->vec2 = player->position - tmp[i].pos;
				}
				else if ((player->position.x - tmp[i].pos.x) < clip->vec2.x)
				{
					clip->ReferencePoint2 = i;
					clip->vec2 = player->position - tmp[i].pos;
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

		if (stage.blocks[i].type < 0 ||
			(moveFlag[stage.blocks[i].type].second == false && caughtFlag[stage.blocks[i].type].second == false))
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

			for (size_t j = 1; keepVec * (float)j != clip->vec1; j++)
			{
				for (size_t k = 0; k < stage.blocks.size(); k++)
				{
					if (stage.blocks[k].number == stage.blocks[i].number)
					{
						continue;
					}

					if (stage.blocks[k].type < 0 || caughtFlag[stage.blocks[k].type].second == false)
					{
						continue;
					}

					if (tmp[k].pos == tmp[i].pos ||
						tmp[k].pos != tmp[i].pos + keepVec * blockSize * (float)j)
					{
						continue;
					}

					if (clip->vec1.x != 0.0f)
					{
						if (tmp[k].pos.x == tmp[i].pos.x || tmp[k].pos.y != tmp[i].pos.y || tmp[k].pos.y != tmp[i].pos.y)
						{
							continue;
						}
						if ((tmp[k].pos.x - tmp[i].pos.x) > clip->vec1.x)
						{
							isEnd = true;
							clip->vec1 = tmp[k].pos - tmp[i].pos;
							break;
						}
					}
					if (clip->vec1.z != 0.0f)
					{
						if (tmp[k].pos.x != tmp[i].pos.x || tmp[k].pos.y != tmp[i].pos.y || tmp[k].pos.y == tmp[i].pos.y)
						{
							continue;
						}
						if ((tmp[k].pos.z - tmp[i].pos.z) > clip->vec1.z)
						{
							isEnd = true;
							clip->vec1 = tmp[k].pos - tmp[i].pos;
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

			for (size_t j = 1; keepVec * (float)j != clip->vec2; j++)
			{
				for (size_t k = 0; k < stage.blocks.size(); k++)
				{
					if (stage.blocks[k].number == stage.blocks[i].number)
					{
						continue;
					}

					if (stage.blocks[k].type < 0 || caughtFlag[stage.blocks[k].type].second == false)
					{
						continue;
					}

					if (tmp[k].pos == tmp[i].pos ||
						tmp[k].pos != tmp[i].pos + keepVec * blockSize * (float)j)
					{
						continue;
					}

					if (clip->vec2.x != 0.0f)
					{
						if (tmp[k].pos.x == tmp[i].pos.x || tmp[k].pos.y != tmp[i].pos.y || tmp[k].pos.z != tmp[i].pos.z)
						{
							continue;
						}
						if ((tmp[k].pos.x - tmp[i].pos.x) < clip->vec2.x)
						{
							isEnd = true;
							clip->vec2 = tmp[k].pos - tmp[i].pos;
							break;
						}
					}
					if (clip->vec2.z != 0.0f)
					{
						if (tmp[k].pos.x != tmp[i].pos.x || tmp[k].pos.y == tmp[i].pos.y || tmp[k].pos.z == tmp[i].pos.z)
						{
							continue;
						}
						if ((tmp[k].pos.z - tmp[i].pos.z) < clip->vec2.z)
						{
							isEnd = true;
							clip->vec2 = tmp[k].pos - tmp[i].pos;
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
