#pragma once
#include "BlockData.h"
#include "NY_Object3DMgr.h"
#include "RVector.h"

namespace GameCommonData
{
class StageBlockModels final
{
private: // シングルトン化
	StageBlockModels() {}
	~StageBlockModels() {}
public:
	StageBlockModels(const StageBlockModels&) = delete;
	StageBlockModels operator=(const StageBlockModels&) = delete;

public: // 定数
	static const RVector3 modelRot; //モデルの回転角度
	static const float modelScale; //モデルのスケーリング倍率

public: // 静的メンバ関数
	// StageBlockModelsの初期化
	static void StaticInitiizer(); //ゲーム中に出てくる共通の箱のモデル

public: // 静的メンバ変数
	static NY_Model3D moveModel; //動かせる地面のモデル
	static NY_Model3D stayModel; //動かない地面のモデル
	static NY_Model3D goalModel; //ゴールブロックのモデル
	static NY_Model3D switchOnModel; //起動したスイッチのモデル
	static NY_Model3D switchOffModel; //起動してないスイッチのモデル
	static NY_Model3D doorModel; //スイッチで消えるブロックのモデル

	static UINT debugBoxGraph;       //仮描画用の箱のテクスチャ
	static NY_Model3D debugBoxModel; //仮描画用の箱のモデル
};

// スクリーン座標→ワールド座標への変換
RVector3 ScreenToWorld(const XMFLOAT2& screen);

// XYZ軸に同じ値のスケーリングをかける
inline RVector3 ScaleXYZ(const float& scale) { return RVector3(scale, scale, scale); }
} //GameCommonData