#pragma once
#include <TexManager.h>
#include <ParticleManager.h>

#define SPAWN_DELAY		(20)
#define SPAWN_NUM_MAX	(16)
#define SPAWN_NUM_MIN	(8)

//シングルトン（いつもの）
class SeaParticle final
{
private:
	//エミッター
	ParticleManager *pManager;
	//パーティクルリソース
	UINT			pTex;
	//パーティクル生成時間カウント
	int				spawnCount;

	//コンストラクタ
	SeaParticle() {
		pTex = TexManager::LoadTexture("Resources/bubble.png");
		pManager = ParticleManager::Create();
	}
	//デストラクタ
	~SeaParticle() {
		delete pManager;
	}


public:
	//ゲッタ
	static SeaParticle *Get() {
		static SeaParticle ins;
		return &ins;
	}

	//初期化
	void Init();

	//更新（毎フレーム生成と更新をする）
	void Update();

	//描画
	void Draw();

};

#define SEA_INIT (SeaParticle::Get()->Init())
#define SEA_UPDATE (SeaParticle::Get()->Update())
#define SEA_DRAW (SeaParticle::Get()->Draw())