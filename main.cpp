#include "Raki_DX12B.h"
#include "FPS.h"
#include "Audio.h"
#include "TexManager.h"

#include "NY_Object3DMgr.h"
#include "Sprite.h"
#include "Raki_imguiMgr.h"

#include "GameCommonData.h"

#include "Player.h"
#include "Stage.h"

#include "Collision.h"

#include "StageMoveParticle.h"
#include "SeaParticle.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace GameCommonData;
using namespace GameCommonData::BlockData;

//-----------RakiEngine_Alpha.ver-----------//

enum Scene
{
    TITLE,
    SELECT,
    GAME_MAIN,
    GAME_OVER,
    GAME_CLEAR
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
    Raki_WinAPI* rakiWinApp;
    rakiWinApp = new Raki_WinAPI;
    rakiWinApp->CreateGameWindow();

    Raki_DX12B::Get()->Initialize(rakiWinApp);

    myImgui::InitializeImGui(Raki_DX12B::Get()->GetDevice(), Raki_WinAPI::GetHWND());

    //オブジェクト管理
    NY_Object3DManager::Get()->CreateObject3DManager(Raki_DX12B::Get()->GetDevice(), rakiWinApp->window_width, rakiWinApp->window_height);
    SpriteManager::Get()->CreateSpriteManager(Raki_DX12B::Get()->GetDevice(), Raki_DX12B::Get()->GetGCommandList(), rakiWinApp->window_width, rakiWinApp->window_height);
    TexManager::InitTexManager();

    //カメラ
    NY_Camera* cam = camera;
    RVector3 eye = { 0.0f, 0.0f, -200.0f };
    RVector3 target = { 0.0f, 0.0f, 0.0f };
    RVector3 up = { 0.0f, 1.0f, 0.0f };
    cam->SetViewStatusEyeTargetUp(eye, target, up);

    //音
    Audio::Init();

#pragma endregion GameValue

    FPS::Get()->Start();

    GameCommonData::CommonData::StaticInitiizer();

    /*背景*/
    UINT backgroundGraph = TexManager::LoadTexture("./Resources/background.png");
    Sprite background;
    background.CreateSprite({ (float)Raki_WinAPI::window_width, (float)Raki_WinAPI::window_height }, { 0.0f, 0.0f }, backgroundGraph, true);

    /*タイトル*/
    UINT titleGraph = TexManager::LoadTexture("./Resources/Title.png");
    Sprite title;
    title.CreateSprite({ (float)Raki_WinAPI::window_width, (float)Raki_WinAPI::window_height }, { 0.0f, 0.0f }, titleGraph, true);

    /*ステージセレクト*/
    UINT stageGraph[] =
    {
        TexManager::LoadTexture("./Resources/stage1.png"),
        TexManager::LoadTexture("./Resources/stage2.png"),
        TexManager::LoadTexture("./Resources/stage3.png"),
        TexManager::LoadTexture("./Resources/stage4.png"),
        TexManager::LoadTexture("./Resources/stage5.png"),
        TexManager::LoadTexture("./Resources/stage6.png"),
    };
    const size_t stageCount = sizeof(stageGraph) / sizeof(stageGraph[0]);
    Sprite stage[stageCount];
    for (size_t i = 0; i < stageCount; i++)
    {
        stage[i].CreateSprite({ 320.0f, 64.0f }, { 0.5f, 0.5f }, stageGraph[i], false);
        stage[i].spdata.position.x = (float)Raki_WinAPI::window_width / 2.0f;
        stage[i].spdata.position.y = (float)Raki_WinAPI::window_height / 2.0f;
        stage[i].UpdateSprite();
    }
    UINT arrowLGraph = TexManager::LoadTexture("./Resources/Arrow_L.png");
    Sprite arrowL;
    arrowL.CreateSprite({ 128.0f, 128.0f }, { 0.0f, 0.5f }, arrowLGraph, false);
    arrowL.spdata.position.x = 200.0f;
    arrowL.spdata.position.y = (float)Raki_WinAPI::window_height / 2.0f;
    arrowL.UpdateSprite();
    UINT arrowRGraph = TexManager::LoadTexture("./Resources/Arrow_R.png");
    Sprite arrowR;
    arrowR.CreateSprite({ 128.0f, 128.0f }, { 1.0f, 0.5f }, arrowRGraph, false);
    arrowR.spdata.position.x = Raki_WinAPI::window_width - 200.0f;
    arrowR.spdata.position.y = (float)Raki_WinAPI::window_height / 2.0f;
    arrowR.UpdateSprite();

