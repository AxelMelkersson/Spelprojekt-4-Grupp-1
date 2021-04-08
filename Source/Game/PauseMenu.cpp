#include "stdafx.h"
#include "PauseMenu.h"
#include "SpriteComponent.h"
#include "TextComponent.h"
#include "UIButton.h"
#include "GameWorld.h"
#include "InputWrapper.h"
#include "Scene.h"
#include "Camera.h"
#include "Game.h"
#include "SpeedrunManager.h"

#include "AnimationComponent.hpp"

PauseMenu::PauseMenu(Scene* aLevelScene)
	:
	myCamera(aLevelScene->GetCamera())
{
	myScene = aLevelScene;
	myMovingIndex = {};
	myMenuActive = {};
}

void PauseMenu::InitMenu()
{
	myIsSpeedrun = CGameWorld::GetInstance()->GetLevelManager().GetSpeedrunManager()->GetIsSpeedrun();

	v2f referenceSize = Config::ourReferenceSize;

	myInput = CGameWorld::GetInstance()->Input();

	myBackground = std::make_unique<UIObject>(myScene);
	v2f backgroundPos = {5.f, 5.f};
	myBar = std::make_unique<UIObject>(myScene);
	v2f barPos = { 30.0f, 90.0f };
	myFire = std::make_unique<UIObject>(myScene);
	v2f firePos = {35.0f, 20.0f};
	myFire2 = std::make_unique<UIObject>(myScene);
	v2f firePos2 = { 112.5f, 20.0f };
	myFire3 = std::make_unique<UIObject>(myScene);
	v2f firePos3 = { 190.0f, 20.0f };

	myFireHighlight = std::make_unique<UIObject>(myScene);

	myContinueBtn = std::make_unique<UIButton>(myScene);
	v2f continuePos = { 165.f, 105.f };
	myLevelSelectBtn = std::make_unique<UIButton>(myScene);
	v2f levelSelectPos = { 165.f, 125.f };
	myMainMenuBtn = std::make_unique<UIButton>(myScene);
	v2f mainMenuPos = { 165.f, 145.f };
	

	myBackground->Init("Sprites/UI/pauseMenu/UI_PauseMenu_Bakground_304x164px.dds", {520.f, 265.f}, backgroundPos, 200);
	myBar->Init("Sprites/UI/pauseMenu/UI_PauseMenu_PauseBarScreen_241x3px.dds", { 275.0f, 5.f }, barPos, 201);
	myFire->InitAnimation("Sprites/UI/pauseMenu/UI_Collectible_Soul_Red_16x16px.dds", { 16.0f, 16.0f }, firePos, 201);
	myFire2->InitAnimation("Sprites/UI/pauseMenu/UI_Collectible_Soul_Yellow_16x16px.dds", { 16.0f, 16.0f }, firePos2, 201);
	myFire3->InitAnimation("Sprites/UI/pauseMenu/UI_Collectible_Soul_Green_16x16px.dds", { 16.0f, 16.0f }, firePos3, 201);
	myFireHighlight->InitAnimation("Sprites/UI/pauseMenu/UI_PauseMenu_Flame_16x16px.dds", { 16.0f, 16.0f }, { 200.0f, 70.0f }, 201);

	myContinueBtn->Init("Sprites/UI/pauseMenu/UI_PauseMenu_Text_Continue_Unmarked_64x16px.dds", { 64.f,16.f }, continuePos, "Sprites/UI/pauseMenu/UI_PauseMenu_Text_Continue_Marked_64x16px.dds", 64);
	myLevelSelectBtn->Init("Sprites/UI/pauseMenu/UI_PauseMenu_Text_LevelSelect_Unmarked_72x16px.dds", { 72.f,16.f }, levelSelectPos,"Sprites/UI/pauseMenu/UI_PauseMenu_Text_LevelSelect_Marked_72x16px.dds", 72);
	myMainMenuBtn->Init("Sprites/UI/pauseMenu/UI_PauseMenu_Text_MainMenu_Unmarked_64x16px.dds", { 64.f,16.f }, mainMenuPos, "Sprites/UI/pauseMenu/UI_PauseMenu_Text_MainMenu_Marked_64x16px.dds", 64);
	
	myButtons.clear();

	InitTexts();
	myButtons.push_back(myContinueBtn.get());
	myButtons.push_back(myLevelSelectBtn.get());
	myButtons.push_back(myMainMenuBtn.get());
}

void PauseMenu::Update(const float& aDeltaTime)
{
	if (CGameWorld::GetInstance()->Input()->GetInput()->GetKeyJustDown(Keys::ESCKey) || myInput->GetController()->IsButtonPressed(Controller::Button::Start))
		SetActiveMenu(!IsPauseActive());

	if (myMenuActive)
		ActivateMenu();
	else
		DeactivateMenu();

	if (myMenuActive)
	{
		UpdateUIElements(aDeltaTime);
		CheckActiveAnimations();
		CheckIndexPress();
	}
	
}


