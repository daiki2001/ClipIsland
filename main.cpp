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
	Sprite stage[sizeof(stageGraph) / sizeof(stageGraph[0])];
	for (size_t i = 0; i < sizeof(stageGraph) / sizeof(stageGraph[0]); i++)
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

	/*プレイヤー*/
	Player player;

	/*ステージ*/
	Stage stageData(&player);
	int stageNumber = 0;

	/*マウス関連の情報*/
	XMFLOAT2 mousePos;
	XMFLOAT2 mouseVec;

	/*シーン遷移*/
	Scene scene = Scene::TITLE;

	bool nFlag = false;
	bool actFlag = false;
	bool actnFlag = false;

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
				stageNumber++;

				if (stageNumber >= sizeof(stageGraph) / sizeof(stageGraph[0]))
				{
					stageNumber = sizeof(stageGraph) / sizeof(stageGraph[0]) - 1;
				}
			}
			if (Input::isKeyTrigger(DIK_SPACE))
			{
				scene = Scene::GAME_MAIN;

				switch (stageNumber)
				{
				case 0:
					stageData.Select("map1.boxmap", true);
					player.position = { 0.0f, -40.0f, 0.0f };
					break;
				case 1:
					stageData.Select("map2.boxmap", true);
					player.position = { 0.0f, -40.0f, 0.0f };
					break;
				case 2:
					stageData.Select("map3.boxmap", true);
					player.position = { 0.0f, -40.0f, 0.0f };
					break;
				case 3:
					stageData.Select("map4.boxmap", true);
					player.position = { 0.0f, -40.0f, 0.0f };
					break;
				case 4:
					stageData.Select("map5.boxmap", true);
					player.position = { 0.0f, 0.0f, 0.0f };
					break;
				case 5:
					stageData.Select("map6.boxmap", true);
					player.position = { 0.0f, -40.0f, 0.0f };
					break;
				default:
					stageData.Select("map1.boxmap", true);
					player.position = { 0.0f, -40.0f, 0.0f };
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
			if (Input::isKey(DIK_1))
			{
				stageData.Select("test.boxmap", true);
			}

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

			nFlag = false;
			actFlag = false;
			actnFlag = false;

			player.object->color = { 1, 1, 1, 1 };

			for (size_t i = 0; i < stageData.stage.blocks.size(); i++)
			{
				actFlag = true;
				if (player.position == stageData.stage.blocks[i].pos)
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
					//player.object->color = { 0, 0, 1, 1 };
				}

				if (stageData.stage.blocks[i].type == BlockType::SWITCH && AB == true)
				{
					stageData.Change();
				}

				if (AB == true)
				{
					nFlag = stageData.stage.blocks[i].type == BlockType::DONT_MOVE_BLOCK /*|| stageData.stage.blocks[i].type == BlockType::START*/ || nFlag == false;
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

			if (isTutorial == false)
			{
				stageData.Clip(Input::isKeyTrigger(DIK_SPACE));

				if (Input::isKeyTrigger(DIK_R))
				{
					stageData.Reset();
				}

				if (Input::isKeyTrigger(DIK_B))
				{
					stageData.StepBack();
				}

				if (Input::isKeyTrigger(DIK_E))
				{
					stageData.Change();
				}

				if (Input::isMouseClicking(MOUSE_L))
				{
					if (Input::isMouseClickTrigger(MOUSE_L))
					{
						mousePos = Input::getMousePos();
					}
					mouseVec = Input::getMouseVelocity();
				}

				if (Input::isMouseClicked(MOUSE_L))
				{
					RVector3 warpVec;
					if (fabsf(mouseVec.x) < fabsf(mouseVec.y))
					{
						warpVec = RVector3(0.0f, -mouseVec.y, 0.0f);
						warpVec = warpVec.norm();
					}
					else
					{
						warpVec = RVector3(mouseVec.x, 0.0f, 0.0f);
						warpVec = warpVec.norm();
					}

					XMMATRIX proj = camera->GetMatrixProjection();
					XMMATRIX view = camera->GetMatrixView();
					RV3Colider::Plane xyPlane = RV3Colider::Plane(RVector3(0, 0, -1), -blockSize / 2.0f);
					RV3Colider::Ray cursorRay = RV3Colider::Ray();
					cursorRay = RV3Colider::CalcScreen2WorldRay(mousePos, Raki_WinAPI::window_width, Raki_WinAPI::window_height, proj, view);
					RVector3 cursorPos;
					RV3Colider::ColisionRay2Plane(cursorRay, xyPlane, nullptr, &cursorPos);

					for (size_t i = 0; i < stageData.stage.warpBlock.size(); i++)
					{
						RVector3 cursorHit = cursorPos;
						auto& warpBlock = stageData.stage.blocks[stageData.stage.warpBlock[i]->blockNumber];

						if (cursorHit.x >= warpBlock.pos.x - blockSize / 2.0f && cursorHit.x <= warpBlock.pos.x + blockSize / 2.0f &&
							cursorHit.y >= warpBlock.pos.y - blockSize / 2.0f && cursorHit.y <= warpBlock.pos.y + blockSize / 2.0f)
						{
							stageData.stage.warpBlock[i]->WarpOpen(warpVec);
							warpBlock.type = BlockType::WARP_OPEN_BLOCK;
							if (stageData.stage.warpBlock[i]->gateNumber == (size_t)-1)
							{
								stageData.stage.warpBlock[i]->gateNumber = stageData.stage.openGateCount++;
							}
						}
					}
				}
			}

			if (player.goalFlag)
			{
				scene = Scene::GAME_CLEAR;

				player.goalFlag = false;
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

			// オブジェクト描画
			NY_Object3DManager::Get()->SetCommonBeginDrawObject3D();
			if (isTutorial == false)
			{
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
