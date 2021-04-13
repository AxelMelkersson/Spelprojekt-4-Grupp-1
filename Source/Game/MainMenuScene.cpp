#include "stdafx.h"
#include "MainMenuScene.h"

#include "UIObject.h"
#include "UIButton.h"
#include "OptionsMenu.h"

#include "LevelManager.hpp"
#include "SpeedrunManager.h"
#include "DataManager.h"
#include "InputWrapper.h"

#include "CutsceneManager.h"
#include "AudioManager.h"
#include "OptionsMenu.h"

#include "Game.h"

#include "SpriteComponent.h"
#include "AnimationComponent.hpp"


MainMenuScene::MainMenuScene()
{
	myMovingIndex = {};
}

void MainMenuScene::Load()
{

	myLevelSelectUnlocked = DataManager::GetInstance().GetBonfireState(0);
	mySpeedrunUnlocked = CGameWorld::GetInstance()->GetLevelManager().GetSpeedrunManager()->GetIsUnlocked();

	myButtons.clear();
	myMovingIndex = {};

	CGameWorld::GetInstance()->GetLevelManager().GetSpeedrunManager()->SetIsSpeedrun(false);
	myInput = CGameWorld::GetInstance()->Input();
	AudioManager::GetInstance()->PlayIfAvailable(AudioList::MenuAmbience);
	AudioManager::GetInstance()->PlayIfAvailable(AudioList::Main_Menu);
	AudioManager::GetInstance()->FadeIn(AudioList::Main_Menu);

	InitObjects();

	Scene::Load();

}

void MainMenuScene::Activate()
{
	Scene::Activate();

	myOptions->DeactivateCredits();
	myOptions->DeactivateTutorial();
}

void MainMenuScene::Deactivate()
{
	Scene::Deactivate();
}

void MainMenuScene::Update(const float& aDeltaTime)
{
	const float zoomX = CGameWorld::GetInstance()->Game()->GetZoomX();
	const float zoomY = CGameWorld::GetInstance()->Game()->GetZoomY();

	float zoomFactor = 1.0f;
	if (zoomX / 16.0f < zoomY / 9.0f)
	{
		zoomFactor = zoomX / 1920.0f;
	}
	else
	{
		zoomFactor = zoomY / 1080.0f;
	}

	const float zoom = 6.0f * zoomFactor;

	GetCamera().SetZoom(zoom);

	GetCamera().SetBounds(v2f(0.0f, 0.0f), v2f(1920.0f, 1080.0f));

	Scene::Update(aDeltaTime);

	UpdateObjects(aDeltaTime);

	if (!mySubMenuActive)
		CheckButtonsPress();
	else
		myOptions->Update(aDeltaTime);
	
}