    /*チュートリアル*/
    UINT tutorialGraph[] =
    {
        TexManager::LoadTexture("./Resources/Tutorial1.png"),
        TexManager::LoadTexture("./Resources/Tutorial2.png"),
    };
    Sprite tutorial[sizeof(tutorialGraph) / sizeof(tutorialGraph[0])];
    for (size_t i = 0; i < sizeof(tutorialGraph) / sizeof(tutorialGraph[0]); i++)
    {
        tutorial[i].CreateSprite({ (float)Raki_WinAPI::window_width, (float)Raki_WinAPI::window_height }, { 0.0f, 0.0f }, tutorialGraph[i], true);
    }
    int tutorialCount = 0;
    bool isTutorial = true;

    /*ステージクリア*/
    bool stageClearFlag[stageCount];
    size_t clearCount = 0;
    for (size_t i = 0; i < stageCount; i++)
    {
        stageClearFlag[i] = false;
    }

    /*プレイヤー*/
    Player player;

    /*ステージ*/
    Stage stageData(&player);
    int stageNumber = 0;

    /*シーン遷移*/
    Scene scene = Scene::TITLE;

    /*パーティクル初期化*/
    StageMoveParticle::Get()->Init();
    SeaParticle::Get()->Init();

    /*パーティクル用変数*/
    int clipBlockPos[128] = { 0 };
    for (size_t i = 0; i < sizeof(clipBlockPos) / sizeof(clipBlockPos[0]); i++)
    {
        clipBlockPos[i] = -1;
    }

    bool nFlag = false;
    bool actFlag = false;
    bool actnFlag = false;
    bool doorFlag = false;

