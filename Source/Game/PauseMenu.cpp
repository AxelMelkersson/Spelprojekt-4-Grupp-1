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
#include "DataManager.h"
#include "AudioManager.h"

#include "AnimationComponent.hpp"

PauseMenu::PauseMenu(Scene* aLevelScene)
	:
	myCamera(aLevelScene->GetCamera()),
	myOptionsMenu(new OptionsMenu(aLevelScene))
{
	myScene = aLevelScene;
	myMovingIndex = {};
	myMenuActive = {};

	mySkipOneUpdate = false;

	myIsSpeedrun = false;

	myIsOutOfFocus = false;

	Subscribe(eMessageType::KilledFocus);
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
	myOptionsBtn = std::make_unique<UIButton>(myScene);
	v2f optionsPos = { 165.f, 125.f };
	myMainMenuBtn = std::make_unique<UIButton>(myScene);
	v2f mainMenuPos = { 165.f, 145.f };

	myOptionsMenu->SetOpenedFromPauseMenu(this);
	myOptionsMenu->Init();

	myBackground->Init("Sprites/UI/pauseMenu/UI_PauseMenu_Bakground_304x164px.dds", {520.f, 265.f}, backgroundPos, 200);
	myBar->Init("Sprites/UI/pauseMenu/UI_PauseMenu_PauseBarScreen_241x3px.dds", { 275.0f, 5.f }, barPos, 201);
	myFire->InitAnimation("Sprites/Objects/Collectible3.dds", { 16.0f, 16.0f }, 8, 8, firePos, 201);
	myFire2->InitAnimation("Sprites/Objects/Collectible2.dds", { 16.0f, 16.0f }, 8, 8, firePos2, 201);
	myFire3->InitAnimation("Sprites/Objects/Collectible1.dds", { 16.0f, 16.0f }, 8, 8, firePos3, 201);

	Animation animation1 = Animation(false, false, false, 0, 7, 7, 0.125f, myFire->GetComponent<SpriteComponent>(), 16, 16);
	Animation animation2 = Animation(false, false, false, 0, 7, 7, 0.125f, myFire2->GetComponent<SpriteComponent>(), 16, 16);
	Animation animation3 = Animation(false, false, false, 0, 7, 7, 0.125f, myFire3->GetComponent<SpriteComponent>(), 16, 16);

	myFire->GetComponent<AnimationComponent>()->SetAnimation(&animation1);
	myFire2->GetComponent<AnimationComponent>()->SetAnimation(&animation2);
	myFire3->GetComponent<AnimationComponent>()->SetAnimation(&animation3);

	myFireHighlight->InitAnimation("Sprites/UI/pauseMenu/UI_PauseMenu_Flame_16x16px.dds", { 16.0f, 16.0f }, 8, 8, { 200.0f, 70.0f }, 201);

	myContinueBtn->Init("Sprites/UI/pauseMenu/UI_PauseMenu_Text_Continue_Unmarked_64x16px.dds", { 64.f,16.f }, continuePos, "Sprites/UI/pauseMenu/UI_PauseMenu_Text_Continue_Marked_64x16px.dds", 64);
	myOptionsBtn->Init("Sprites/UI/pauseMenu/UI_PauseMenu_Text_Options_Unmarked_44x16px.dds", { 44.f,16.f }, optionsPos,"Sprites/UI/pauseMenu/UI_PauseMenu_Text_Options_Marked_44x16px.dds", 44);
	myMainMenuBtn->Init("Sprites/UI/pauseMenu/UI_PauseMenu_Text_MainMenu_Unmarked_64x16px.dds", { 64.f,16.f }, mainMenuPos, "Sprites/UI/pauseMenu/UI_PauseMenu_Text_MainMenu_Marked_64x16px.dds", 64);
	
	myButtons.clear();

	InitTexts();
	myButtons.push_back(myContinueBtn.get());
	myButtons.push_back(myOptionsBtn.get());
	myButtons.push_back(myMainMenuBtn.get());
}

void PauseMenu::Update(const float& aDeltaTime)
{
	if ((CGameWorld::GetInstance()->Input()->GetInput()->GetKeyJustDown(Keys::ESCKey) || myInput->GetController()->IsButtonPressed(Controller::Button::Start)) && !myOptionsMenu->IsOptionsActive())
		SetActiveMenu(!IsPauseActive());

	if (myMenuActive)
		ActivateMenu();
	else
		DeactivateMenu();

	myOptionsMenu->Update(aDeltaTime);

	if (myMenuActive && !mySkipOneUpdate)
	{
		UpdateUIElements(aDeltaTime);
		CheckActiveAnimations();
		CheckIndexPress();
	}
	
	mySkipOneUpdate = false;
}


void PauseMenu::SetActiveMenu(const bool aStatement)
{
	myMenuActive = aStatement;

	if (myMenuActive && !myIsOutOfFocus)
	{
		//UpdateCollectibleInfo(false);
	}
}

