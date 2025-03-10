#include "stdafx.h"
#include "WinScene.hpp"

#include "UIObject.h"

#include "Game.h"
#include "GameWorld.h"
#include "InputWrapper.h"
#include "SpeedrunManager.h"
#include "CutsceneManager.h"
#include "AudioManager.h"

WinScene::WinScene()
{

}

WinScene::~WinScene()
{

}

void WinScene::Load()
{
	myFadeTime = 3.0f;

	AudioManager::GetInstance()->FadeIn(AudioList::Main_Menu);
	if (CGameWorld::GetInstance()->GetLevelManager().GetSpeedrunManager()->GetIsSpeedrun())
	{
		const std::shared_ptr<SpeedrunManager> speedrunManager = CGameWorld::GetInstance()->GetLevelManager().GetSpeedrunManager();
		const std::string speedrunTime = speedrunManager->GetTimeOutput(speedrunManager->GetScore());

		UIText* winText = new UIText(this);
		winText->Activate();

		std::string text;
		float score = speedrunManager->GetScore();
		float bestHighscore = speedrunManager->GetBestHighscore();

		if (score < bestHighscore || bestHighscore < 0.02)
		{
			text = "New Highscore: " + speedrunTime;
		}
		else
		{
			text = "Time: " + speedrunTime;
		}

		winText->Init(text, "Text/Peepo.ttf", EFontSize::EFontSize_72, 0);

		if (score < bestHighscore || bestHighscore < 0.02)
		{
			winText->SetPosition({ 70.0f, 100.0f });
		}
		else
		{
			winText->SetPosition({ 100.0f, 100.0f });
		}
		speedrunManager->ReportScoreToHighscores();
	}
	else
	{
		myFadeTime = 0.0f;
		CGameWorld::GetInstance()->GetLevelManager().GetSpeedrunManager()->Unlock();

		UIObject* winCutscene = new UIObject(this);
		winCutscene->SetActive(true);
		winCutscene->Init("Video/cutscene_ending_320x180px.dds", { 512.f, 256.f }, v2f(0, 0), 201);

		CutsceneManager::GetInstance().PlayVideo(CutsceneType::Outro);
	}

	Scene::Load();
}

void WinScene::Update(const float& aDeltaTime)
{
	const float zoomX = CGameWorld::GetInstance()->Game()->GetZoomX();
	const float zoomY = CGameWorld::GetInstance()->Game()->GetZoomY();

	constexpr float aspectRatioX = 16.0f;
	constexpr float aspectRatioY = 9.0f;
	constexpr float sizeX = 1920.0f;
	constexpr float sizeY = 1080.0f;

	float zoomFactor = 1.0f;
	if (zoomX / aspectRatioX < zoomY / aspectRatioY)
	{
		zoomFactor = zoomX / sizeX;
	}
	else
	{
		zoomFactor = zoomY / sizeY;
	}

	const float zoom = 6.0f * zoomFactor;

	GetCamera().SetZoom(zoom);

	if (!CutsceneManager::GetInstance().IsPlaying())
	{
		myFadeTime -= aDeltaTime;
		if (myFadeTime <= 0)
		{
			CGameWorld::GetInstance()->GetLevelManager().SingleLoadScene(LevelManager::eScenes::MainMenu);
		}
	}

	Scene::Update(aDeltaTime);
}