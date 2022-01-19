#pragma once

#include <NY_Camera.h>
#include <ParticleManager.h>

//ステージを挟めるとき、ステージが挟まったときに出てくるパーティクル
//めんどくせえ、シングルトンだ
class StageMoveParticle final
{
private:
	//パーティクルマネージャー（エミッタ）
	ParticleManager *pManager;
	//テクスチャ番号
	UINT ptex;

	//コンストラクタ、デストラクタ
	StageMoveParticle();
	~StageMoveParticle();

public:
	//コピー、代入無効


	//ゲッタ
	static StageMoveParticle *Get() {
		static StageMoveParticle ins;
		return &ins;
	}

	//初期化
	void Init(NY_Camera *cam);

	//更新
	void Update();

	/// <summary>
	/// パーティクル発生
	/// <para>動かせるブロックを示すパーティクルは、毎フレーム呼び出す</para>
	/// <para>動かしたあとに発生するやつは、動かした直後に呼び出す</para>
	/// </summary>
	/// <param name="spawnPos">出現中心点</param>
	/// <param name="max">出現範囲、正の方向</param>
	/// <param name="min">出現範囲、負の方向</param>
	void SpawnMoveStandbyParticle(RVector3 spawnPos, RVector3 max, RVector3 min, float s_scale = 0.1f, float e_scale = 0.01f);

	//描画
	void Draw();

	//終了
	void Finalize();

};