void MainMenuScene::InitObjects()
{
	myBackground = new UIObject(this);
	myTitleSprite = new UIObject(this);
	myFireHighlight = new UIObject(this);
	myNewGameBtn = new UIButton(this);
	mySpeedrunModeBtn = new UIButton(this);
	myLevelSelectBtn = new UIButton(this);
	myOptionsBtn = new UIButton(this);
	myExitGameBtn = new UIButton(this);

	v2f backgroundPos = { 0.f, 0.f };
	v2f titleSpritePos = { 101.f, 16.f };
	
	v2f newGameBtnPos = {210.f, 80.f};
	v2f levelSelectBtnPos = { 210.f, 100.f };
	v2f speedrunModeBtnPos = { 210.f, 120.f };
	v2f optionsBtnPos = {210.f, 140.f};
	v2f exitGameBtnPos = {210.f, 160.f};
	
	myOptions = new OptionsMenu(this);
	myOptions->Init();
	mySubMenuActive = false;

	std::string playSpritePath;
	std::string playSpritePathAnim;
	v2f playPos;

	if (myLevelSelectUnlocked)
	{
		playSpritePath = "Sprites/UI/startMenu/UI_StartMenu_Text_Continue_64x16px_Unmarked.dds";
		playSpritePathAnim = "Sprites/UI/startMenu/UI_StartMenu_Text_Continue_64x16px_Marked.dds";
		playPos = { 64.f, 16.f };
	}
	else
	{
		playSpritePath = "Sprites/UI/startMenu/UI_StartMenu_Text_NewGame_56x16px_unmarked.dds";
		playSpritePathAnim = "Sprites/UI/startMenu/UI_StartMenu_Text_NewGame_56x16px_marked.dds";
		playPos = { 56.f,16.f };
	}

	myBackground->Init("Sprites/UI/startMenu/UI_startMenu_Background_320x180px.dds", { 520.f, 265.f }, backgroundPos, 200);
	myTitleSprite->Init("Sprites/UI/startMenu/UI_startMenu_logotype_A_192x64px.dds", { 192.0f, 64.0f }, titleSpritePos, 201);
	myFireHighlight->InitAnimation("Sprites/UI/pauseMenu/UI_PauseMenu_Flame_16x16px.dds", { 16.0f, 16.0f }, 8, 8, { 200.0f, 70.0f }, 201);

	myNewGameBtn->Init(playSpritePath, playPos, newGameBtnPos, playSpritePathAnim, playPos.x);
	myLevelSelectBtn->Init("Sprites/UI/startMenu/UI_StartMenu_Text_LevelSelect_Unmarked_72x16px.dds", { 72.f,16.f }, levelSelectBtnPos, "Sprites/UI/startMenu/UI_StartMenu_Text_LevelSelect_Marked_72x16px.dds", 72, "Sprites/UI/startMenu/UI_StartMenu_Text_LevelSelect_80x9px_Locked.dds", myLevelSelectUnlocked);
	mySpeedrunModeBtn->Init("Sprites/UI/startMenu/UI_StartMenu_Text_Speedrun_57x16px_Unmarked.dds", { 57.f,16.f }, speedrunModeBtnPos, "Sprites/UI/startMenu/UI_StartMenu_Text_Speedrun_57x16px_Marked.dds", 57, "Sprites/UI/startMenu/UI_StartMenu_Text_Speedrun_65x12px_Locked.dds", mySpeedrunUnlocked);
	myOptionsBtn->Init("Sprites/UI/startMenu/UI_StartMenu_Text_Option_44x16px_unmarked.dds", { 44.f,16.f }, optionsBtnPos, "Sprites/UI/startMenu/UI_StartMenu_Text_Option_44x16px_marked.dds", 44);
	myExitGameBtn->Init("Sprites/UI/startMenu/UI_StartMenu_Text_QuitGame_56x16px_Unmarked.dds", { 56.f,16.f }, exitGameBtnPos, "Sprites/UI/startMenu/UI_StartMenu_Text_QuitGame_56x16px_Marked.dds", 56);
	
	SetActiveMenu(true);
	SetBackgroundActive(true);

	InitLogoAnimations();

	myButtons.push_back(myNewGameBtn);
	myButtons.push_back(myLevelSelectBtn);
	myButtons.push_back(mySpeedrunModeBtn);
	myButtons.push_back(myOptionsBtn);
	myButtons.push_back(myExitGameBtn);
}

void MainMenuScene::UpdateObjects(const float& aDeltaTime)
{
	myBackground->UpdateUIObjects(aDeltaTime);
	myTitleSprite->UpdateUIObjects(aDeltaTime);
	LogoAnimation();

	CheckActiveAnimations();
}

