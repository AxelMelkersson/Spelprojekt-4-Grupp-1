// Basics
#include "stdafx.h"
#include "GameWorld.h"

// Core
#include "Game.h"
#include "GameObject.h"
#include "InputWrapper.h"
#include "AudioManager.h"

// Scene
#include "Scene.h"
#include "LevelScene.h"

// I/O
#include "Keys.h"
#include <math.h>
#include "../External/Headers/CU/Utilities.h"
#include "CutsceneManager.h"

CGameWorld* CGameWorld::ourInstance = nullptr;

CGameWorld::CGameWorld(CGame* aGame)
	: myGame(aGame)
	, myDeltaTime(0.0f)
	, myTotalTime(0.0f)
	, myLevelScene(LevelScene())
	, myMainMenuScene(MainMenuScene())
	, myLevelSelect(LevelSelect())
	, myIntroLogosScene(IntroLogosScene())
	, myWinScene(WinScene())
	, mySpeedrunScene(SpeedrunScene())
{
	myTimer = std::make_unique<Utils::Timer>();
	myInput = std::make_shared<InputWrapper>();
	myInput->Init();

	if (!ourInstance)
	{
		ourInstance = this;
	}
}
CGameWorld::~CGameWorld() 
{
	myLevelManager.UnloadAllScenes();
}

void CGameWorld::Init()
{
	AudioManager::GetInstance()->Init();
	myLevelManager.Init(&myMainMenuScene, &myLevelSelect, &myLevelScene, &myIntroLogosScene, &myWinScene, &mySpeedrunScene);
	myLevelManager.SingleLoadScene(LevelManager::eScenes::IntroLogos);

	//myLevelManager.SingleLoadScene(LevelManager::eScenes::MainMenu);
}

void CGameWorld::Update()
{
	myTimer->Update();
	myDeltaTime = myTimer->GetDeltaTime();
	myInput->Update(myDeltaTime);
	AudioManager::GetInstance()->Update(myDeltaTime);

	if (!CutsceneManager::GetInstance().IsPlaying())
	{
		myLevelManager.Update();
		Scene::Manager::Update(myDeltaTime);
	}
	else
	{
		CutsceneManager::GetInstance().Update(myDeltaTime);
	}
}

void CGameWorld::Render()
{
	if (!CutsceneManager::GetInstance().IsPlaying())
	{
		Scene::Manager::Render();
	}
	else
	{
		CutsceneManager::GetInstance().Render();
	}
}

void CGameWorld::LoadDebugger()
{
	
}