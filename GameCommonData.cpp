#include "GameCommonData.h"
#include "TexManager.h"

namespace GameCommonData
{
UINT CommonData::boxGraph = 0;
NY_Model3D CommonData::boxModel = {};

void CommonData::StaticInitiizer()
{
	using namespace GameCommonData::BlockData;

	boxGraph = TexManager::LoadTexture("./Resources/test.jpeg");
	boxModel.CreateBoxModel(blockSize / 2.0f, 1.0f, 1.0f, boxGraph);
}
RVector3 ScreenToWorld(const XMFLOAT2& screen)
{
	RVector3 returnPos;

	return returnPos;
}
} //GameCommonData
