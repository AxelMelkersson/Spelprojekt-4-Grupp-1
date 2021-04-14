#include "stdafx.h"
#include "LevelManager.hpp"

#include "Scene.h"
#include "LevelScene.h"
#include "MainMenuScene.h"

#include "TiledLoader.h"
#include "SpeedrunManager.h"

#include "GameObject.h"

#include "DataManager.h"

LevelManager::LevelManager()
{
	myTiledLoader = std::make_shared<TiledLoader>();
#ifndef _RETAIL
	myImGuiIsActive = {};
#endif //RETAIL

	myLoadedLevel = 0;
	myLastDoorType = 1;

	myLevelTransition = false;
	myLoadingHiddenRoom = false;
	
	Subscribe(eMessageType::ResetSaveFile);
	Subscribe(eMessageType::LoadNext);
	Subscribe(eMessageType::LoadPrevious);
	Subscribe(eMessageType::LoadHiddenRoom);
	Subscribe(eMessageType::LoadMainRoom);
}

void LevelManager::Init(Scene* aMainMenuScene, Scene* aLevelSelect, Scene* aLevelScene, Scene* anIntroLogosScene, Scene* aWinScene, Scene* aSpeedrunScene)
{
	myScenes.insert({ eScenes::MainMenu, aMainMenuScene });
	myScenes.insert({ eScenes::LevelSelect, aLevelSelect });
	myScenes.insert({ eScenes::LevelScene, aLevelScene });
	myScenes.insert({ eScenes::SpeedrunScene, aSpeedrunScene });
	myScenes.insert({ eScenes::IntroLogos, anIntroLogosScene});
	myScenes.insert({ eScenes::WinScene, aWinScene });

	mySpeedrunManager = std::make_shared<SpeedrunManager>();
	mySpeedrunManager->SetIsSpeedrun(false);
	
	DataManager::GetInstance().ParseCollectableInfo();
}

void LevelManager::Update()
{
	if (myLevelTransition)
	{
		LevelScene* levelScene = dynamic_cast<LevelScene*>(myScenes[eScenes::LevelScene]);
		levelScene->Transitioning();
		levelScene->IncreaseBlackScreen(1.0f);

		if (levelScene->GetReachedFullOpacity())
		{
			myLevelTransition = false;
			SingleLoadScene(eScenes::LevelScene);
		}
	}

#ifndef _RETAIL
	ImGuiUpdate();
#endif //RETAIL
}

#ifndef _RETAIL
void LevelManager::ImGuiUpdate()
{
	if (myImGuiIsActive)
	{
		bool levelManager = true;
		ImGui::Begin("Level Manager", &levelManager, ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::InputInt("Scene Path", &myLoadedLevel, ImGuiWindowFlags_AlwaysAutoResize);

		if (ImGui::Button("Load Scene"))
		{
			SingleLoadScene(eScenes::LevelScene);
		}
		if (ImGui::Button("MainMenu"))
		{
			SingleLoadScene(eScenes::MainMenu);
		}
		if (ImGui::Button("Reset SaveFile"))
		{
			DataManager::GetInstance().ResetSaveFile();
		}

		ImGui::End();
	}
}
void LevelManager::ToggleImGui()
{
	myImGuiIsActive = !myImGuiIsActive;
}
#endif //RETAIL

void LevelManager::SingleLoadScene(eScenes aScene)
{
	UnloadAllScenes();

	AdditiveLoadScene(aScene);
}

void LevelManager::AdditiveLoadScene(eScenes aScene)
{
	if (!myScenes[aScene]->IsLoaded())
	{
		myScenes[aScene]->Load();
		myScenes[aScene]->Activate();
	}
}

void LevelManager::UnloadScene(eScenes aScene)
{
	if (myScenes[aScene]->IsLoaded())
	{
		myScenes[aScene]->Unload();
	}
}

void LevelManager::ReloadScene(eScenes aScene)
{
	UnloadScene(aScene);
	AdditiveLoadScene(aScene);
}

void LevelManager::UnloadAllScenes()
{
	for (int scene = 0; scene < static_cast<int>(eScenes::Count); ++scene)
	{
		eScenes currentScene = static_cast<eScenes>(scene);
		UnloadScene(currentScene);
	}
}

const bool LevelManager::GetIsActive(eScenes aScene)
{
	return myScenes[aScene]->IsActive();
}

void LevelManager::LoadLevel(LevelScene* aLevelScene, GameObject* aPlayer)
{
	myTiledLoader->Load(aLevelScene, myLoadedLevel, aPlayer, myLoadingHiddenRoom);
	DataManager::GetInstance().SaveStartLevel(myLoadedLevel);
}

void LevelManager::LoadLevel(LevelScene* aLevelScene, const int& aLevelIndex, GameObject* aPlayer)
{
	myLoadedLevel = aLevelIndex;
	myTiledLoader->Load(aLevelScene, aLevelIndex, aPlayer, myLoadingHiddenRoom);
}

void LevelManager::SetLevelIndex(const int& aLevelIndex)
{
	myLoadedLevel = aLevelIndex;
}

void LevelManager::UsedLevelSelect()
{
	myTiledLoader->UsedLevelSelect();
}

void LevelManager::Continued()
{
	myTiledLoader->Continued();
}

void LevelManager::Notify(const Message& aMessage)
{
	if (aMessage.myMessageType == eMessageType::LoadNext)
	{
		++myLoadedLevel;
		if (myLoadedLevel >= DataManager::GetInstance().GetLevelCount())
		{
			myLoadedLevel = DataManager::GetInstance().GetLevelCount() - 1;
			SingleLoadScene(eScenes::WinScene);
			return;
		}

		myLastDoorType = std::get<int>(aMessage.myData);

		myLevelTransition = true;
	}
	else if (aMessage.myMessageType == eMessageType::LoadPrevious)
	{
		--myLoadedLevel;
		if (myLoadedLevel < 0)
		{
			myLoadedLevel = 0;
		}

		myLastDoorType = std::get<int>(aMessage.myData);

		myLevelTransition = true;
	}
	else if (aMessage.myMessageType == eMessageType::LoadHiddenRoom)
	{
		myLoadingHiddenRoom = true;

		myLastDoorType = std::get<int>(aMessage.myData);

		myLevelTransition = true;
	}
	else if (aMessage.myMessageType == eMessageType::LoadMainRoom)
	{
		myLoadingHiddenRoom = false;

		myLastDoorType = std::get<int>(aMessage.myData);

		myLevelTransition = true;
	}
	else if (aMessage.myMessageType == eMessageType::ResetSaveFile)
	{
		myLoadedLevel = 0;
	}
}

const int& LevelManager::GetDoorType()
{
	return myLastDoorType;
}
