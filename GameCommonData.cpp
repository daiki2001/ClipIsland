#include "GameCommonData.h"
#include "Raki_WinAPI.h"
#include "NY_Camera.h"
#include <DirectXMath.h>
#include "TexManager.h"

namespace GameCommonData
{
const RVector3 StageBlockModels::modelRot = { -270.0f, 90.0f, 270.0f };
const float StageBlockModels::modelScale = 20.0f;

NY_Model3D StageBlockModels::moveModel = {};
NY_Model3D StageBlockModels::stayModel = {};
NY_Model3D StageBlockModels::goalModel = {};
NY_Model3D StageBlockModels::switchOnModel = {};
NY_Model3D StageBlockModels::switchOffModel = {};
NY_Model3D StageBlockModels::doorModel = {};
NY_Model3D StageBlockModels::RectModel = {};

UINT StageBlockModels::debugBoxGraph = 0;
NY_Model3D StageBlockModels::debugBoxModel = {};

void StageBlockModels::StaticInitiizer()
{
	using namespace GameCommonData::BlockData;

	moveModel.LoadObjModel("underMove");
	stayModel.LoadObjModel("underStay");
	goalModel.LoadObjModel("underGoal");
	switchOnModel.LoadObjModel("underButtonOn");
	switchOffModel.LoadObjModel("underButtonOff");
	doorModel.LoadObjModel("underWood");
	RectModel.LoadObjModel("underRectangle");

	debugBoxGraph = TexManager::LoadTexture("./Resources/test.jpeg");
	debugBoxModel.CreateBoxModel(blockSize / 2.0f, 1.0f, 1.0f, debugBoxGraph);
}

RVector3 ScreenToWorld(const XMFLOAT2& screen)
{
	RVector3 returnPos = RVector3(screen.x, screen.y, 0.0f);

	// スクリーン座標 → クリップ座標
	returnPos.x = returnPos.x / Raki_WinAPI::window_width * 2.0f - 1.0f;
	returnPos.y = (returnPos.y / Raki_WinAPI::window_height * 2.0f - 1.0f) * -1.0f;

	// クリップ座標 → ビュー座標
	XMMATRIX invProj = DirectX::XMMatrixInverse(nullptr, camera->GetMatrixProjection());
	float invProjW = returnPos.x * invProj.r[0].m128_f32[3] + returnPos.y * invProj.r[1].m128_f32[3] + returnPos.z * invProj.r[2].m128_f32[3] + invProj.r[3].m128_f32[3];
	RVector3 invProjVec
	{
		(returnPos.x * invProj.r[0].m128_f32[0] + returnPos.y * invProj.r[1].m128_f32[0] + returnPos.z * invProj.r[2].m128_f32[0] + invProj.r[3].m128_f32[0]) / invProjW,
		(returnPos.x * invProj.r[0].m128_f32[1] + returnPos.y * invProj.r[1].m128_f32[1] + returnPos.z * invProj.r[2].m128_f32[1] + invProj.r[3].m128_f32[1]) / invProjW,
		(returnPos.x * invProj.r[0].m128_f32[2] + returnPos.y * invProj.r[1].m128_f32[2] + returnPos.z * invProj.r[2].m128_f32[2] + invProj.r[3].m128_f32[2]) / invProjW
	};
	returnPos = invProjVec;

	// ビュー座標 → ワールド座標
	XMMATRIX invView = DirectX::XMMatrixInverse(nullptr, camera->GetMatrixView());
	float invViewW = returnPos.x * invView.r[0].m128_f32[3] + returnPos.y * invView.r[1].m128_f32[3] + returnPos.z * invView.r[2].m128_f32[3] + invView.r[3].m128_f32[3];
	RVector3 invViewVec
	{
		(returnPos.x * invView.r[0].m128_f32[0] + returnPos.y * invView.r[1].m128_f32[0] + returnPos.z * invView.r[2].m128_f32[0] + invView.r[3].m128_f32[0]) / invViewW,
		(returnPos.x * invView.r[0].m128_f32[1] + returnPos.y * invView.r[1].m128_f32[1] + returnPos.z * invView.r[2].m128_f32[1] + invView.r[3].m128_f32[1]) / invViewW,
		(returnPos.x * invView.r[0].m128_f32[2] + returnPos.y * invView.r[1].m128_f32[2] + returnPos.z * invView.r[2].m128_f32[2] + invView.r[3].m128_f32[2]) / invViewW
	};
	returnPos = invViewVec;

	return returnPos;
}
} //GameCommonData