    while (true)  // ゲームループ
    {
        if (rakiWinApp->ProcessMessage()) { break; }

        //更新

        Input::StartGetInputState();

        switch (scene)
        {
        case TITLE:
            if (Input::isKeyTrigger(DIK_SPACE))
            {
                scene = Scene::SELECT;
            }

            // 描画開始
            Raki_DX12B::Get()->StartDraw();

            // スプライト描画
            SpriteManager::Get()->SetCommonBeginDraw();
            title.Draw();

            // 描画終了
            Raki_DX12B::Get()->EndDraw();
            break;
        case SELECT:
            if (Input::isKeyTrigger(DIK_A))
            {
                stageNumber--;

                if (stageNumber < 0)
                {
                    stageNumber = 0;
                }
            }
            if (Input::isKeyTrigger(DIK_D))
            {
                static const size_t openStageCount = 3;

                if (stageNumber < openStageCount + clearCount - 1)
                {
                    stageNumber++;
                }

                if (stageNumber >= stageCount)
                {
                    stageNumber = stageCount - 1;
                }
            }

            if (Input::isKeyTrigger(DIK_SPACE))
            {
                scene = Scene::GAME_MAIN;

                switch (stageNumber)
                {
                case 0:
                    stageData.Select("map001.boxmap", true);
                    player.position = { 0.0f, 0.0f, 0.0f };
                    stageData.Select("map1.boxmap", true);
                    break;
                case 1:
                    stageData.Select("map2.boxmap", true);
                    break;
                case 2:
                    stageData.Select("map3.boxmap", true);
                    break;
                case 3:
                    stageData.Select("map4.boxmap", true);
                    break;
                case 4:
                    stageData.Select("map5.boxmap", true);
                    break;
                case 5:
                    stageData.Select("map6.boxmap", true);
                    break;
                default:
                    stageData.Select("map1.boxmap", true);
                    break;
                }
            }

            // 描画開始
            Raki_DX12B::Get()->StartDraw();

            // スプライト描画
            SpriteManager::Get()->SetCommonBeginDraw();
            stage[stageNumber].Draw();
            arrowL.Draw();
            arrowR.Draw();

            // 描画終了
            Raki_DX12B::Get()->EndDraw();
            break;
        case GAME_MAIN:
#if _DEBUG
            if (Input::isKey(DIK_1))
            {
                stageData.Select("test1.boxmap", true);
            }
#endif //_DEBUG
            if (isTutorial)
            {
                player.playerOldPos = player.position;
                player.playerCollision.Update(player.position);
                stageData.Update();

                if (Input::isKeyTrigger(DIK_SPACE))
                {
                    tutorialCount++;
                    isTutorial = tutorialCount < sizeof(tutorial) / sizeof(tutorial[0]);
                }
            }
            else
            {
                player.Update();
                stageData.Update();
            }

            // パーティクルの更新
            StageMoveParticle::Get()->Update();
            SeaParticle::Get()->Update();

            {
                nFlag = false;
                actFlag = false;
                actnFlag = false;

                player.object->color = { 1, 1, 1, 1 };

            for (size_t i = 0; i < stageData.stage.blocks.size(); i++)
            {
                actFlag = true;
                if (player.position == stageData.stage.blocks[i].pos)
                {
                    //nFlag = true;
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
                }

                if (stageData.stage.blocks[i].type == BlockType::SWITCH && AB == true)
                {
                    stageData.Change();
                }

                if (stageData.stage.blocks[i].type == BlockType::NONE)
                {
                    stageData.stage.blocks[i].pos.z -= 50;
                }

                if (AB == true)
                {
                   nFlag = stageData.stage.blocks[i].type == BlockType::DONT_MOVE_BLOCK || stageData.stage.blocks[i].type == BlockType::DOOR /*|| nFlag == false*/;
                   actnFlag = false;
                   break;
                }

            }

                if (nFlag == true)
                {
                   //player.object->position = player.playerOldPos +player.offsetPos;
                   player.object->position = Rv3Ease::InQuad(player.endPos, player.startPos, player.timeRate) + player.offsetPos;
                   player.position = player.startPos;
                }
                 
                if (actFlag == true && actnFlag == true)
                {
                   //player.object->position = player.playerOldPos + player.offsetPos;
                   player.object->position = Rv3Ease::InQuad(player.endPos, player.startPos, player.timeRate) + player.offsetPos;
                   player.position = player.startPos;
                }

                if (isTutorial == false)
                {
                    stageData.Clip(Input::isKeyTrigger(DIK_SPACE));

                    if (Input::isKeyTrigger(DIK_R))
                    {
                        doorFlag = false;
                        stageData.Reset();
                    }

                    if (Input::isKeyTrigger(DIK_B))
                    {
                        stageData.StepBack();
                    }
#if _DEBUG
                    if (Input::isKeyTrigger(DIK_E))
                    {
                        stageData.Change();
                    }
#endif //_DEBUG
                }

                if (player.goalFlag)
                {
                    scene = Scene::GAME_CLEAR;

                    if (stageClearFlag[stageNumber] == false)
                    {
                        stageClearFlag[stageNumber] = true;
                        clearCount++;
                    }

                    player.goalFlag = false;
                }
            }

            NY_Object3DManager::Get()->UpdateAllObjects();

            // 描画開始
            Raki_DX12B::Get()->StartDraw();

            // 背景描画
            SpriteManager::Get()->SetCommonBeginDraw();
            if (isTutorial)
            {
                tutorial[tutorialCount].Draw();
            }
            else
            {
                background.Draw();
            }

            NY_Object3DManager::Get()->SetCommonBeginDrawObject3D();
            if (isTutorial == false)
            {
                // パーティクル描画
                StageMoveParticle::Get()->Draw();
                SeaParticle::Get()->Draw();

                // オブジェクト描画
                player.Draw();
                stageData.Draw();
            }

            // 前景描画
            SpriteManager::Get()->SetCommonBeginDraw();

            // 描画終了
            Raki_DX12B::Get()->EndDraw();
            break;
        case GAME_OVER:
            scene = Scene::SELECT;
            break;
        case GAME_CLEAR:
            scene = Scene::SELECT;
            break;
        default:
            break;
        }

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
