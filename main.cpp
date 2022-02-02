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
	Tutorial,
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
	RVector3 eye = { 0.0f, 0.0f, -250.0f };
	RVector3 target = { 0.0f, 0.0f, 0.0f };
	RVector3 up = { 0.0f, 1.0f, 0.0f };
	cam->SetViewStatusEyeTargetUp(eye, target, up);

	//音
	Audio::Init();

	SoundData titleAndSelectBGM;
	SoundData gameBGM;
	SoundData decisionSE;
	SoundData noDecisionSE;
	SoundData clearSE;

	titleAndSelectBGM = Audio::LoadSound_wav("./Resources/Sound/TitleAndSelectBack.wav");
	gameBGM = Audio::LoadSound_wav("./Resources/Sound/GameBack.wav");
	decisionSE = Audio::LoadSound_wav("./Resources/Sound/DecisionSE.wav");
	noDecisionSE = Audio::LoadSound_wav("./Resources/Sound/no.wav");
	clearSE = Audio::LoadSound_wav("./Resources/Sound/ClearSE.wav");

#pragma endregion GameValue

	FPS::Get()->Start();

	GameCommonData::StageBlockModels::StaticInitiizer();

	/*背景*/
	UINT backgroundGraph = TexManager::LoadTexture("./Resources/background.jpg");
	UINT flontgroundGraph = TexManager::LoadTexture("./Resources/background.png");
	Sprite background;
	Sprite flontground;
	background.CreateSprite({ (float)Raki_WinAPI::window_width, (float)Raki_WinAPI::window_height }, { 0.0f, 0.0f }, backgroundGraph, true, nullptr);
	flontground.CreateSprite({ (float)Raki_WinAPI::window_width, (float)Raki_WinAPI::window_height }, { 0.0f, 0.0f }, backgroundGraph, true, nullptr);

	/*カーソル*/
	UINT cursorGraph = TexManager::LoadTexture("./Resources/cursor.png");
	Sprite cursor;
	cursor.CreateSprite({ 320.0f, 64.0f }, { 0.5f, 0.5f }, cursorGraph, false, nullptr);

	/*タイトル*/
	uvAnimData titleAnime;
	const size_t titleDiv = 1;
	UINT titleGraph = TexManager::LoadDivTextureTest(&titleAnime, "./Resources/Title.png", (int)titleDiv, 1280);
	Sprite title;
	title.CreateSprite({ (float)Raki_WinAPI::window_width, (float)Raki_WinAPI::window_height }, { 0.0f, 0.0f }, titleGraph, true, &titleAnime);

	/*ステージセレクト*/
	UINT selectGraph = TexManager::LoadTexture("./Resources/STAGESELECT.jpg");
	Sprite select;
	select.CreateSprite({ (float)Raki_WinAPI::window_width, (float)Raki_WinAPI::window_height }, { 0.0f, 0.0f }, selectGraph, true, nullptr);
	const int selectMap[] = {
		2,3,
		1,4,
		0,5,
	};
	const size_t stageMax = sizeof(selectMap) / sizeof(selectMap[0]) * 3;
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
		stage[i].CreateSprite({ 320.0f, 64.0f }, { 0.5f, 0.5f }, stageGraph[i], false, nullptr);
		stage[i].spdata.position.x = (float)Raki_WinAPI::window_width / 2.0f;
		stage[i].spdata.position.y = (float)Raki_WinAPI::window_height / 2.0f;
		stage[i].UpdateSprite();
	}
	UINT arrowLGraph = TexManager::LoadTexture("./Resources/Arrow_L.png");
	Sprite arrowL;
	arrowL.CreateSprite({ 128.0f, 128.0f }, { 0.0f, 0.5f }, arrowLGraph, false, nullptr);
	arrowL.spdata.position.x = 200.0f;
	arrowL.spdata.position.y = (float)Raki_WinAPI::window_height / 2.0f;
	arrowL.UpdateSprite();
	UINT arrowRGraph = TexManager::LoadTexture("./Resources/Arrow_R.png");
	Sprite arrowR;
	arrowR.CreateSprite({ 128.0f, 128.0f }, { 1.0f, 0.5f }, arrowRGraph, false, nullptr);
	arrowR.spdata.position.x = Raki_WinAPI::window_width - 200.0f;
	arrowR.spdata.position.y = (float)Raki_WinAPI::window_height / 2.0f;
	arrowR.UpdateSprite();
	bool isLoad = false;

	/*チュートリアル*/
	UINT tutorialGraph[] =
	{
		TexManager::LoadTexture("./Resources/Rule.jpg"),
	};
	Sprite tutorial[sizeof(tutorialGraph) / sizeof(tutorialGraph[0])];
	for (size_t i = 0; i < sizeof(tutorialGraph) / sizeof(tutorialGraph[0]); i++)
	{
		tutorial[i].CreateSprite({ (float)Raki_WinAPI::window_width, (float)Raki_WinAPI::window_height }, { 0.0f, 0.0f }, tutorialGraph[i], false, nullptr);
	}
	int tutorialCount = 0;
	bool isTutorial = true;

	/*ステージ番号*/
	UINT stageNoGraph[] =
	{
		TexManager::LoadTexture("./Resources/stageNo1.jpg"),
		TexManager::LoadTexture("./Resources/stageNo2.jpg"),
		TexManager::LoadTexture("./Resources/stageNo3.jpg"),
		TexManager::LoadTexture("./Resources/stageNo4.jpg"),
		TexManager::LoadTexture("./Resources/stageNo5.jpg"),
		TexManager::LoadTexture("./Resources/stageNo6.jpg"),
		TexManager::LoadTexture("./Resources/stageNo7.jpg"),
		TexManager::LoadTexture("./Resources/stageNo8.jpg"),
		TexManager::LoadTexture("./Resources/stageNo9.jpg"),
		TexManager::LoadTexture("./Resources/stageNo10.jpg"),
		TexManager::LoadTexture("./Resources/stageNo11.jpg"),
		TexManager::LoadTexture("./Resources/stageNo12.jpg"),
		TexManager::LoadTexture("./Resources/stageNo13.jpg"),
		TexManager::LoadTexture("./Resources/stageNo14.jpg"),
		TexManager::LoadTexture("./Resources/stageNo15.jpg"),
		TexManager::LoadTexture("./Resources/stageNo16.jpg"),
		TexManager::LoadTexture("./Resources/stageNo17.jpg"),
		TexManager::LoadTexture("./Resources/stageNo18.jpg"),
	};
	Sprite stageNo[stageMax];
	for (size_t i = 0; i < stageMax; i++)
	{
		stageNo[i].CreateSprite({ 200.0f, 32.0f }, { 0.5f, 0.5f }, stageNoGraph[i], false, nullptr);
	}

	/*ステージクリア*/
	UINT ClearGraph = TexManager::LoadTexture("./Resources/clear.png");
	Sprite Clear;
	bool stageClearFlag[stageCount];
	size_t clearCount = 0;
	for (size_t i = 0; i < stageCount; i++)
	{
		stageClearFlag[i] = false;
	}
	int clearSelect = 0;

	/*走査線*/
	Sprite mpSprite;
	mpSprite.CreateSprite({ 1280.0f / 2.0f, 720.0f / 2.0f }, { 0.0f, 0.0f }, 0, false, nullptr);

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
	bool soundFlag = false;

	while (true)  // ゲームループ
	{
		if (rakiWinApp->ProcessMessage()) { break; }

		//更新

		Input::StartGetInputState();

		switch (scene)
		{
		case TITLE:
			if (soundFlag == false)
			{
				Audio::PlayLoadedSound(titleAndSelectBGM);
			}
			soundFlag = true;
			titleAnime.usingNo++;
			if (titleAnime.usingNo >= titleDiv)
			{
				titleAnime.usingNo = 0;
			}

			if (Input::isKeyTrigger(DIK_SPACE))
			{
				Audio::PlayLoadedSound(decisionSE);
				soundFlag = false;
				scene = Scene::SELECT;
			}

			title.UpdateSprite();

			// 描画開始
			Raki_DX12B::Get()->StartDraw();

			// スプライト描画
			SpriteManager::Get()->SetCommonBeginDraw();
			title.Draw();

			// 描画終了
			Raki_DX12B::Get()->EndDraw();
			break;
		case SELECT:
			if (Input::isKeyTrigger(DIK_W))
			{
				stageNumber--;

				if (stageNumber < 0)
				{
					stageNumber = 0;
				}
			}
			if (Input::isKeyTrigger(DIK_S))
			{
				static const size_t openStageCount = 3;

				if (stageNumber < openStageCount + clearCount - 1)
				{
					stageNumber++;
					Audio::PlayLoadedSound(noDecisionSE);
				}
				else
				{
					Audio::PlayLoadedSound(decisionSE);
				}

				if (stageNumber >= stageCount)
				{
					stageNumber = stageCount - 1;
				}
			}
			if (Input::isKeyTrigger(DIK_A))
			{
				stageNumber--;

				if (stageNumber < 0)
				{
					stageNumber = 0;
					Audio::PlayLoadedSound(noDecisionSE);
				}
				else
				{
					Audio::PlayLoadedSound(decisionSE);
				}
			}
			if (Input::isKeyTrigger(DIK_D))
			{
				static const size_t openStageCount = 3;

				if (stageNumber < openStageCount + clearCount - 1)
				{
					stageNumber++;

					Audio::PlayLoadedSound(decisionSE);
				}
				else
				{
					Audio::PlayLoadedSound(noDecisionSE);
				}

				if (stageNumber >= stageCount)
				{
					stageNumber = stageCount - 1;
				}
			}

			if (Input::isKeyTrigger(DIK_SPACE))
			{
				Audio::StopLoadedSound(titleAndSelectBGM);
				Audio::PlayLoadedSound(gameBGM);

				player.Tposition = RVector3(0.0f, 0.0f, 0.0f);

				isLoad = true;
			}

			if (isLoad)
			{
				scene = Scene::Tutorial;
				isLoad = false;

				switch (stageNumber)
				{
				case 0:
					stageData.Select("stage1.boxmap", true);
					break;
				case 1:
					stageData.Select("stage2.boxmap", true);
					break;
				case 2:
					stageData.Select("stage3.boxmap", true);
					break;
				case 3:
					stageData.Select("stage4.boxmap", true);
					break;
				case 4:
					stageData.Select("stage5.boxmap", true);
					break;
				case 5:
					stageData.Select("stage6.boxmap", true);
					break;
				case 6:
					stageData.Select("stage7.boxmap", true);
					break;
				case 7:
					stageData.Select("stage8.boxmap", true);
					break;
				case 8:
					stageData.Select("stage9.boxmap", true);
					break;
				case 9:
					stageData.Select("stage10.boxmap", true);
					break;
				case 10:
					stageData.Select("stage11.boxmap", true);
					break;
				case 11:
					stageData.Select("stage12.boxmap", true);
					break;
				case 12:
					stageData.Select("stage13.boxmap", true);
					break;
				case 13:
					stageData.Select("stage14.boxmap", true);
					break;
				case 14:
					stageData.Select("stage15.boxmap", true);
					break;
				case 15:
					stageData.Select("stage16.boxmap", true);
					break;
				case 16:
					stageData.Select("stage17.boxmap", true);
					break;
				case 17:
					stageData.Select("stage18.boxmap", true);
					break;
				default:
					stageData.Select("stage18.boxmap", true);
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
		case Tutorial:
			player.playerOldPos = player.position;
			player.playerCollision.Update(player.Tposition);
			stageData.Update();

			if (Input::isKeyTrigger(DIK_SPACE))
			{
				scene = GAME_MAIN;
			}

			// 描画開始
			RAKI_DX12B_GET->StartDraw();

			SpriteManager::Get()->SetCommonBeginDraw();
			tutorial[tutorialCount].Draw();

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
			player.Update();
			stageData.Update();

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
						stageData.stage.ChangeSwitchModel(&StageBlockModels::switchOnModel);
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
					//player.object->position = Rv3Ease::InQuad(player.endPos, player.startPos, player.timeRate) + player.offsetPos;
					player.object->position = player.startPos + player.offsetPos;
					player.position = player.startPos;
					player.playerFlag = false;
				}
				else
				{
					player.playerFlag = true;
				}

				if (actFlag == true && actnFlag == true)
				{
					//player.object->position = player.playerOldPos + player.offsetPos;
				   // player.object->position = Rv3Ease::InQuad(player.endPos, player.startPos, player.timeRate) + player.offsetPos;
					player.object->position = player.startPos + player.offsetPos;
					player.position = player.startPos;
					player.playerFlag = false;
				}
				else
				{
					player.playerFlag = true;
				}

				if (player.moveFlag == false)
				{
					stageData.Clip(Input::isKeyTrigger(DIK_SPACE) && stageData.clipFlag == false && stageData.backFlag == false);

					if (Input::isKeyTrigger(DIK_R))
					{
						doorFlag = false;
						stageData.Reset();
					}

					if (Input::isKeyTrigger(DIK_B) == true && stageData.backFlag == false && stageData.clipFlag == false)
					{
						stageData.backFlag = true;
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
					Audio::PlayLoadedSound(clearSE);
					Audio::StopLoadedSound(gameBGM);
					Audio::PlayLoadedSound(titleAndSelectBGM);

					scene = Scene::GAME_CLEAR;

					if (stageClearFlag[stageNumber] == false)
					{
						stageClearFlag[stageNumber] = true;
						clearCount++;
					}

					clearSelect = 0;
					player.goalFlag = false;
				}
			}

			NY_Object3DManager::Get()->UpdateAllObjects();

			// 描画開始
			Raki_DX12B::Get()->StartDrawRenderTarget();

			// 背景描画
			SpriteManager::Get()->SetCommonBeginDraw();
			background.Draw();

			NY_Object3DManager::Get()->SetCommonBeginDrawObject3D();
			// オブジェクト描画
			player.Draw();
			stageData.Draw();

			// パーティクル描画
			StageMoveParticle::Get()->Draw();
			SeaParticle::Get()->Draw();

			Raki_DX12B::Get()->StartDrawBackbuffer();

			// 前景描画
			mpSprite.DrawMPRender();
			SpriteManager::Get()->SetCommonBeginDraw();
			flontground.Draw();

			// 描画終了
			Raki_DX12B::Get()->CloseDraw();
			break;
		case GAME_OVER:
			scene = Scene::SELECT;
			break;
		case GAME_CLEAR:
		{
			stage[0].spdata.position = {
				(float)Raki_WinAPI::window_width / 2.0f - 150.0f,
				(float)Raki_WinAPI::window_height / 2.0f, 0.0f };
			stage[1].spdata.position = {
				(float)Raki_WinAPI::window_width / 2.0f + 150.0f,
				(float)Raki_WinAPI::window_height / 2.0f, 0.0f };

			if ((stageNumber + 1Ui64) >= stageMax)
			{
				clearSelect = 1;
			}

			if (Input::isKeyTrigger(DIK_A))
			{
				clearSelect--;

				if (clearSelect < 0)
				{
					clearSelect = 0;
				}
				if ((stageNumber + 1Ui64) >= stageMax && clearSelect < 1)
				{
					clearSelect = 1;
				}
			}
			if (Input::isKeyTrigger(DIK_D))
			{
				clearSelect++;

				if (clearSelect > 1)
				{
					clearSelect = 1;
				}
			}

			cursor.spdata.position = {
				(float)Raki_WinAPI::window_width / 2.0f - 150.0f + 300.0f * clearSelect,
				(float)Raki_WinAPI::window_height / 2.0f, 0.0f };

			if (Input::isKeyTrigger(DIK_SPACE))
			{
				if (clearSelect == 0)
				{
					scene = Scene::SELECT;
					isLoad = true;
					stageNumber++;
				}
				else if (clearSelect == 1)
				{
					scene = Scene::SELECT;

					stage[0].spdata.position = {
						(float)Raki_WinAPI::window_width / 2.0f,
						(float)Raki_WinAPI::window_height / 2.0f, 0.0f };
					stage[1].spdata.position = {
						(float)Raki_WinAPI::window_width / 2.0f,
						(float)Raki_WinAPI::window_height / 2.0f, 0.0f };

				}
			}

			stage[0].UpdateSprite();
			stage[1].UpdateSprite();
			cursor.UpdateSprite();

			// 描画開始
			Raki_DX12B::Get()->StartDraw();

			// スプライト描画
			SpriteManager::Get()->SetCommonBeginDraw();
			stage[0].Draw();
			stage[1].Draw();
			cursor.Draw();

			// 描画終了
			Raki_DX12B::Get()->EndDraw();

			break;
		}
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
