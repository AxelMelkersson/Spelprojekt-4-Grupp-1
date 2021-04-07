#include "stdafx.h"
#include "WinScene.hpp"

#include "UIObject.h"

#include "Game.h"
#include "GameWorld.h"
#include "InputWrapper.h"
#include "SpeedrunManager.h"

WinScene::WinScene()
{

}

WinScene::~WinScene()
{

}

void WinScene::Load()
{
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
	winText->Init("Sprites/UI/winScene/win.dds", v2f(64.0f, 16.0f), v2f(160.0f, 90.0f), 201);
	winText->SetPosition(v2f(160.0f, 90.0f));

	UIObject* enterText = new UIObject(this);
	enterText->SetActive(true);
	enterText->Init("Sprites/UI/winScene/enter.dds", v2f(64.0f, 16.0f), v2f(160.0f, 110.0f), 201);
	enterText->SetPosition(v2f(160.0f, 110.0f));

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

	if (CGameWorld::GetInstance()->Input()->GetInput()->GetKeyJustDown(Keys::ENTERKey))
	{
		CGameWorld::GetInstance()->GetLevelManager().SingleLoadScene(LevelManager::eScenes::MainMenu);
	}

	Scene::Update(aDeltaTime);
}