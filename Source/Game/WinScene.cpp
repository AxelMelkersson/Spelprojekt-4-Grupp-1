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
	AudioManager::GetInstance()->FadeIn(AudioList::Main_Menu);
	if (CGameWorld::GetInstance()->GetLevelManager().GetSpeedrunManager()->GetIsSpeedrun())
	{
		CGameWorld::GetInstance()->GetLevelManager().GetSpeedrunManager()->ReportScoreToHighscores();
	}
	else
	{
		CGameWorld::GetInstance()->GetLevelManager().GetSpeedrunManager()->Unlock();
	}
	UIObject* winText = new UIObject(this);
	winText->SetActive(true);
	winText->Init("Video/cutscene_ending_320x180px.dds", { 512.f, 256.f }, v2f(0, 0), 201);

	CutsceneManager::GetInstance().PlayVideo(CutsceneType::Outro);

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

	if (CGameWorld::GetInstance()->Input()->GetInput()->GetKeyJustDown(Keys::ENTERKey) || CGameWorld::GetInstance()->Input()->GetController()->IsButtonPressed(Controller::Button::Cross))
	{
		CGameWorld::GetInstance()->GetLevelManager().SingleLoadScene(LevelManager::eScenes::MainMenu);
	}

	Scene::Update(aDeltaTime);
}