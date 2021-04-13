#include "stdafx.h"
#include "LevelScene.h"

#include "GameWorld.h"

#include "../External/Headers/CU/Utilities.h"

#include "Player.hpp"

#include "InputWrapper.h"

#include "Jesus.hpp"
#include "HiddenArea.hpp"

#include "LevelManager.hpp"
#include "SpeedrunManager.h"
#include "AudioManager.h"

#include "Game.h"

#include "PostMaster.hpp"

#include "BashableObject.hpp"
#include "CameraStaticDistance.hpp"

#include "SpriteComponent.h"

#include "Glide.hpp"

LevelScene::LevelScene()
	: 
	myPlayer(nullptr),
	myBackground(nullptr),
	myIsSpeedrun(false),
	myStayBlackTime(0.2f),
	myEffectFactory(nullptr),
	Scene()
{}

void LevelScene::Load()
{
	myIsSpeedrun = CGameWorld::GetInstance()->GetLevelManager().GetSpeedrunManager()->GetIsSpeedrun();

	myBlackScreenOpacity = 1.0f;
	myBlackScreenOpacitySpeed = 4.3f;
	myStayBlackTime = 0.2f;

	AudioManager::GetInstance()->FadeOut(AudioList::Main_Menu);
	AudioManager::GetInstance()->Stop(AudioList::MenuAmbience);

	myReachedFullOpacity = true;
	myIsTransitioning = false;

	myEffectFactory = new ParticleEffectFactory(this);
	myEffectFactory->StartFirefliesEffects();
	//myEffectFactory->StartRainEffects();

	myPlayer = new Player(this);

	myBackground = new Background(this);

	CGameWorld::GetInstance()->GetLevelManager().LoadLevel(this, myPlayer);

	AddBlackScreen();

	myPopUp = new UIPopUp(this);
	myPopUp->InitPopUp();

	myPauseMenu = new PauseMenu(this);
	myPauseMenu->InitMenu();

	if (myIsSpeedrun)
	{
		myTimer = new Timer(this);
		myTimer->Init({ 10, 13 });
		myTimer->Start(CGameWorld::GetInstance()->GetLevelManager().GetSpeedrunManager()->GetScore());
	}

	Scene::Load();
}

void LevelScene::Unload()
{
	delete myPauseMenu;

	AudioManager::GetInstance()->FadeOut(AudioList::Forest_Theme);
	AudioManager::GetInstance()->FadeOut(AudioList::Village_Theme);
	AudioManager::GetInstance()->FadeOut(AudioList::Castle_Theme);
	AudioManager::GetInstance()->FadeOut(AudioList::Finale);
	AudioManager::GetInstance()->FadeOut(AudioList::Crows);
	AudioManager::GetInstance()->FadeOut(AudioList::Castle);
	AudioManager::GetInstance()->FadeOut(AudioList::Wind);
	AudioManager::GetInstance()->StopAllSounds();

	if (myIsSpeedrun)
	{
		CGameWorld::GetInstance()->GetLevelManager().GetSpeedrunManager()->SetScore(myTimer->GetTime());
		myTimer->Stop();
	}

	Scene::Unload();
}

void LevelScene::Activate()
{
	Scene::Activate();

	GetCamera().StartFollowing(myPlayer, { 40.0f, 40.0f });
}

void LevelScene::Deactivate()
{
	if (!myReachedFullOpacity)
	{
		IncreaseBlackScreen(1.0f);
		return;
	}

	Scene::Deactivate();

	GetCamera().StopFollowing();
}

void LevelScene::Update(const float& aDeltaTime)
{/*
	if (myEffectFactory != NULL)
		myEffectFactory->SpawnEffect(myPlayer->GetPosition(), eParticleEffects::TrailEffect2);*/

	/*if (CGameWorld::GetInstance()->Input()->GetInput()->GetKeyJustDown(Keys::LeftMouseButton))
	{
		v2f position = GetPlayer()->GetPosition();

		myEffectFactory->TestEffect(position);
	}*/
	//else if (CGameWorld::GetInstance()->Input()->GetInput()->GetKeyJustDown(Keys::RightMouseButton))
	//{
	//	myEffectFactory->TestEffectFollowObject();
	//}

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

	if (myIsTransitioning)
	{
		return;
	}

	myBlackScreen->SetPosition(v2f(myPlayer->GetPositionX(), myPlayer->GetPositionY()));

	if (myReachedFullOpacity)
	{
		DecreaseBlackScreen();
	}

	myPauseMenu->Update(aDeltaTime);

	myPopUp->Update(aDeltaTime);

	if (myPauseMenu->IsPauseActive() == false && myPauseMenu->GetOptionsIsActive() == false)
		Scene::Update(aDeltaTime);
	else if (myIsSpeedrun)
		myTimer->Update(aDeltaTime);
}

void LevelScene::AddBlackScreen()
{
	myBlackScreen = new GameObject(this);
	myBlackScreen->SetZIndex(1000);

	myBlackScreen->SetPivot(v2f(0.5f, 0.5f));

	myBlackScreen->SetPosition(v2f(myPlayer->GetPositionX(), myPlayer->GetPositionY()));

	SpriteComponent* sprite = myBlackScreen->AddComponent<SpriteComponent>();
	sprite->SetSpritePath("Sprites/BlackScreen.dds");
	sprite->SetSize(v2f(10000.0f, 10000.0f));
}

void LevelScene::DecreaseBlackScreen()
{
	if (myStayBlackTime > 0)
	{
		myStayBlackTime -= CGameWorld::GetInstance()->GetTimer()->GetDeltaTime();
		return;
	}

	myBlackScreen->GetComponent<SpriteComponent>()->SetColor(v4f(1.0f, 1.0f, 1.0f, myBlackScreenOpacity));
	myBlackScreenOpacity -= CGameWorld::GetInstance()->GetTimer()->GetDeltaTime() * myBlackScreenOpacitySpeed;

	if (myBlackScreenOpacity <= 0.0f)
	{
		myReachedFullOpacity = false;
	}
}

void LevelScene::IncreaseBlackScreen(const float& aOpacitySpeedFactor)
{
	myBlackScreen->GetComponent<SpriteComponent>()->SetColor(v4f(1.0f, 1.0f, 1.0f, myBlackScreenOpacity));
	myBlackScreenOpacity += CGameWorld::GetInstance()->GetTimer()->GetDeltaTime() * myBlackScreenOpacitySpeed * aOpacitySpeedFactor;

	if (myBlackScreenOpacity >= 1.0f)
	{
		myReachedFullOpacity = true;
	}
}

const bool LevelScene::GetReachedFullOpacity()
{
	return myReachedFullOpacity;
}

GameObject* LevelScene::GetPlayer()
{
	return myPlayer;
}

ParticleEffectFactory& LevelScene::GetEffectFactory()
{
	return *myEffectFactory;
}

Background& LevelScene::GetBackground()
{
	return *myBackground;
}

void LevelScene::Transitioning()
{
	myIsTransitioning = true;
}