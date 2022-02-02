#include "SceneChangeDirection.h"

SceneChangeDirection::SceneChangeDirection() {
	//テクスチャ読み込み
	tex1 = TexManager::LoadTexture("Resources/SCClip1.png");
	tex2 = TexManager::LoadTexture("Resources/SCClip2.png");
	//テクスチャ生成
	XMFLOAT2 size = { 640,720 };
	XMFLOAT2 anchor = { 0.5,0.5 };
	press1.CreateSprite(size, anchor, tex1, false, nullptr);
	press2.CreateSprite(size, anchor, tex2, false, nullptr);
	//座標を画面外に
	press1.spdata.position = RVector3(-1280 / 2, 720 / 2, 0);
	press2.spdata.position = RVector3(1280 + 1280 / 2, 720 / 2, 0);
	press1.UpdateSprite();
	press2.UpdateSprite();
}

SceneChangeDirection::~SceneChangeDirection()
{
}

void SceneChangeDirection::Init()
{
	//演出未完
	isEndDir = false;
	//どっちの演出も初期化
	frameCount = 0;
	isStart = false;
	isEnd = false;
}

void SceneChangeDirection::Start_StartDir()
{
	//スタート演出
	isStart = true;
	//演出開始
	isEndDir = false;
	//フレームカウント初期化
	frameCount = 0;
	//イージング目標座標設定
	//開始点
	ps1s = RVector3(1280 / 4, 720 / 2, 0);
	ps2s = RVector3(1280 / 4 * 3, 720 / 2, 0);
	//終了点
	ps1e = RVector3(-1280 / 4, 720 / 2, 0);
	ps2e = RVector3(1280 + 1280 / 4, 720 / 2, 0);
}

void SceneChangeDirection::Start_EndDir()
{
	//終了演出
	isEnd = true;
	//演出開始
	isEndDir = false;
	//フレームカウント初期化
	frameCount = 0;
	//イージング目標座標設定
	//開始点
	ps1s = RVector3(-1280 / 4, 720 / 2, 0);
	ps2s = RVector3(1280 + 1280 / 4, 720 / 2, 0);
	//終了点
	ps1e = RVector3(1280 / 4, 720 / 2, 0);
	ps2e = RVector3((1280 / 4) * 3, 720 / 2, 0);
}

void SceneChangeDirection::Update()
{
	//インクリメント
	if (frameCount < DIR_FRAME) {
		frameCount++;
	}
	float timeRate = float(frameCount) / float(DIR_FRAME);

	//イージング計算
	if (isStart) {
		press1.spdata.position = Rv3Ease::OutQuad(ps1s, ps1e, timeRate);
		press2.spdata.position = Rv3Ease::OutQuad(ps2s, ps2e, timeRate);
	}
	else if(isEnd){
		press1.spdata.position = Rv3Ease::InQuad(ps1s, ps1e, timeRate);
		press2.spdata.position = Rv3Ease::InQuad(ps2s, ps2e, timeRate);
	}
	press1.UpdateSprite();
	press2.UpdateSprite();

	//フレーム数が既定値
	if (frameCount > DIR_FRAME) {
		isEndDir = true;
	}
}

void SceneChangeDirection::Draw()
{
	//描画準備
	SpriteManager::Get()->SetCommonBeginDraw();
	//描画
	press1.Draw();
	press2.Draw();
}