bool PauseMenu::IsPauseActive()
{
	return myMenuActive;
}

bool PauseMenu::GetOptionsIsActive()
{
	return myOptionsMenu->IsOptionsActive();
}

void PauseMenu::SelectButton()
{
	if (myMovingIndex == 0)
	{
		AudioManager::GetInstance()->PlayAudio(AudioList::MenuStart);
		SetActiveMenu(false);
	}
	else if (myMovingIndex == 1)
	{
		AudioManager::GetInstance()->PlayAudio(AudioList::MenuSelect);
		myOptionsMenu->SetActive(true);
		SetActiveMenu(false);

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
		AudioManager::GetInstance()->PlayAudio(AudioList::MenuMove);
		myMovingIndex--;
		if (myMovingIndex < 0)
			myMovingIndex = myButtons.size() - 1;
	}
	else if (myInput->GetInput()->GetKeyJustDown(Keys::DOWNARROWKey) || myInput->GetController()->IsButtonPressed(Controller::Button::DPadDown))
	{
		AudioManager::GetInstance()->PlayAudio(AudioList::MenuMove);
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
	myTitleString->UpdateUIObjects(aDeltaTime);

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
	myTitleString->SetActive(true);
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
	myTitleString->SetActive(false);
	myCollectibleString->Deactivate();
	myCollectibleString2->Deactivate();
	myCollectibleString3->Deactivate();

}


void PauseMenu::InitTexts()
{
	UpdateCollectibleInfo(true);

	myTitleString = std::make_unique<UIObject>(myScene);
	myTitleString->Init("Sprites/UI/pauseMenu/UI_PauseMenu_PauseTitleScreen_125x16px.dds", v2f(128.0f, 16.0f), { 155.f, 65.f }, 201);

	myCollectibleString = new UIText(myScene);
	myCollectibleString->Init(std::to_string(myTotalCollectibleInfoCollected[0]) + "/" + std::to_string(myTotalCollectibleInfo[0]), "Text/Peepo.ttf", EFontSize::EFontSize_48);
	myCollectibleString->SetPosition({ 50.0f, 25.0f });

	myCollectibleString2 = new UIText(myScene);
	myCollectibleString2->Init(std::to_string(myTotalCollectibleInfoCollected[1]) + "/" + std::to_string(myTotalCollectibleInfo[1]), "Text/Peepo.ttf", EFontSize::EFontSize_48);
	myCollectibleString2->SetPosition({ 125.0f, 25.0f });

	myCollectibleString3 = new UIText(myScene);
	myCollectibleString3->Init(std::to_string(myTotalCollectibleInfoCollected[2]) + "/" + std::to_string(myTotalCollectibleInfo[2]), "Text/Peepo.ttf", EFontSize::EFontSize_48);
	myCollectibleString3->SetPosition({ 205.0f, 25.0f });
}

void PauseMenu::UpdateCollectibleInfo(const bool aIniting)
{
	myTotalCollectibleInfo.clear();
	myTotalCollectibleInfoCollected.clear();

	DataManager& dataManager = DataManager::GetInstance();
	int collectiblesInfoSize = dataManager.GetCollectableCount();

	myTotalCollectibleInfo.push_back(0);
	myTotalCollectibleInfo.push_back(0);
	myTotalCollectibleInfo.push_back(0);

	myTotalCollectibleInfoCollected.push_back(0);
	myTotalCollectibleInfoCollected.push_back(0);
	myTotalCollectibleInfoCollected.push_back(0);

	for (int collectible = 0; collectible < collectiblesInfoSize; ++collectible)
	{
		CollectableInfo collectibleInfo = dataManager.GetCollectableInfoIndex(collectible);

		++myTotalCollectibleInfo[collectibleInfo.myDifficulty];

		if (collectibleInfo.myCollectedState)
		{
			++myTotalCollectibleInfoCollected[collectibleInfo.myDifficulty];
		}
	}

	if (!aIniting)
	{
		myCollectibleString->SetText(std::to_string(myTotalCollectibleInfoCollected[0]) + "/" + std::to_string(myTotalCollectibleInfo[0]));
		myCollectibleString2->SetText(std::to_string(myTotalCollectibleInfoCollected[1]) + "/" + std::to_string(myTotalCollectibleInfo[1]));
		myCollectibleString3->SetText(std::to_string(myTotalCollectibleInfoCollected[2]) + "/" + std::to_string(myTotalCollectibleInfo[2]));
	}
}

void PauseMenu::SkipOneUpdate()
{
	mySkipOneUpdate = true;
}

void PauseMenu::Notify(const Message& aMessage)
{
	if (aMessage.myMessageType == eMessageType::KilledFocus)
	{
		myIsOutOfFocus = true;
		SetActiveMenu(true);
	}
	else if (aMessage.myMessageType == eMessageType::SetFocus)
	{
		myIsOutOfFocus = false;
		//UpdateCollectibleInfo(false);
	}
}