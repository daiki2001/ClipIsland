#include "SeaParticle.h"
#include <NY_random.h>

void SeaParticle::Init()
{
	//カウント初期化
	spawnCount = 0;
}

void SeaParticle::Update()
{
	spawnCount++;
	//生成時間カウント
	if (spawnCount > SPAWN_DELAY) {
		//生成数を設定
		int spawnNum = NY_random::intrand_sl(SPAWN_NUM_MAX, SPAWN_NUM_MIN);
		for (int i = 0; i < spawnNum; i++) {
			//出現地点設定
			RVector3 spawnPos(
				NY_random::floatrand_sl(640, -640),
				NY_random::floatrand_sl(360, -360),
				0
			);
			//生成
			pManager->Add(120, spawnPos,
				RVector3(0, 0, 0), RVector3(0, 0, 0),
				0.5f, 5.0f,
				XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 0));
		}

		//カウントをリセット
		spawnCount = 0;
	}

	//全パーティクル更新
	pManager->Update();
}

void SeaParticle::Draw()
{
	//パーティクル描画
	pManager->Draw(pTex);
}

