#pragma once
#include <wrl.h>

#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx12.h"
#include "imgui/imgui.h"

class ImguiMgr final
{
private:
	//imgui用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeapForImgui;

	//コンストラクタ、デストラクタを無効
	ImguiMgr() {};
	~ImguiMgr() {};

public:
	//imguiMgrのゲッタ
	static ImguiMgr *Get() {
		static ImguiMgr obj;
		return &obj;
	}

	/// <summary>
	/// imguiの初期化、imgui用デスクリプタヒープ作成
	/// </summary>
	/// <param name="dev">ID3D12Deviceのポインタ</param>
	/// <returns>bool型で成否</returns>
	bool InitImgui(ID3D12Device *dev, HWND hwnd);

	void StartDrawImgui(const char *windowTitle, int posX, int posY);

	void EndDrawImgui();

	void FinalizeImgui();
};

namespace myImgui {
	/// <summary>
	/// imguiの初期化、imgui用デスクリプタヒープ作成
	/// Raki_DX12Bの初期化処理で実行するため不要？
	/// </summary>
	/// <param name="dev">ID3D12Deviceのポインタ</param>
	/// <returns>bool型で成否</returns>
	bool InitializeImGui(ID3D12Device *dev, HWND hwnd);

	/// <summary>
	/// Imguiのウィンドウを作成、描画開始。ウィンドウに表示する内容はこの関数の実行より下に書くこと
	/// </summary>
	/// <param name="windowTitle">表示するimguiウィンドウのタイトル</param>
	/// <param name="posX">初期の横幅</param>
	/// <param name="posY">初期の縦幅</param>
	void StartDrawImGui(const char *windowTitle, int sizeX, int sizeY);

	/// <summary>
	/// Imguiのウィンドウ描画の終了。ウィンドウに表示する内容はこの関数の実行より上に書くこと。
	/// </summary>
	void EndDrawImGui();

	/// <summary>
	/// imguiの終了処理。アプリケーションの終了タイミングで実行すること
	/// </summary>
	void FinalizeImGui();
}