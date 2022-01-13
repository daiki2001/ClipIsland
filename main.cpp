#include "Raki_DX12B.h"
#include "FPS.h"
#include "Audio.h"
#include "TexManager.h"

#include "NY_Object3DMgr.h"
#include "Sprite.h"
#include "Raki_imguiMgr.h"

#include "LoadStage.h"

using namespace DirectX;
using namespace Microsoft::WRL;

//-----------RakiEngine_Alpha.ver-----------//


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
    Raki_WinAPI *rakiWinApp;
    rakiWinApp = new Raki_WinAPI;
    rakiWinApp->CreateGameWindow();

    Raki_DX12B::Get()->Initialize(rakiWinApp);

    myImgui::InitializeImGui(Raki_DX12B::Get()->GetDevice(), Raki_WinAPI::GetHWND());

    //オブジェクト管理
    NY_Object3DManager::Get()->CreateObject3DManager(Raki_DX12B::Get()->GetDevice(), rakiWinApp->window_width, rakiWinApp->window_height);
    SpriteManager::Get()->CreateSpriteManager(Raki_DX12B::Get()->GetDevice(), Raki_DX12B::Get()->GetGCommandList(), rakiWinApp->window_width, rakiWinApp->window_height);
    TexManager::InitTexManager();

    //カメラ
    NY_Camera cam;
    cam._eye = { 0.0f, 0.0f, -200.0f };
    cam._target = { 0.0f, 0.0f, 0.0f };
    cam._up = { 0.0f, 1.0f, 0.0f };
    cam.UpdateViewMat();
    NY_Object3DManager::Get()->SetCamera(&cam);

    //音
    Audio::Init();

#pragma endregion GameValue

    FPS::Get()->Start();

    UINT backgroundGraph = TexManager::LoadTexture("./Resources/background.png");
    NY_Model3D background;
    background.CreatePlaneModelXY(110.0f, 60.0f, 1.0f, 1.0f, backgroundGraph);
    Object3d* backgroundObj = CreateObject3d(&background);

    LoadStage stageData;
    stageData.Load("test.boxmap");

    while (true)  // ゲームループ
    {
        if (rakiWinApp->ProcessMessage()) { break; }

        //更新
        Input::StartGetInputState();

        NY_Object3DManager::Get()->UpdateAllObjects();

        // 描画
        Raki_DX12B::Get()->StartDraw();
        NY_Object3DManager::Get()->SetCommonBeginDrawObject3D(Raki_DX12B::Get()->GetGCommandList());

        DrawObject3d(backgroundObj);

        stageData.Draw();

        Raki_DX12B::Get()->EndDraw();

        if (Input::isKey(DIK_ESCAPE))
        {
            break;
        }

        FPS::Get()->run();
    }

    //imgui終了
    myImgui::FinalizeImGui();

    // ウィンドウクラスを登録解除
    rakiWinApp->DeleteGameWindow();

    delete rakiWinApp;
    rakiWinApp = nullptr;

    return 0;
}