void MainMenuScene::CheckButtonsPress()
{
	if (myInput->GetInput()->GetKeyJustDown(Keys::UPARROWKey) || myInput->GetController()->IsButtonPressed(Controller::Button::DPadUp))
	{
		myMovingIndex--;
		if (myMovingIndex < 0)
			myMovingIndex = myButtons.size() - 1;
		if ((myMovingIndex == static_cast<int>(eMainMenuButton::SpeedrunMode) && mySpeedrunModeBtn->GetIsUnlocked() == false))
		{
			myMovingIndex--;
			if (myMovingIndex < 0)
				myMovingIndex = myButtons.size() - 1;
		}
		if ((myMovingIndex == static_cast<int>(eMainMenuButton::LevelSelect) && myLevelSelectBtn->GetIsUnlocked() == false))
		{
			myMovingIndex--;
			if (myMovingIndex < 0)
				myMovingIndex = myButtons.size() - 1;
		}
		AudioManager::GetInstance()->PlayAudio(AudioList::MenuMove);
	}
	else if (myInput->GetInput()->GetKeyJustDown(Keys::DOWNARROWKey) || myInput->GetController()->IsButtonPressed(Controller::Button::DPadDown))
	{
		myMovingIndex++;
		if (myMovingIndex > myButtons.size() - 1)
			myMovingIndex = 0;
		if ((myMovingIndex == static_cast<int>(eMainMenuButton::LevelSelect) && myLevelSelectBtn->GetIsUnlocked() == false))
		{
			myMovingIndex++;
			if (myMovingIndex > myButtons.size() - 1)
				myMovingIndex = 0;
		}
		if ((myMovingIndex == static_cast<int>(eMainMenuButton::SpeedrunMode) && mySpeedrunModeBtn->GetIsUnlocked() == false))
		{
			myMovingIndex++;
			if (myMovingIndex > myButtons.size() - 1)
				myMovingIndex = 0;
		}
		AudioManager::GetInstance()->PlayAudio(AudioList::MenuMove);
	}

	if (myInput->GetInput()->GetKeyJustDown(Keys::ENTERKey) || myInput->GetController()->IsButtonPressed(Controller::Button::Cross))
	{
		if (myMovingIndex == static_cast<int>(eMainMenuButton::StartGame))
		{
			if (!DataManager::GetInstance().GetBonfireState(0))
			{
				CGameWorld::GetInstance()->GetLevelManager().UsedLevelSelect();
			}

			CGameWorld::GetInstance()->GetLevelManager().Continued();

			//CutsceneManager::GetInstance().PlayVideo(CutsceneType::Intro);
			AudioManager::GetInstance()->PlayAudio(AudioList::MenuStart);
			CGameWorld::GetInstance()->GetLevelManager().SingleLoadScene(LevelManager::eScenes::LevelScene);
		}
		else if (myMovingIndex == static_cast<int>(eMainMenuButton::Options))
		{
			AudioManager::GetInstance()->PlayAudio(AudioList::MenuSelect);
			myOptions->SetActive(true);
			mySubMenuActive = true;
			SetActiveMenu(false);
		}
		else if (myMovingIndex == static_cast<int>(eMainMenuButton::LevelSelect) && myLevelSelectBtn->GetIsUnlocked())
		{
			AudioManager::GetInstance()->PlayAudio(AudioList::MenuSelect);
			CGameWorld::GetInstance()->GetLevelManager().SingleLoadScene(LevelManager::eScenes::LevelSelect);

#ifndef _RETAIL
			CGameWorld::GetInstance()->GetLevelManager().ToggleImGui();
#endif //RETAIL
		}
		else if (myMovingIndex == static_cast<int>(eMainMenuButton::SpeedrunMode) && mySpeedrunModeBtn->GetIsUnlocked())
		{
			AudioManager::GetInstance()->PlayAudio(AudioList::MenuSelect);
			CGameWorld::GetInstance()->GetLevelManager().SingleLoadScene(LevelManager::eScenes::SpeedrunScene);
			//CGameWorld::GetInstance()->GetLevelManager().GetSpeedrunManager()->SetIsSpeedrun(true);
			//CGameWorld::GetInstance()->GetLevelManager().SetLevelIndex(0);
			//CGameWorld::GetInstance()->GetLevelManager().SingleLoadScene(LevelManager::eScenes::LevelScene);

#ifndef _RETAIL
			CGameWorld::GetInstance()->GetLevelManager().ToggleImGui();
#endif //RETAIL
		}
		else if (myMovingIndex == static_cast<int>(eMainMenuButton::ExitGame))
			PostQuitMessage(0);
	}
}

