#include "GameCommonData.h"
#include "Raki_WinAPI.h"
#include "NY_Camera.h"
#include <DirectXMath.h>
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
