#include "Stage.h"

#define SefeDelete(ptr)\
if (ptr != nullptr)\
{\
	delete ptr;\
	ptr = nullptr;\
}

Stage::Stage() :
	stage(nullptr)
{
}

Stage::~Stage()
{
	SefeDelete(stage);
}

void Stage::Update()
{
}

void Stage::Draw()
{
	stage->Draw();
}

void Stage::StepBack()
{
	if (clipBlock.empty())
	{
		return;
	}

	stage->debugBoxObj[clipBlock.top().blockNumber1]->position = clipBlock.top().pos1;
	stage->debugBoxObj[clipBlock.top().blockNumber2]->position = clipBlock.top().pos2;

	clipBlock.pop();
}

void Stage::Reset()
{
	stage->Reset();

	while (clipBlock.empty() == false)
	{
		clipBlock.pop();
	}
}
