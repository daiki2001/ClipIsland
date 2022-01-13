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
