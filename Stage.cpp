#include "Stage.h"
#include <vector>
#include "StageMoveParticle.h"

#define EoF (-1) // Error of function

const size_t Stage::maxFlame = 15;

Stage::Stage(Player *player) :
	stage{},
	player(player),
	flag2d(false),
	easeNumber{},
	easeStartPos{},
	nowFlame(0),
	isEasing(false)
{
	clipSE=Audio::LoadSound_wav("./Resources/Sound/sand.wav");
	bottunSE = Audio::LoadSound_wav("./Resources/Sound/bottunSE.wav");
	blockSE = Audio::LoadSound_wav("./Resources/Sound/blockSE.wav");
	stepBackSE = Audio::LoadSound_wav("./Resources/Sound/RandB.wav");
	resetSE = Audio::LoadSound_wav("./Resources/Sound/RandB.wav");
	backFlag = false;
	clipFlag = false;
}

Stage::~Stage()
{
}

void Stage::Update()
{
	stage.Update();

	for (size_t i = 0; i < stage.blocks.size(); i++)
	{
		stage.blocks[i].collision.Update(stage.blocks[i].pos);

		for (size_t j = 0, k = 0; j < stage.warpBlock.size(); j++)
		{
			if (stage.warpBlock[j]->gateNumber == size_t(-1))
			{
				continue;
			}

			if (stage.warpBlock[j]->isOpen == false)
			{
				continue;
			}

			if (stage.warpBlock[j]->GetObjectPos() == stage.blocks[i].pos)
			{
				stage.warpBlock[j]->blockNumber = (int)i;

				if (k % 2 == 1)
				{
					stage.warpBlock[k - 1]->twinBlockNumber = stage.warpBlock[k]->blockNumber;
					stage.warpBlock[k]->twinBlockNumber = stage.warpBlock[k - 1]->blockNumber;
				}
			}

			k++;
		}
	}
		if (isEasing)
		{
			clipFlag = true;
			player->easeFlag = true;

			float rate = (float)nowFlame / maxFlame;

			if (rate > 1.0f)
			{
				isEasing = false;
				clipFlag = false;

				easeNumber.clear();
				easeStartPos.clear();

				easeNumber.shrink_to_fit();
				easeStartPos.shrink_to_fit();

				player->easeFlag = false;
			}

			for (size_t i = 0; i < easeNumber.size(); i++)
			{
				if (stage.blocks[easeNumber[i]].number == clipBlock.top().blockNumber1)
				{
					if (clipBlock.top().vec1[0] == RVector3(0.0f, 0.0f, 0.0f))
					{
						continue;
					}
					stage.blocks[easeNumber[i]].pos = Rv3Ease::OutQuad(
						easeStartPos[i],
						easeStartPos[i] + clipBlock.top().vec1[0],
						rate);
				}
				else if (stage.blocks[easeNumber[i]].number == clipBlock.top().blockNumber2)
				{
					if (clipBlock.top().vec2[0] == RVector3(0.0f, 0.0f, 0.0f))
					{
						continue;
					}
					stage.blocks[easeNumber[i]].pos = Rv3Ease::OutQuad(
						easeStartPos[i],
						easeStartPos[i] + clipBlock.top().vec2[0],
						rate);
				}
			}
			nowFlame++;
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
	player->startPos = stage.GetStartPlayerPos();
	player->endPos = stage.GetStartPlayerPos();

	return result;
}

int Stage::Clip(bool flag)
{
	using namespace GameCommonData::BlockData;

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

	if (clip.vec1[0].x != 0)
	{
		clip.vec1[0].x += blockSize;
		clip.vec2[0].x -= blockSize;
	}
	if (clip.vec1[0].y != 0)
	{
		clip.vec1[0].y += blockSize;
		clip.vec2[0].y -= blockSize;
	}
	if (clip.vec1[0].z != 0)
	{
		clip.vec1[0].z += blockSize;
		clip.vec2[0].z -= blockSize;
	}

	for (size_t i = 0; i < clip.vec1.size(); i++)
	{
		if (clip.vec1[i].length() == 0.0f && clip.vec2[i].length() == 0.0f &&
			clip.gateNumber1.size() <= 0 && clip.gateNumber2.size() <= 0)
		{
			return EoF;
		}
	}

	const size_t blockType[2] = { (size_t)stage.blocks[clip.ReferencePoint1].type, (size_t)stage.blocks[clip.ReferencePoint2].type };

	if (moveFlag[blockType[0]].second == true && moveFlag[blockType[1]].second == true)
	{
		clip.playerPos = player->position;
		clip.playerStartPos = player->startPos;
		clip.playerEndPos = player->endPos;
		clip.isClip = true;

		if (flag)
		{
			Audio::PlayLoadedSound(clipSE);
			Audio::PlayLoadedSound(blockSE);
			clipBlock.push(clip);
		}
	}

	if (flag)
	{
		nowFlame = 0;
		isEasing = true;

		for (size_t i = 0; i < stage.blocks.size(); i++)
		{
			if (stage.blocks[i].number == clipBlock.top().blockNumber1 ||
				stage.blocks[i].number == clipBlock.top().blockNumber2)
			{
				easeNumber.push_back(i);
				easeStartPos.push_back(stage.blocks[i].pos);
			}
		}
	}

	if (clip.isClip)
	{
		for (size_t i = 0; i < stage.blocks.size(); i++)
		{
			if (stage.blocks[i].number != clip.blockNumber1 &&
				stage.blocks[i].number != clip.blockNumber2)
			{
				continue;
			}

			StageMoveParticle::Get()->SpawnMoveStandbyParticle(
				stage.blocks[i].pos,
				RVector3(+blockSize / 6.0f, +blockSize / 6.0f, +blockSize / 6.0f),
				RVector3(-blockSize / 6.0f, -blockSize / 6.0f, -blockSize / 6.0f)
			);
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

		player->stepbackFlag = true;

		for (size_t i = 0; i < stage.blocks.size(); i++)
		{
			if (clipBlock.top().isVani == true)
			{
				stage.blocks[i].type = stage.blocks[i].InitType;
				stage.blocks[i].pos.z = clipBlock.top().backPosZ;
				player->position = clipBlock.top().vaniPos;
				// Tpositionも追加しないとバグる
				player->Tposition = clipBlock.top().vaniPos;
				stage.ChangeSwitchModel(&GameCommonData::StageBlockModels::switchOffModel);
			}

			if (clipBlock.top().isVani == false)
			{
				for (size_t j = 0; j < clipBlock.top().vec1.size(); j++)
				{
					if (stage.blocks[i].number == clipBlock.top().blockNumber1)
					{
						stage.blocks[i].pos -= clipBlock.top().vec1[j];
					}
					if (stage.blocks[i].number == clipBlock.top().blockNumber2)
					{
						stage.blocks[i].pos -= clipBlock.top().vec2[j];
					}
				}
				player->endPos = clipBlock.top().playerEndPos;
				player->position = clipBlock.top().playerPos;
				player->Tposition = clipBlock.top().playerPos;//いらないかも
			}
		}
		clipBlock.pop();

		player->stepbackFlag = false;

		Audio::PlayLoadedSound(stepBackSE);
		backFlag = false;
	//}
	return 0;
}

void Stage::Reset()
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
	player->startPos = stage.GetStartPlayerPos();
	player->endPos = stage.GetStartPlayerPos();
	player->Tposition = stage.GetStartPlayerPos();

	Audio::PlayLoadedSound(resetSE);
	
}

void Stage::Change()
{
	using namespace GameCommonData::BlockData;

	int DoorChange[50];
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
		swi.vaniPos = player->startPos;
		swi.playerPos = player->position;
		swi.playerStartPos = player->startPos;
		swi.playerEndPos = player->endPos;

		Audio::PlayLoadedSound(bottunSE);

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

bool Stage::GetClipBlocksALL(int blocksArray[], const size_t &arraySize)
{
	if (clipBlock.empty())
	{
		return false;
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
			blocksArray[j] = (int)i;
			j++;
		}
	}

	return clipBlock.top().isClip;
}

int Stage::Clip2d(ClipBlock *clip)
{
	if (clip == nullptr)
	{
		return EoF;
	}

	using namespace GameCommonData::BlockData;

	auto& tmp = stage.blocks;
	bool isWarp = false;
	std::vector<RVector3> dontMoveBlocksPos; //プレイヤーと同軸上にある不動ブロックの場所
	std::vector<int> warpBlockNumber;

	clip->vec1.push_back(RVector3());
	clip->vec2.push_back(RVector3());

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

			if (tmp[i].type == BlockType::WARP_OPEN_BLOCK)
			{
				warpBlockNumber.push_back(i);
				isWarp = true;
				continue;
			}

			if (tmp[i].type < 0 ||
				(caughtFlag[stage.blocks[i].type].second == false && moveFlag[stage.blocks[i].type].second == false))
			{
				// ブロックの処理が無い場合は無視する
				continue;
			}

			// 不動ブロックの時の処理
			if (moveFlag[stage.blocks[i].type].second == false)
			{
				dontMoveBlocksPos.push_back(stage.blocks[i].pos);
				continue;
			}

			// ブロックがプレイヤーより+の方向にある時の処理
			if ((player->position.y - tmp[i].pos.y) < 0.0f)
			{
				if (clip->ReferencePoint1 == -1)
				{
					clip->ReferencePoint1 = i;
					clip->vec1[clip->vec1.size() - 1] = player->position - tmp[i].pos;
					clip->vec1[clip->vec1.size() - 1].z = 0.0f;
				}
				else if ((player->position.y - tmp[i].pos.y) > clip->vec1[clip->vec1.size() - 1].y)
				{
					clip->ReferencePoint1 = i;
					clip->vec1[clip->vec1.size() - 1] = player->position - tmp[i].pos;
					clip->vec1[clip->vec1.size() - 1].z = 0.0f;
				}
			}
			// ブロックがプレイヤーより-の方向にある時の処理
			else
			{
				if (clip->ReferencePoint2 == -1)
				{
					clip->ReferencePoint2 = i;
					clip->vec2[clip->vec2.size() - 1] = player->position - tmp[i].pos;
					clip->vec2[clip->vec2.size() - 1].z = 0.0f;
				}
				else if ((player->position.y - tmp[i].pos.y) < clip->vec2[clip->vec2.size() - 1].y)
				{
					clip->ReferencePoint2 = i;
					clip->vec2[clip->vec2.size() - 1] = player->position - tmp[i].pos;
					clip->vec2[clip->vec2.size() - 1].z = 0.0f;
				}
			}
		}

		RVector3 space = RVector3();

		for (size_t i = 0; i < warpBlockNumber.size(); i++)
		{
			// ブロックがプレイヤーより+の方向にある時の処理
			if ((player->position.y - tmp[warpBlockNumber[i]].pos.y) < 0.0f)
			{
				if (clip->ReferencePoint1 == -1)
				{
					clip->gateNumber1.push_back((int)i);
					clip->vec1[clip->vec1.size() - 1] = player->position - tmp[warpBlockNumber[i]].pos;
					clip->vec1[clip->vec1.size() - 1].z = 0.0f;
				}
				else if ((player->position.y - tmp[warpBlockNumber[i]].pos.y) > clip->vec1[clip->vec1.size() - 1].y)
				{
					clip->ReferencePoint1 = -1;
					clip->gateNumber1.push_back((int)i);
					clip->vec1[clip->vec1.size() - 1] = player->position - tmp[warpBlockNumber[i]].pos;
					clip->vec1[clip->vec1.size() - 1].z = 0.0f;
				}
			}
			// ブロックがプレイヤーより-の方向にある時の処理
			else
			{
				if (clip->ReferencePoint2 == -1)
				{
					clip->gateNumber2.push_back((int)i);
					clip->vec2[clip->vec2.size() - 1] = player->position - tmp[warpBlockNumber[i]].pos;
					clip->vec2[clip->vec2.size() - 1].z = 0.0f;
				}
				else if ((player->position.y - tmp[warpBlockNumber[i]].pos.y) < clip->vec2[clip->vec2.size() - 1].y)
				{
					clip->ReferencePoint2 = -1;
					clip->gateNumber2.push_back((int)i);
					clip->vec2[clip->vec2.size() - 1] = player->position - tmp[warpBlockNumber[i]].pos;
					clip->vec2[clip->vec2.size() - 1].z = 0.0f;
				}
			}
		}

		// 引っかかるブロックがあるかどうか
		for (size_t i = 0; i < dontMoveBlocksPos.size(); i++)
		{
			space = player->position - dontMoveBlocksPos[i];

			if (space.y < 0)
			{
				if (space.y > clip->vec1[clip->vec1.size() - 1].y)
				{
					clip->vec1[clip->vec1.size() - 1] -= space;
				}
			}
			else
			{
				if (space.y < clip->vec2[clip->vec2.size() - 1].y)
				{
					clip->vec2[clip->vec2.size() - 1] -= space;
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

			if (tmp[i].type == BlockType::WARP_OPEN_BLOCK)
			{
				warpBlockNumber.push_back(i);
				isWarp = true;
				continue;
			}

			if (tmp[i].type < 0 ||
				(caughtFlag[stage.blocks[i].type].second == false && moveFlag[stage.blocks[i].type].second == false))
			{
				// ブロックの処理が無い場合は無視する
				continue;
			}

			// 不動ブロックの時の処理
			if (moveFlag[stage.blocks[i].type].second == false)
			{
				dontMoveBlocksPos.push_back(stage.blocks[i].pos);
				continue;
			}

			// ブロックがプレイヤーより+の方向にある時の処理
			if ((player->position.x - tmp[i].pos.x) < 0.0f)
			{
				if (clip->ReferencePoint1 == -1)
				{
					clip->ReferencePoint1 = i;
					clip->vec1[clip->vec1.size() - 1] = player->position - tmp[i].pos;
					clip->vec1[clip->vec1.size() - 1].z = 0.0f;
				}
				else if ((player->position.x - tmp[i].pos.x) > clip->vec1[clip->vec1.size() - 1].x)
				{
					clip->ReferencePoint1 = i;
					clip->vec1[clip->vec1.size() - 1] = player->position - tmp[i].pos;
					clip->vec1[clip->vec1.size() - 1].z = 0.0f;
				}
			}
			// ブロックがプレイヤーより-の方向にある時の処理
			else
			{
				if (clip->ReferencePoint2 == -1)
				{
					clip->ReferencePoint2 = i;
					clip->vec2[clip->vec2.size() - 1] = player->position - tmp[i].pos;
					clip->vec2[clip->vec2.size() - 1].z = 0.0f;
				}
				else if ((player->position.x - tmp[i].pos.x) < clip->vec2[clip->vec2.size() - 1].x)
				{
					clip->ReferencePoint2 = i;
					clip->vec2[clip->vec2.size() - 1] = player->position - tmp[i].pos;
					clip->vec2[clip->vec2.size() - 1].z = 0.0f;
				}
			}
		}

		RVector3 space = RVector3();

		for (size_t i = 0; i < warpBlockNumber.size(); i++)
		{
			// ブロックがプレイヤーより+の方向にある時の処理
			if ((player->position.x - tmp[warpBlockNumber[i]].pos.x) < 0.0f)
			{
				if (clip->ReferencePoint1 == -1)
				{
					clip->gateNumber1.push_back((int)i);
					clip->vec1[clip->vec1.size() - 1] = player->position - tmp[warpBlockNumber[i]].pos;
					clip->vec1[clip->vec1.size() - 1].z = 0.0f;
				}
				else if ((player->position.x - tmp[warpBlockNumber[i]].pos.x) > clip->vec1[clip->vec1.size() - 1].x)
				{
					clip->ReferencePoint1 = -1;
					clip->gateNumber1.push_back((int)i);
					clip->vec1[clip->vec1.size() - 1] = player->position - tmp[warpBlockNumber[i]].pos;
					clip->vec1[clip->vec1.size() - 1].z = 0.0f;
				}
			}
			// ブロックがプレイヤーより-の方向にある時の処理
			else
			{
				if (clip->ReferencePoint2 == -1)
				{
					clip->gateNumber2.push_back((int)i);
					clip->vec2[clip->vec2.size() - 1] = player->position - tmp[warpBlockNumber[i]].pos;
					clip->vec2[clip->vec2.size() - 1].z = 0.0f;
				}
				else if ((player->position.x - tmp[warpBlockNumber[i]].pos.x) < clip->vec2[clip->vec2.size() - 1].x)
				{
					clip->ReferencePoint2 = -1;
					clip->gateNumber2.push_back((int)i);
					clip->vec2[clip->vec2.size() - 1] = player->position - tmp[warpBlockNumber[i]].pos;
					clip->vec2[clip->vec2.size() - 1].z = 0.0f;
				}
			}
		}

		// 引っかかるブロックがあるかどうか
		for (size_t i = 0; i < dontMoveBlocksPos.size(); i++)
		{
			space = player->position - dontMoveBlocksPos[i];

			if (space.x < 0)
			{
				if (space.x > clip->vec1[clip->vec1.size() - 1].x)
				{
					clip->vec1[clip->vec1.size() - 1] -= space;
				}
			}
			else
			{
				if (space.x < clip->vec2[clip->vec2.size() - 1].x)
				{
					clip->vec2[clip->vec2.size() - 1] -= space;
				}
			}
		}
	}

	while (isWarp)
	{
		isWarp = false;
		dontMoveBlocksPos.clear();
		dontMoveBlocksPos.shrink_to_fit();

		clip->vec1.push_back(RVector3());
		clip->vec2.push_back(RVector3());

		auto& warpBlock = stage.warpBlock;
		Warp* warp1 = nullptr;
		Warp* warp2 = nullptr;

		if (clip->gateNumber1.empty() == false)
		{
			warp1 = warpBlock[clip->gateNumber1[clip->gateNumber1.size() - 1]];
		}
		if (clip->gateNumber2.empty() == false)
		{
			warp2 = warpBlock[clip->gateNumber2[clip->gateNumber2.size() - 1]];
		}

		for (size_t i = 0; i < tmp.size(); i++)
		{
			if (warp1 == nullptr || warp1->twinBlockNumber == -1)
			{
				goto WARP2;
			}

			if ((tmp[i].pos.x != tmp[warp1->twinBlockNumber].pos.x && tmp[i].pos.y == tmp[warp1->twinBlockNumber].pos.y) ||
				(tmp[i].pos.x == tmp[warp1->twinBlockNumber].pos.x && tmp[i].pos.y != tmp[warp1->twinBlockNumber].pos.y))
			{
				// ブロックが同軸上に無い時は無視する
				continue;
			}

			if (tmp[i].pos == tmp[warp1->twinBlockNumber].pos)
			{
				continue;
			}

			if (tmp[i].type == BlockType::WARP_OPEN_BLOCK)
			{
				warpBlockNumber.push_back((int)i);
				isWarp = true;
				continue;
			}

			if (tmp[i].type < 0 ||
				(caughtFlag[tmp[i].type].second == false && moveFlag[tmp[i].type].second == false))
			{
				// ブロックの処理が無い場合は無視する
				continue;
			}

			// 不動ブロックの時の処理
			if (moveFlag[tmp[i].type].second == false)
			{
				dontMoveBlocksPos.push_back(tmp[i].pos);
				continue;
			}

			if (warpBlock[warp1->gateNumber]->forwordVec == RVector3(1.0f, 0.0f, 0.0f))
			{
				if ((tmp[warp1->twinBlockNumber].pos.x - tmp[i].pos.x) < 0.0f)
				{
					if (clip->ReferencePoint1 == -1)
					{
						clip->ReferencePoint1 = (int)i;
						clip->vec1[clip->vec1.size() - 1] = tmp[warp1->twinBlockNumber].pos - tmp[i].pos;
						clip->vec1[clip->vec1.size() - 1].z = 0.0f;
					}
					else if ((tmp[warp1->twinBlockNumber].pos.x - tmp[i].pos.x) > clip->vec1[clip->vec1.size() - 1].x)
					{
						clip->ReferencePoint1 = (int)i;
						clip->vec1[clip->vec1.size() - 1] = tmp[warp1->twinBlockNumber].pos - tmp[i].pos;
						clip->vec1[clip->vec1.size() - 1].z = 0.0f;
					}
				}
			}
			else if (warpBlock[warp1->gateNumber]->forwordVec == RVector3(-1.0f, 0.0f, 0.0f))
			{
				if ((tmp[warp1->twinBlockNumber].pos.x - tmp[i].pos.x) > 0.0f)
				{
					if (clip->ReferencePoint1 == -1)
					{
						clip->ReferencePoint1 = (int)i;
						clip->vec1[clip->vec1.size() - 1] = tmp[warp1->twinBlockNumber].pos - tmp[i].pos;
						clip->vec1[clip->vec1.size() - 1].z = 0.0f;
					}
					else if ((tmp[warp1->twinBlockNumber].pos.x - tmp[i].pos.x) < clip->vec1[clip->vec1.size() - 1].x)
					{
						clip->ReferencePoint1 = (int)i;
						clip->vec1[clip->vec1.size() - 1] = tmp[warp1->twinBlockNumber].pos - tmp[i].pos;
						clip->vec1[clip->vec1.size() - 1].z = 0.0f;
					}
				}
			}
			if (warpBlock[warp1->gateNumber]->forwordVec == RVector3(0.0f, 1.0f, 0.0f))
			{
				if ((tmp[warp1->twinBlockNumber].pos.y - tmp[i].pos.y) < 0.0f)
				{
					if (clip->ReferencePoint1 == -1)
					{
						clip->ReferencePoint1 = (int)i;
						clip->vec1[clip->vec1.size() - 1] = tmp[warp1->twinBlockNumber].pos - tmp[i].pos;
						clip->vec1[clip->vec1.size() - 1].z = 0.0f;
					}
					else if ((tmp[warp1->twinBlockNumber].pos.y - tmp[i].pos.y) > clip->vec1[clip->vec1.size() - 1].y)
					{
						clip->ReferencePoint1 = (int)i;
						clip->vec1[clip->vec1.size() - 1] = tmp[warp1->twinBlockNumber].pos - tmp[i].pos;
						clip->vec1[clip->vec1.size() - 1].z = 0.0f;
					}
				}
			}
			else if (warpBlock[warp1->gateNumber]->forwordVec == RVector3(0.0f, -1.0f, 0.0f))
			{
				if ((tmp[warp1->twinBlockNumber].pos.y - tmp[i].pos.y) > 0.0f)
				{
					if (clip->ReferencePoint1 == -1)
					{
						clip->ReferencePoint1 = (int)i;
						clip->vec1[clip->vec1.size() - 1] = tmp[warp1->twinBlockNumber].pos - tmp[i].pos;
						clip->vec1[clip->vec1.size() - 1].z = 0.0f;
					}
					else if ((tmp[warp1->twinBlockNumber].pos.y - tmp[i].pos.y) < clip->vec1[clip->vec1.size() - 1].y)
					{
						clip->ReferencePoint1 = (int)i;
						clip->vec1[clip->vec1.size() - 1] = tmp[warp1->twinBlockNumber].pos - tmp[i].pos;
						clip->vec1[clip->vec1.size() - 1].z = 0.0f;
					}
				}
			}

			WARP2:
			if (warp2 == nullptr || warp2->twinBlockNumber == -1)
			{
				continue;
			}

			// ブロックが同軸上に無い時は無視する
			if ((tmp[i].pos.x != tmp[warp2->twinBlockNumber].pos.x && tmp[i].pos.y == tmp[warp2->twinBlockNumber].pos.y) ||
				(tmp[i].pos.x == tmp[warp2->twinBlockNumber].pos.x && tmp[i].pos.y != tmp[warp2->twinBlockNumber].pos.y))
			{
				continue;
			}

			if (tmp[i].pos == tmp[warp2->twinBlockNumber].pos)
			{
				continue;
			}

			if (tmp[i].type == BlockType::WARP_OPEN_BLOCK)
			{
				warpBlockNumber.push_back((int)i);
				isWarp = true;
				continue;
			}

			if (tmp[i].type < 0 ||
				(caughtFlag[tmp[i].type].second == false && moveFlag[tmp[i].type].second == false))
			{
				// ブロックの処理が無い場合は無視する
				continue;
			}

			// 不動ブロックの時の処理
			if (moveFlag[tmp[i].type].second == false)
			{
				dontMoveBlocksPos.push_back(tmp[i].pos);
				continue;
			}

			if (warpBlock[warp2->gateNumber]->forwordVec == RVector3(1.0f, 0.0f, 0.0f))
			{
				if ((tmp[warp2->twinBlockNumber].pos.x - tmp[i].pos.x) < 0.0f)
				{
					if (clip->ReferencePoint2 == -1)
					{
						clip->ReferencePoint2 = (int)i;
						clip->vec2[clip->vec2.size() - 1] = tmp[warp2->twinBlockNumber].pos - tmp[i].pos;
						clip->vec2[clip->vec2.size() - 1].z = 0.0f;
					}
					else if ((tmp[warp2->twinBlockNumber].pos.x - tmp[i].pos.x) > clip->vec2[clip->vec2.size() - 1].x)
					{
						clip->ReferencePoint2 = (int)i;
						clip->vec2[clip->vec2.size() - 1] = tmp[warp2->twinBlockNumber].pos - tmp[i].pos;
						clip->vec2[clip->vec2.size() - 1].z = 0.0f;
					}
				}
			}
			else if (warpBlock[warp2->gateNumber]->forwordVec == RVector3(-1.0f, 0.0f, 0.0f))
			{
				if ((tmp[warp2->twinBlockNumber].pos.x - tmp[i].pos.x) > 0.0f)
				{
					if (clip->ReferencePoint2 == -1)
					{
						clip->ReferencePoint2 = (int)i;
						clip->vec2[clip->vec2.size() - 1] = tmp[warp2->twinBlockNumber].pos - tmp[i].pos;
						clip->vec2[clip->vec2.size() - 1].z = 0.0f;
					}
					else if ((tmp[warp2->twinBlockNumber].pos.x - tmp[i].pos.x) < clip->vec2[clip->vec2.size() - 1].x)
					{
						clip->ReferencePoint2 = (int)i;
						clip->vec2[clip->vec2.size() - 1] = tmp[warp2->twinBlockNumber].pos - tmp[i].pos;
						clip->vec2[clip->vec2.size() - 1].z = 0.0f;
					}
				}
			}
			if (warpBlock[warp2->gateNumber]->forwordVec == RVector3(0.0f, 1.0f, 0.0f))
			{
				if ((tmp[warp2->twinBlockNumber].pos.y - tmp[i].pos.y) < 0.0f)
				{
					if (clip->ReferencePoint2 == -1)
					{
						clip->ReferencePoint2 = (int)i;
						clip->vec2[clip->vec2.size() - 1] = tmp[warp2->twinBlockNumber].pos - tmp[i].pos;
						clip->vec2[clip->vec2.size() - 1].z = 0.0f;
					}
					else if ((tmp[warp2->twinBlockNumber].pos.y - tmp[i].pos.y) > clip->vec2[clip->vec2.size() - 1].y)
					{
						clip->ReferencePoint2 = (int)i;
						clip->vec2[clip->vec2.size() - 1] = tmp[warp2->twinBlockNumber].pos - tmp[i].pos;
						clip->vec2[clip->vec2.size() - 1].z = 0.0f;
					}
				}
			}
			else if (warpBlock[warp2->gateNumber]->forwordVec == RVector3(0.0f, -1.0f, 0.0f))
			{
				if ((tmp[warp2->twinBlockNumber].pos.y - tmp[i].pos.y) > 0.0f)
				{
					if (clip->ReferencePoint2 == -1)
					{
						clip->ReferencePoint2 = (int)i;
						clip->vec2[clip->vec2.size() - 1] = tmp[warp2->twinBlockNumber].pos - tmp[i].pos;
						clip->vec2[clip->vec2.size() - 1].z = 0.0f;
					}
					else if ((tmp[warp2->twinBlockNumber].pos.y - tmp[i].pos.y) < clip->vec2[clip->vec2.size() - 1].y)
					{
						clip->ReferencePoint2 = (int)i;
						clip->vec2[clip->vec2.size() - 1] = tmp[warp2->twinBlockNumber].pos - tmp[i].pos;
						clip->vec2[clip->vec2.size() - 1].z = 0.0f;
					}
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
			for (size_t l = 0; l < clip->vec1.size(); l++)
			{
				if (clip->vec1[l].length() == 0.0f)
				{
					break;
				}

				keepVec = clip->vec1[l];
				keepVec = keepVec.norm();

				for (size_t j = 1; keepVec * (const float)j != clip->vec1[l]; j++)
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
							tmp[k].pos != tmp[i].pos + keepVec * blockSize * (const float)j)
						{
							continue;
						}

						if (clip->vec1[l].x != 0.0f)
						{
							if (tmp[k].pos.x == tmp[i].pos.x || tmp[k].pos.y != tmp[i].pos.y)
							{
								continue;
							}
							if ((tmp[k].pos.x - tmp[i].pos.x) > clip->vec1[l].x)
							{
								isEnd = true;
								clip->vec1[l] = tmp[k].pos - tmp[i].pos;
								break;
							}
						}
						if (clip->vec1[l].y != 0.0f)
						{
							if (tmp[k].pos.x != tmp[i].pos.x || tmp[k].pos.y == tmp[i].pos.y)
							{
								continue;
							}
							if ((tmp[k].pos.y - tmp[i].pos.y) > clip->vec1[l].y)
							{
								isEnd = true;
								clip->vec1[l] = tmp[k].pos - tmp[i].pos;
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
		if (stage.blocks[i].number == clip->blockNumber2)
		{
			for (size_t l = 0; l < clip->vec2.size(); l++)
			{
				if (clip->vec2[l].length() == 0.0f)
				{
					break;
				}

				keepVec = clip->vec2[l];
				keepVec = keepVec.norm();
		
				for (size_t j = 1; keepVec * blockSize * (const float)j != clip->vec2[l]; j++)
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
							tmp[k].pos != tmp[i].pos + keepVec * blockSize * (const float)j)
						{
							continue;
						}

						if (clip->vec2[l].x != 0.0f)
						{
							if (tmp[k].pos.x == tmp[i].pos.x || tmp[k].pos.y != tmp[i].pos.y)
							{
								continue;
							}
							if ((tmp[k].pos.x - tmp[i].pos.x) < clip->vec2[l].x)
							{
								isEnd = true;
								clip->vec2[l] = tmp[k].pos - tmp[i].pos;
								break;
							}
						}
						if (clip->vec2[l].y != 0.0f)
						{
							if (tmp[k].pos.x != tmp[i].pos.x || tmp[k].pos.y == tmp[i].pos.y)
							{
								continue;
							}
							if ((tmp[k].pos.y - tmp[i].pos.y) < clip->vec2[l].y)
							{
								isEnd = true;
								clip->vec2[l] = tmp[k].pos - tmp[i].pos;
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
	}

	while (isWarp)
	{
		isWarp = false;
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

	auto& tmp = stage.blocks;
	std::vector<float> dontMoveBlocksPos; //プレイヤーと同軸上にある不動ブロックの場所
	clip->vec1.push_back(RVector3());
	clip->vec2.push_back(RVector3());
	bool isWarp = false;

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

			if (stage.blocks[i].type != BlockType::WARP_OPEN_BLOCK)
			{
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
					clip->vec1[clip->vec1.size() - 1] = player->position - tmp[i].pos;
				}
				else if ((player->position.z - tmp[i].pos.z) > clip->vec1[clip->vec1.size() - 1].z)
				{
					clip->ReferencePoint1 = i;
					clip->vec1[clip->vec1.size() - 1] = player->position - tmp[i].pos;
				}
			}
			// ブロックがプレイヤーより-の方向にある時の処理
			else
			{
				if (clip->ReferencePoint2 == -1)
				{
					clip->ReferencePoint2 = i;
					clip->vec2[clip->vec2.size() - 1] = player->position - tmp[i].pos;
				}
				else if ((player->position.z - tmp[i].pos.z) < clip->vec2[clip->vec2.size() - 1].z)
				{
					clip->ReferencePoint2 = i;
					clip->vec2[clip->vec2.size() - 1] = player->position - tmp[i].pos;
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
				if (space > clip->vec1[clip->vec1.size() - 1].z)
				{
					clip->vec1[clip->vec1.size() - 1].z -= space;
				}
			}
			else
			{
				if (space < clip->vec2[clip->vec2.size() - 1].z)
				{
					clip->vec2[clip->vec2.size() - 1].z -= space;
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

			if (stage.blocks[i].type != BlockType::WARP_OPEN_BLOCK)
			{
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
					clip->vec1[clip->vec1.size() - 1] = player->position - tmp[i].pos;
				}
				else if ((player->position.x - tmp[i].pos.x) > clip->vec1[clip->vec1.size() - 1].x)
				{
					clip->ReferencePoint1 = i;
					clip->vec1[clip->vec1.size() - 1] = player->position - tmp[i].pos;
				}
			}
			// ブロックがプレイヤーより-の方向にある時の処理
			else
			{
				if (clip->ReferencePoint2 == -1)
				{
					clip->ReferencePoint2 = i;
					clip->vec2[clip->vec2.size() - 1] = player->position - tmp[i].pos;
				}
				else if ((player->position.x - tmp[i].pos.x) < clip->vec2[clip->vec2.size() - 1].x)
				{
					clip->ReferencePoint2 = i;
					clip->vec2[clip->vec2.size() - 1] = player->position - tmp[i].pos;
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
				if (space > clip->vec1[clip->vec1.size() - 1].x)
				{
					clip->vec1[clip->vec1.size() - 1].x -= space;
				}
			}
			else
			{
				if (space < clip->vec2[clip->vec2.size() - 1].x)
				{
					clip->vec2[clip->vec2.size() - 1].x -= space;
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
			if (clip->vec1[clip->vec1.size() - 1].length() == 0.0f)
			{
				continue;
			}

			keepVec = clip->vec1[clip->vec1.size() - 1];
			keepVec = keepVec.norm();

			for (size_t j = 1; keepVec * (const float)j != clip->vec1[clip->vec1.size() - 1]; j++)
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
						tmp[k].pos != tmp[i].pos + keepVec * blockSize * (const float)j)
					{
						continue;
					}

					if (clip->vec1[clip->vec1.size() - 1].x != 0.0f)
					{
						if (tmp[k].pos.x == tmp[i].pos.x || tmp[k].pos.y != tmp[i].pos.y || tmp[k].pos.y != tmp[i].pos.y)
						{
							continue;
						}
						if ((tmp[k].pos.x - tmp[i].pos.x) > clip->vec1[clip->vec1.size() - 1].x)
						{
							isEnd = true;
							clip->vec1[clip->vec1.size() - 1] = tmp[k].pos - tmp[i].pos;
							break;
						}
					}
					if (clip->vec1[clip->vec1.size() - 1].z != 0.0f)
					{
						if (tmp[k].pos.x != tmp[i].pos.x || tmp[k].pos.y != tmp[i].pos.y || tmp[k].pos.y == tmp[i].pos.y)
						{
							continue;
						}
						if ((tmp[k].pos.z - tmp[i].pos.z) > clip->vec1[clip->vec1.size() - 1].z)
						{
							isEnd = true;
							clip->vec1[clip->vec1.size() - 1] = tmp[k].pos - tmp[i].pos;
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
			if (clip->vec2[clip->vec2.size() - 1].length() == 0.0f)
			{
				continue;
			}

			keepVec = clip->vec2[clip->vec2.size() - 1];
			keepVec = keepVec.norm();

			for (size_t j = 1; keepVec * (const float)j != clip->vec2[clip->vec2.size() - 1]; j++)
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
						tmp[k].pos != tmp[i].pos + keepVec * blockSize * (const float)j)
					{
						continue;
					}

					if (clip->vec2[clip->vec2.size() - 1].x != 0.0f)
					{
						if (tmp[k].pos.x == tmp[i].pos.x || tmp[k].pos.y != tmp[i].pos.y || tmp[k].pos.z != tmp[i].pos.z)
						{
							continue;
						}
						if ((tmp[k].pos.x - tmp[i].pos.x) < clip->vec2[clip->vec2.size() - 1].x)
						{
							isEnd = true;
							clip->vec2[clip->vec2.size() - 1] = tmp[k].pos - tmp[i].pos;
							break;
						}
					}
					if (clip->vec2[clip->vec2.size() - 1].z != 0.0f)
					{
						if (tmp[k].pos.x != tmp[i].pos.x || tmp[k].pos.y == tmp[i].pos.y || tmp[k].pos.z == tmp[i].pos.z)
						{
							continue;
						}
						if ((tmp[k].pos.z - tmp[i].pos.z) < clip->vec2[clip->vec2.size() - 1].z)
						{
							isEnd = true;
							clip->vec2[clip->vec2.size() - 1] = tmp[k].pos - tmp[i].pos;
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