void MainMenuScene::SetActiveMenu(const bool aStateBool)
{
	myTitleSprite->SetActive(aStateBool);
	myNewGameBtn->SetActive(aStateBool);
	myLevelSelectBtn->SetActive(aStateBool);
	mySpeedrunModeBtn->SetActive(aStateBool);
	myOptionsBtn->SetActive(aStateBool);
	myExitGameBtn->SetActive(aStateBool);
	myFireHighlight->SetActive(aStateBool);
}

void MainMenuScene::SetBackgroundActive(const bool aStateBool)
{
	myBackground->SetActive(aStateBool);
}

void MainMenuScene::CheckActiveAnimations()
{
	for (int i = 0; i < myButtons.size(); i++)
	{
		if (i == myMovingIndex)
		{
			myButtons[i]->SetIsHighlightActive(true);
			myFireHighlight->SetPositionX(myButtons[i]->GetPositionX() - 11.0f);
			myFireHighlight->SetPositionY(myButtons[i]->GetPositionY() + 4.f);
		}
		else
		{
			myButtons[i]->SetIsHighlightActive(false);
		}
	}
}

void MainMenuScene::InitLogoAnimations()
{
	myLogoTypeTimes[0] = 20;
	myLogoTypeTimes[1] = 3;
	myLogoTypeTimes[2] = 5;
	myLogoTypeTimes[3] = 1;

	myLogoAnimationIndex = 0;

	myCurrentLogoTypeTimes = 0;

	SpriteComponent* animationSprite1 = myTitleSprite->AddComponent<SpriteComponent>();
	SpriteComponent* animationSprite3 = myTitleSprite->AddComponent<SpriteComponent>();

	animationSprite1->SetSpritePath("Sprites/UI/startMenu/UI_startMenu_logotype_B_192x64px.dds");
	animationSprite3->SetSpritePath("Sprites/UI/startMenu/UI_startMenu_logotype_C_192x64px.dds");

	animationSprite1->SetSize(v2f(192.0f, 64.0f));
	animationSprite3->SetSize(v2f(192.0f, 64.0f));

	animationSprite1->Deactivate();
	animationSprite3->Deactivate();

	myLogoAnimations[0] = Animation(false, true, false, 0, 1, 1, 0.06125f, myTitleSprite->GetComponent<SpriteComponent>(), 192, 64);
	myLogoAnimations[1] = Animation(false, true, false, 0, 8, 8, 0.06125f, animationSprite1, 192, 64);
	myLogoAnimations[2] = Animation(false, true, false, 0, 1, 1, 0.06125f, myTitleSprite->GetComponent<SpriteComponent>(), 192, 64);
	myLogoAnimations[3] = Animation(false, true, false, 0, 12, 12, 0.06125f, animationSprite3, 192, 64);

	AnimationComponent* titleAnimation = myTitleSprite->AddComponent<AnimationComponent>();
	titleAnimation->SetSprite(myTitleSprite->GetComponent<SpriteComponent>());

	titleAnimation->SetAnimation(&myLogoAnimations[0]);
}

void MainMenuScene::LogoAnimation()
{
	AnimationComponent* titleAnimation = myTitleSprite->GetComponent<AnimationComponent>();

	if (titleAnimation->GetHasBeenDisplayedOnce())
	{
		++myCurrentLogoTypeTimes;
		titleAnimation->SetAnimation(&myLogoAnimations[myLogoAnimationIndex]);
	}

	if (myCurrentLogoTypeTimes >= myLogoTypeTimes[myLogoAnimationIndex])
	{
		myCurrentLogoTypeTimes = 0;
		++myLogoAnimationIndex;

		if (myLogoAnimationIndex >= 4)
		{
			myLogoAnimationIndex = 0;
		}

		titleAnimation->SetAnimation(&myLogoAnimations[myLogoAnimationIndex]);
	}
}