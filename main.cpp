﻿#include "Raki_DX12B.h"
#include "FPS.h"
#include "Audio.h"
#include "TexManager.h"

#include "NY_Object3DMgr.h"
#include "Sprite.h"
#include "SceneManager.h"
#include "Raki_imguiMgr.h"

using namespace DirectX;
using namespace Microsoft::WRL;

//-----------RakiEngine_Alpha.ver-----------//


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    Raki_WinAPI *rakiWinApp;
    rakiWinApp = new Raki_WinAPI;
    rakiWinApp->CreateGameWindow();

    Raki_DX12B::Get()->Initialize(rakiWinApp);

    myImgui::InitializeImGui(Raki_DX12B::Get()->GetDevice(), Raki_WinAPI::GetHWND());

    //オブジェクト管理
    NY_Object3DManager::Get()->CreateObject3DManager(Raki_DX12B::Get()->GetDevice(), rakiWinApp->window_width, rakiWinApp->window_height);
    SpriteManager::Get()->CreateSpriteManager(Raki_DX12B::Get()->GetDevice(), rakiWinApp->window_width, rakiWinApp->window_height);
    TexManager::InitTexManager();

    //音
    Audio::Init();

    //シーン管理
    //SceneManager *smgr;
    //smgr = new SceneManager;

#pragma endregion GameValue

    FPS::Get()->Start();

    while (true)  // ゲームループ
    {
        if (rakiWinApp->ProcessMessage()) { break; }

        //更新
        Input::StartGetInputState();

        //smgr->Update();

        //smgr->Draw();

        FPS::Get()->run();
    }

    //smgr->Finalize();

    //imgui終了
    myImgui::FinalizeImGui();

    // ウィンドウクラスを登録解除
    rakiWinApp->DeleteGameWindow();


    //delete smgr;
    //smgr = nullptr;
    delete rakiWinApp;
    rakiWinApp = nullptr;

    return 0;
}