void PauseMenu::SetActiveMenu(const bool aStatement)
{
	myMenuActive = aStatement;
}

bool PauseMenu::IsPauseActive()
{
	return myMenuActive;
}

void PauseMenu::SelectButton()
{
	if (myMovingIndex == 0)
	{
		SetActiveMenu(false);
	}
	else if (myMovingIndex == 1)
	{
		CGameWorld::GetInstance()->GetLevelManager().SingleLoadScene(LevelManager::eScenes::LevelSelect);
#ifndef _RETAIL
		CGameWorld::GetInstance()->GetLevelManager().ToggleImGui();
#endif //RETAIL
	}
	else if (myMovingIndex == 2)
		CGameWorld::GetInstance()->GetLevelManager().SingleLoadScene(LevelManager::eScenes::MainMenu);
}


void PauseMenu::CheckIndexPress()
{
	if (myInput->GetInput()->GetKeyJustDown(Keys::UPARROWKey) || myInput->GetController()->IsButtonPressed(Controller::Button::DPadUp))
	{
		myMovingIndex--;
		if (myMovingIndex < 0)
			myMovingIndex = myButtons.size() - 1;
	}
	else if (myInput->GetInput()->GetKeyJustDown(Keys::DOWNARROWKey) || myInput->GetController()->IsButtonPressed(Controller::Button::DPadDown))
	{
		myMovingIndex++;
		if (myMovingIndex > myButtons.size() - 1)
			myMovingIndex = 0;
	}

	if (myInput->GetInput()->GetKeyJustDown(Keys::ENTERKey) || myInput->GetController()->IsButtonPressed(Controller::Button::Cross))
	{
		SelectButton();
		//myButtons[myMovingIndex]->ActivateButton();
	}

}

void PauseMenu::UpdateUIElements(const float& aDeltaTime)
{
	myBackground->UpdateUIObjects(aDeltaTime);
	myBar->UpdateUIObjects(aDeltaTime);
	myFireHighlight->UpdateUIObjects(aDeltaTime);
	myFire->UpdateUIObjects(aDeltaTime);
	myFire2->UpdateUIObjects(aDeltaTime);
	myFire3->UpdateUIObjects(aDeltaTime);

	for (auto button : myButtons)
		button->UpdateButton(aDeltaTime);
}


void PauseMenu::ActivateMenu()
{
	for (auto button : myButtons)
		button->SetActive(true);

	myBackground->SetActive(true);
	myBar->SetActive(true);
	myFire->SetActive(true);
	myFire2->SetActive(true);
	myFire3->SetActive(true);
	myFireHighlight->SetActive(true);
	myTitleString->Activate();
	myCollectibleString->Activate();
	myCollectibleString2->Activate();
	myCollectibleString3->Activate();
}

void PauseMenu::CheckActiveAnimations()
{
	for (int i = 0; i < myButtons.size(); i++)
	{
		if (i == myMovingIndex)
		{
			myButtons[i]->SetIsHighlightActive(true);
			myFireHighlight->SetPositionX(myButtons[i]->GetPositionX() - 10.0f);
			myFireHighlight->SetPositionY(myButtons[i]->GetPositionY());
		}
		else
			myButtons[i]->SetIsHighlightActive(false);
	}
}


void PauseMenu::DeactivateMenu()
{
	for (auto button : myButtons)
		button->SetActive(false);

	myBackground->SetActive(false);
	myBar->SetActive(false);
	myFire->SetActive(false);
	myFire2->SetActive(false);
	myFire3->SetActive(false);
	myFireHighlight->SetActive(false);
	myTitleString->Deactivate();
	myCollectibleString->Deactivate();
	myCollectibleString2->Deactivate();
	myCollectibleString3->Deactivate();

}


void PauseMenu::InitTexts()
{
	myTitleString = std::make_unique<UIText>(myScene);
	myTitleString->Init("Pause Menu", "Text/alagard.ttf", EFontSize::EFontSize_48);
	myTitleString->SetPosition({ 155.f, 85.f });
	myCollectibleString = std::make_unique<UIText>(myScene);
	myCollectibleString->Init("00/00", "Text/Peepo.ttf", EFontSize::EFontSize_48);
	myCollectibleString->SetPosition({ 50.0f, 25.0f });
	myCollectibleString2 = std::make_unique<UIText>(myScene);
	myCollectibleString2->Init("00/00", "Text/Peepo.ttf", EFontSize::EFontSize_48);
	myCollectibleString2->SetPosition({ 125.0f, 25.0f });
	myCollectibleString3 = std::make_unique<UIText>(myScene);
	myCollectibleString3->Init("00/00", "Text/Peepo.ttf", EFontSize::EFontSize_48);
	myCollectibleString3->SetPosition({ 205.0f, 25.0f });
}