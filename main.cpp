#include "Raki_DX12B.h"
#include "FPS.h"
#include "Audio.h"
#include "TexManager.h"

#include "NY_Object3DMgr.h"
#include "Sprite.h"
#include "Raki_imguiMgr.h"

#include "Player.h"
#include "Stage.h"

#include "Collision.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace  BlockData;

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

    /*背景*/
    UINT backgroundGraph = TexManager::LoadTexture("./Resources/background.png");
    Sprite background;
    background.CreateSprite({ (float)Raki_WinAPI::window_width, (float)Raki_WinAPI::window_height }, { 0.0f, 0.0f }, backgroundGraph, true);

    /*プレイヤー*/
    Player player;

    /*ステージ*/
    Stage stageData(&player);
    stageData.Select("test3.boxmap", true);

    bool nFlag = false;
    bool actFlag = false;
    bool actnFlag = false;

    while (true)  // ゲームループ
    {
        if (rakiWinApp->ProcessMessage()) { break; }

        //更新

        Input::StartGetInputState();

        if (Input::isKey(DIK_1))
        {
            stageData.Select("test3.boxmap", true);
        }
        if (Input::isKey(DIK_2))
        {
            stageData.Select("test.boxmap", true);
        }

        /*for (size_t i = 0; i < stageData.stage.collision.size(); i++)
        {
            bool AB = intersectAABB(player.playerCollision, stageData.stage.collision[i]);
            if (AB == true)
            {
                player.object->color = { 0, 0, 1, 1 };
                if (stageData.stage.blockType[i] == BlockType::DONT_MOVE_BLOCK)
                {
                    
                   player.moveFlag = false;
                }
                else {
                    player.moveFlag = true;
                }
                break;
            }
            else
            {
                player.object->color = { 1, 1, 1, 1 };
            }
        }*/

        player.Update();
        stageData.Update();

        nFlag = false;
        actFlag = false;
        actnFlag = false;

        player.object->color = { 1, 1, 1, 1 };

        for (size_t i = 0; i < stageData.stage.blocks.size(); i++)
        {
            actFlag = true;
            if (player.position == stageData.stage.debugBoxObj[i]->position)
            {
                nFlag = true;
                actFlag = false;
                break;
            }
        }
///true
        for (size_t i = 0; i < stageData.stage.blocks.size(); i++)
        {
            actnFlag = true;
            bool AB = intersectAABB(player.playerCollision, stageData.stage.blocks[i].collision);
        
            if (stageData.stage.blocks[i].type == BlockType::GOAL && AB == true)
            {
                player.goalFlag = true;
                player.object->color = { 0, 0, 1, 1 };
                
            }
        
            if (AB == true)
            {
                nFlag = stageData.stage.blocks[i].type == BlockType::DONT_MOVE_BLOCK || nFlag == false;
                actnFlag = false;
                break;
            }
        }
///false


        if (nFlag == true)
        {
            player.position = player.playerOldPos;
            player.object->position = player.position;
            //player.object->color = { 0, 0, 1, 1 };
        }

        if (actFlag == true && actnFlag == true)
        {
            player.position = player.playerOldPos;
            player.object->position = player.position;
          //  player.object->color = { 0, 0, 1, 1 };
        }


       // player.object->color = { 1, 1, 1, 1 };


        stageData.Clip(Input::isKeyTrigger(DIK_SPACE));

        if (Input::isKeyTrigger(DIK_R))
        {
            stageData.Reset();
        }

        if (Input::isKeyTrigger(DIK_B))
        {
            stageData.StepBack();
        }

        NY_Object3DManager::Get()->UpdateAllObjects();

        // 描画開始
        Raki_DX12B::Get()->StartDraw();

        // 背景描画
        SpriteManager::Get()->SetCommonBeginDraw();
        background.Draw();

        // オブジェクト描画
        NY_Object3DManager::Get()->SetCommonBeginDrawObject3D();
        player.Draw();
        stageData.Draw();

        // 前景描画
        SpriteManager::Get()->SetCommonBeginDraw();

        // 描画終了
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


