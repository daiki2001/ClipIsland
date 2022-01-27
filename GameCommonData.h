#pragma once
#include "BlockData.h"
#include "NY_Object3DMgr.h"
#include "RVector.h"

namespace GameCommonData
{
class CommonData final
{
private: // シングルトン化
	CommonData() {}
	~CommonData() {}
public:
	CommonData(const CommonData&) = delete;
	CommonData operator=(const CommonData&) = delete;

public: // 静的メンバ関数
	// CommonDataの中に宣言した変数の初期化
	static void StaticInitiizer(); //ゲーム中に出てくる共通の箱のモデル

public: // 静的メンバ変数
	static UINT boxGraph;       //ゲーム中に出てくる共通の箱の画像
	static NY_Model3D boxModel; //ゲーム中に出てくる共通の箱のモデル
};

RVector3 ScreenToWorld(const XMFLOAT2& screen);
} //GameCommonData