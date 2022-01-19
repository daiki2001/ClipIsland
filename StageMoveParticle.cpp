#include "StageMoveParticle.h"

#include <TexManager.h>
#include <NY_random.h>

StageMoveParticle::StageMoveParticle()
{
	//テクスチャ読み込み
	ptex = TexManager::LoadTexture("Resources/blockParticleTex.png");
}

StageMoveParticle::~StageMoveParticle()
{
	//pManagerが消されていない場合
	if (pManager != nullptr) {
		delete pManager;//消去
		pManager = nullptr;
	}
}

void StageMoveParticle::Init(NY_Camera *cam)
{
	//カメラを取り込み、エミッター生成
	pManager = ParticleManager::Create(cam);
}

void StageMoveParticle::Update()
{
	//パーティクル更新
	pManager->Update();
}

void StageMoveParticle::SpawnMoveStandbyParticle(RVector3 spawnPos, RVector3 max, RVector3 min, float s_scale, float e_scale)
{
	//乱数で生成範囲を決定
	float maxX = spawnPos.x + max.x,
		maxY = spawnPos.y + max.y,
		maxZ = spawnPos.z + max.z,
		minX = spawnPos.x + min.x,
		minY = spawnPos.y + min.y,
		minZ = spawnPos.z + min.z;

	NY_random::floatrand_sl(1, -1);

	RVector3 spawnPosition(NY_random::floatrand_sl(maxX, minX),
		NY_random::floatrand_sl(maxY, minY),
		NY_random::floatrand_sl(maxZ, minZ));

	//速度、加速度を設定
	RVector3 velocity(NY_random::floatrand_sl(1, -1), 0, 1);
	RVector3 accel(NY_random::floatrand_sl(0.2, -0.2), 0, -0.1);

	//生成
	pManager->Add(30, spawnPosition, velocity, accel, s_scale, e_scale);

}

void StageMoveParticle::Draw()
{
	//描画
	pManager->Draw(ptex);
}

void StageMoveParticle::Finalize()
{
	//エミッターを消去
	delete pManager;
	pManager = nullptr;
}
