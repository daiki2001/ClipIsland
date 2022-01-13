#pragma once

#include <windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>

#include "NY_Camera.h"
#include "RVector.h"

class ParticleManager
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// 頂点データ
	struct PVertex {
		RVector3 pos;
		float scale;
	};

	//定数バッファデータ構造体
	struct ConstBufferData {
		XMMATRIX mat;
		XMMATRIX matBillBoard;
	};

	// パーティクル1粒クラス
	class Particle {
		// Microsoft::WRL::を省略
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		// DirectX::を省略
		using XMFLOAT2 = DirectX::XMFLOAT2;
		using XMFLOAT3 = DirectX::XMFLOAT3;
		using XMFLOAT4 = DirectX::XMFLOAT4;
		using XMMATRIX = DirectX::XMMATRIX;

	public:
		//座標
		RVector3 pos;
		//速度
		RVector3 vel;
		//加速度
		RVector3 acc;
		//色
		RVector3 color;
		//スケーリング
		float scale;
		//回転
		float rot;
		//初期値
		XMFLOAT3 s_color = {};
		float s_scale = 1.0f;
		float s_rotation = 0.0f;
		// 最終値
		XMFLOAT3 e_color = {};
		float e_scale = 0.0f;
		float e_rotation = 0.0f;
		// 現在フレーム
		int nowFrame = 0;
		// 終了フレーム
		int endFrame = 0;
	};

private:
	// 最大生成頂点数
	static const int MAX_VERTEX = 65536;
	// デバイス
	ID3D12Device *dev;

public:
	/// <summary>
	/// パーティクルマネージャー生成
	/// </summary>
	/// <param name="camera">NY_Cameraのインスタンス</param>
	/// <returns>生成したParticleManagerのインスタンス</returns>
	static ParticleManager *Create(NY_Camera *camera);

	/// <summary>
	/// パーティクルマネージャー初期化
	/// </summary>
	void Initialize();
	
	/// <summary>
	/// 全パーティクル更新
	/// </summary>
	void Update();

	/// <summary>
	/// パーティクル描画
	/// </summary>
	void Draw();

	/// <summary>
	/// パーティクル追加
	/// </summary>
	void Add(int life, RVector3 pos, RVector3 vel, RVector3 acc, float startScale, float endScale);

private:


};

