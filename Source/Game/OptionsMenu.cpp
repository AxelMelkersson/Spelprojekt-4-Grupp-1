#include "stdafx.h"
#include "OptionsMenu.h"

#include "GameWorld.h"
#include "InputWrapper.h"
#include "Scene.h"
#include "MainMenuScene.h"
#include "Camera.h"
#include "Game.h"
#include "tga2d/engine.h"
#include "AudioManager.h"
#include "DataManager.h"
#include "AnimationComponent.hpp"

#include "TutorialMenu.h"
#include "CreditsMenu.h"

OptionsMenu::OptionsMenu(Scene* aLevelScene) : myCamera(aLevelScene->GetCamera()), myPauseMenu(nullptr)
{
	myScene = aLevelScene;

	myMovingIndex = 0;
	mySoundMovingIndex = 0;
	myScreenMovingIndex = 0;

	myMusicVol = 0.0f;
	mySFXVol = 0.0f;
	myMusicStep = 2.0f;
	myVFXStep = 2.0f;

	myMenuAcitve = false;
	myIsOpenedFromPause = false;

	myAudioManager = nullptr;
	myCreditsMenu = nullptr;
	myTutorial = nullptr;
}

void OptionsMenu::Init()
{
	v2f referenceSize = Config::ourReferenceSize;
	myMusicVol = myAudioManager->GetInstance()->GetMusicVolume();
	mySFXVol = myAudioManager->GetInstance()->GetSFXVolume();
	myInput = CGameWorld::GetInstance()->Input();

	myBackground = new UIObject(myScene);
	myBar = new UIObject(myScene);
	myTitle = new UIObject(myScene);

	myFireHighlight = new UIObject(myScene);
	mySoundBtn = new UIButton(myScene);
	myCreditsBtn = new UIButton(myScene);
	myTutorialsBtn = new UIButton(myScene);
	myBackBtn = new UIButton(myScene);
	myResetBtn = new UIButton(myScene);
	myScreenBtn = new UIButton(myScene);

	mySoundSettings = new UIObject(myScene);
	myBGHighlight = new UIObject(myScene);
	myVFXHighlight = new UIObject(myScene);
	myBGDot = new UIObject(myScene);
	myVFXDot = new UIObject(myScene);

	myTutorial = new TutorialMenu(myScene);
	myCreditsMenu = new CreditsMenu(myScene);

	myResolutions = new UIObject(myScene);
	my720pHgh = new UIObject(myScene);
	my1080pHgh = new UIObject(myScene);
	my4KHgh = new UIObject(myScene);
	myScreenSizeDot = new UIObject(myScene);

	v2f backgroundPos = { 8.f, 8.f };
	v2f titlePos = { 140.f, 35.f };
	v2f barPos = { 30.0f, 60.0f };
	v2f screenPos = { 140.f, 70.f };
	v2f soundPos = { 140.f, 90.f };
	v2f creditsPos = { 140.f, 110.f };
	v2f tutorialPos = { 140.f, 130.f };
	v2f backPos = { 140.f, 150.f };
	v2f resetPos = { 30.f, 160.f };
	v2f soundSettingPos = { 215.f, 90.f };
	v2f bgDot = { 215.f + (myMusicVol * 74.f), 97.5f };
	v2f SFXDot = { 215.f + (mySFXVol * 74.f), 114.5f };
	v2f resolutionPos = { 215, 73.f };
	v2f creditScreenPos = { 120.f, 50.f };

	//Misc
	myBackground->Init("Sprites/UI/optionsMenu/UI_OptionsMenu_Background.dds", { 512.0f, 256.f }, backgroundPos, 201);
	myBar->Init("Sprites/UI/pauseMenu/UI_PauseMenu_PauseBarScreen_241x3px.dds", { 275.0f, 5.f }, barPos, 202);
	myTitle->Init("Sprites/UI/optionsMenu/UI_options_MenuTitle_143_20px.dds", { 250.f, 35.f }, titlePos, 202);

	//Buttons
	myFireHighlight->InitAnimation("Sprites/UI/pauseMenu/UI_PauseMenu_Flame_16x16px.dds", { 16.0f, 16.0f }, 8, 8, { 200.0f, 70.0f }, 202);
	myScreenBtn->Init("Sprites/UI/optionsMenu/UI_OptionsMenu_Text_Screensize_66x16px_Unmarked.dds", { 66.f, 16.f }, screenPos, "Sprites/UI/optionsMenu/UI_OptionsMenu_Text_Screensize_66x16px_Marked.dds", 66);
	mySoundBtn->Init("Sprites/UI/optionsMenu/UI_OptionsMenu_Text_Sound_36x16px_Unmarked.dds", { 36.0f, 16.0f }, soundPos, "Sprites/UI/optionsMenu/UI_OptionsMenu_Text_Sound_36x16px_Marked.dds", 36);
	myCreditsBtn->Init("Sprites/UI/optionsMenu/UI_OptionsMenu_Text_Credits_45x10px_Unmarked.dds", { 45.f, 16.f }, creditsPos, "Sprites/UI/optionsMenu/UI_OptionsMenu_Text_Credits_45x10px_Marked.dds", 45);
	myTutorialsBtn->Init("Sprites/UI/optionsMenu/UI_OptionsMenu_Text_Tutorials_48x10px_Unmarked.dds", { 48.f, 16.f }, tutorialPos, "Sprites/UI/optionsMenu/UI_OptionsMenu_Text_Tutorials_48x10px_Marked.dds", 48);
	
	if (myIsOpenedFromPause)
	{
		myBackBtn->Init("Sprites/UI/pauseMenu/UI_PauseMenu_Text_Back_29x16px_Unmarked.dds", { 29.f,16.f }, backPos, "Sprites/UI/pauseMenu/UI_PauseMenu_Text_Back_29x16px_Marked.dds", 29);
	}
	else
	{
		myBackBtn->Init("Sprites/UI/optionsMenu/UI_OptionsMenu_Text_MainMenu_Unmarked_64x16px.dds", { 64.f,16.f }, backPos, "Sprites/UI/optionsMenu/UI_OptionsMenu_Text_MainMenu_Marked_64x16px.dds", 64);
	}
	
	myResetBtn->Init("Sprites/UI/optionsMenu/UI_OptionsMenu_Text_ResetSave_65x16px_Unmarked.dds", { 65.f,16.f }, resetPos, "Sprites/UI/optionsMenu/UI_OptionsMenu_Text_ResetSave_65x16px_Marked.dds", 65);

	//Sound
	mySoundSettings->Init("Sprites/UI/optionsMenu/UI_OptionsMenu_Text_Sound_Setting_74x26px_Unmarked.dds", { 128.f, 32.f }, soundSettingPos, 202);
	myBGHighlight->Init("Sprites/UI/optionsMenu/UI_OptionsMenu_Text_Sound_Setting_74x26px_BG_Marked.dds", { 128.f, 32.f }, soundSettingPos, 203);
	myVFXHighlight->Init("Sprites/UI/optionsMenu/UI_OptionsMenu_Text_Sound_Setting_74x26px_VFX_Marked.dds", { 128.f, 32.f }, soundSettingPos, 203);
	myBGDot->Init("Sprites/UI/optionsMenu/UI_OptionsMenu_Text_Sound_SettingsMark_3x3px.dds", { 3.f, 3.f }, bgDot, 204);
	myVFXDot->Init("Sprites/UI/optionsMenu/UI_OptionsMenu_Text_Sound_SettingsMark_3x3px.dds", { 3.f, 3.f }, SFXDot, 204);

	//Credits
	myCreditsMenu->SetPosition(v2f(8.0f, 8.0f));
	myCreditsMenu->SetZIndex(205);

	//Tutorial
	myTutorial->SetPosition(v2f(8.0f, 8.0f));
	myTutorial->SetZIndex(205);

	//Screen
	myResolutions->Init("Sprites/UI/optionsMenu/UI_OptionsMenu_Text_Screensize_Resolutions_All_73x7px_Unmarked.dds", { 128.0f, 8.0f }, resolutionPos, 202);
	my720pHgh->Init("Sprites/UI/optionsMenu/UI_OptionsMenu_Text_Screensize_Resolutions_720p_73x7pxMarked.dds", { 128.0f, 8.0f }, resolutionPos, 203);
	my1080pHgh->Init("Sprites/UI/optionsMenu/UI_OptionsMenu_Text_Screensize_Resolutions_1080p_73x7pxMarked.dds", { 128.0f, 8.0f }, resolutionPos, 203);
	my4KHgh->Init("Sprites/UI/optionsMenu/UI_OptionsMenu_Text_Screensize_Resolutions_4k_73x7pxMarked.dds", { 128.0f, 8.0f }, resolutionPos, 203);
	myScreenSizeDot->Init("Sprites/UI/optionsMenu/UI_OptionsMenu_Text_Screensize_Resolutions_73x7px_Marked.dds", { 8.f, 8.f }, resolutionPos, 204);

	my720pHgh->UpdateUIObjects(0);
	my1080pHgh->UpdateUIObjects(0);
	my4KHgh->UpdateUIObjects(0);

	if (CGameWorld::GetInstance()->Game()->GetZoomY() > 720)
	{
		myScreenSizeDot->SetPositionX(my1080pHgh->GetPositionX() + 27.0f);
		myScreenMovingIndex = 1;
	}
	else if (CGameWorld::GetInstance()->Game()->GetZoomY() > 1080)
	{
		myScreenSizeDot->SetPositionX(my4KHgh->GetPositionX() + 58.0f);
		myScreenMovingIndex = 2;
	}
	else
	{
		myScreenSizeDot->SetPositionX(my720pHgh->GetPositionX());
		myScreenMovingIndex = 0;
	}

	myButtons.push_back(myScreenBtn);
	myButtons.push_back(mySoundBtn);
	myButtons.push_back(myCreditsBtn);
	myButtons.push_back(myTutorialsBtn);
	myButtons.push_back(myBackBtn);
	myButtons.push_back(myResetBtn);

	mySoundObjects.push_back(myBGHighlight);
	mySoundObjects.push_back(myVFXHighlight);

	myResolutionObj.push_back(my720pHgh);
	myResolutionObj.push_back(my1080pHgh);
	myResolutionObj.push_back(my4KHgh);
}
void OptionsMenu::Update(const float& aDeltaTime)
{
	if (myTutorialActtive)
	{
		myTutorial->Activate();
	}
	else
	{
		myTutorial->Deactivate();
	}

	if (myCreditsActive)
	{
		myCreditsMenu->Activate();
	}
	else
	{
		myCreditsMenu->Deactivate();
	}

	if (myMenuAcitve)
	{
		ActivateMenu();
		CheckActiveAnimations();
		CheckIndexPress(aDeltaTime);
		UpdateUIElements(aDeltaTime);
	}
	else
	{
		DeactivateMenu();
	}
}

void OptionsMenu::SetActive(const bool aStatement)
{
	myMenuAcitve = aStatement;
}
const bool OptionsMenu::IsOptionsActive() const
{
	return myMenuAcitve;
}

void OptionsMenu::DeactivateTutorial()
{
	myTutorial->Deactivate();
}
void OptionsMenu::DeactivateCredits()
{
	myCreditsMenu->Deactivate();
}

void OptionsMenu::SetOpenedFromPauseMenu(PauseMenu* aPauseMenu)
{
	myIsOpenedFromPause = true;
	myPauseMenu = aPauseMenu;
}

// Private Methods
void OptionsMenu::CheckIndexPress(const float& aDeltaTime)
{
	bool entered = myInput->GetInput()->GetKeyJustDown(Keys::ENTERKey) || myInput->GetController()->IsButtonPressed(Controller::Button::Cross);

	if (entered && myScreenSettingsActive == false)
	{
		switch (static_cast<eOptionsMenu>(myMovingIndex))
		{
		case eOptionsMenu::Back:{
			AudioManager::GetInstance()->PlayAudio(AudioList::MenuBack);
			if (myIsOpenedFromPause)
			{
				SetActive(false);
				myPauseMenu->SetActiveMenu(true);
				myPauseMenu->SkipOneUpdate();
			}
			else
			{
				CGameWorld::GetInstance()->GetLevelManager().ReloadScene(LevelManager::eScenes::MainMenu);
			}
			break;
		}
		case eOptionsMenu::Sound:{
			if (!mySoundSettingsActive)
			{
				AudioManager::GetInstance()->PlayAudio(AudioList::MenuSelect);
				mySoundSettingsActive = true;
				mySubMenuActive = true;

			}
			else
			{
				mySoundSettingsActive = false;
				myBGHighlight->SetActive(false);
				myVFXHighlight->SetActive(false);
				AudioManager::GetInstance()->PlayAudio(AudioList::MenuBack);
				DataManager::GetInstance().SaveSFXVolume(mySFXVol);
				DataManager::GetInstance().SaveMusicVolume(myMusicVol);
			}
			break;
		}
		case eOptionsMenu::ScreenSize:{
			if (!myScreenSettingsActive)
			{
				AudioManager::GetInstance()->PlayAudio(AudioList::MenuSelect);
				myScreenSettingsActive = true;
			}
			else
			{
				AudioManager::GetInstance()->PlayAudio(AudioList::MenuBack);
				myScreenSettingsActive = false;
			}
			break;
		}
		case eOptionsMenu::Credits:{
			if (!myCreditsActive)
			{
				AudioManager::GetInstance()->PlayAudio(AudioList::MenuSelect);
				myCreditsActive = true;
				myCreditsMenu->Activate();
				DeactivateMenu();
			}
			else
			{
				AudioManager::GetInstance()->PlayAudio(AudioList::MenuBack);
				myCreditsMenu->Deactivate();
				ActivateMenu();
				myCreditsActive = false;
			}
			break;
		}
		case eOptionsMenu::Tutorial:{
			if (!myTutorialActtive)
			{
				AudioManager::GetInstance()->PlayAudio(AudioList::MenuSelect);
				myTutorialActtive = true;
				DeactivateMenu();
				myTutorial->Activate();
			}
			else
			{
				AudioManager::GetInstance()->PlayAudio(AudioList::MenuBack);
				myTutorialActtive = false;
				ActivateMenu();
				myTutorial->Deactivate();
			}
			break;
		}
		default:{
			assert((false) && "myMovingIndex in OptionsMenu::CheckIndexPress does not correlate to eOptionsMenu.");
			break;
		}
		}
	}
	else if (myScreenSettingsActive == true && entered)
	{
		AudioManager::GetInstance()->PlayAudio(AudioList::MenuBack);
		switch (myScreenMovingIndex)
		{
		case 0:{
			myScreenSizeDot->SetPositionX(my720pHgh->GetPositionX() - myCamera.GetPosition().x);
			CGameWorld::GetInstance()->Game()->UpdateWindowSize(1280, 720);
			break;
		}
		case 1:{
			myScreenSizeDot->SetPositionX(my1080pHgh->GetPositionX() + 27.f - myCamera.GetPosition().x);
			CGameWorld::GetInstance()->Game()->UpdateWindowSize(1920, 1080);
			break;
		}
		case 2:{
			myScreenSizeDot->SetPositionX(my4KHgh->GetPositionX() + 58.f - myCamera.GetPosition().x);
			CGameWorld::GetInstance()->Game()->UpdateWindowSize(3840, 2160);
			break;
		}
		default:{
			assert((false) && "myScreenMovingIndex in OptionsMenu::CheckIndexPress does not correlate to any option.");
			break;
		}
		}
		myScreenSettingsActive = false;

		for (int i = 0; i < myResolutionObj.size(); i++)
		{
			myResolutionObj[i]->SetActive(false);
		}
	}

	if (mySoundSettingsActive)
	{
		if (myInput->GetInput()->GetKeyJustDown(Keys::UPARROWKey) || myInput->GetController()->IsButtonPressed(Controller::Button::DPadUp))
		{
			AudioManager::GetInstance()->PlayAudio(AudioList::MenuMove);
			mySoundMovingIndex--;
			if (mySoundMovingIndex < 0)
				mySoundMovingIndex = mySoundObjects.size() - 1;
		}
		else if (myInput->GetInput()->GetKeyJustDown(Keys::DOWNARROWKey) || myInput->GetController()->IsButtonPressed(Controller::Button::DPadDown))
		{
			AudioManager::GetInstance()->PlayAudio(AudioList::MenuMove);
			mySoundMovingIndex++;
			if (mySoundMovingIndex > mySoundObjects.size() - 1)
				mySoundMovingIndex = 0;
		}

		if (myInput->GetInput()->GetKeyDown(Keys::RIGHTARROWKey) || myInput->GetController()->IsButtonHoldDown(Controller::Button::DPadRight))
		{
			v2f bgDot = { 215.f + (myMusicVol * 74.f), 97.5f };
			v2f SFXDot = { 215.f + (mySFXVol * 74.f), 114.5f };

			//AudioManager::GetInstance()->PlayAudio(AudioList::MenuMove);
			if (mySoundMovingIndex == 0 && myMusicVol < 1.0f)
			{
				myMusicVol += .5f * aDeltaTime;
				myBGDot->SetPosition(bgDot - v2f(0.0f - myMusicStep * aDeltaTime, 0));
				myAudioManager->GetInstance()->SetMusicVolume(myMusicVol);
			}
			else if (mySoundMovingIndex == 1 && mySFXVol < 1.0f)
			{
				mySFXVol += .5f * aDeltaTime;
				myVFXDot->SetPosition(SFXDot - v2f(0.0f - myVFXStep * aDeltaTime, 0.0f));
				myAudioManager->GetInstance()->SetSFXVolume(mySFXVol);
			}
		}
		else if (myInput->GetInput()->GetKeyDown(Keys::LEFTARROWKey) || myInput->GetController()->IsButtonHoldDown(Controller::Button::DPadLeft))
		{
			v2f bgDot = { 215.f + (myMusicVol * 74.f), 97.5f };
			v2f SFXDot = { 215.f + (mySFXVol * 74.f), 114.5f };

			//AudioManager::GetInstance()->PlayAudio(AudioList::MenuMove);
			if (mySoundMovingIndex == 0 && myMusicVol > 0.0f)
			{
				myMusicVol -= .5f * aDeltaTime;
				myBGDot->SetPosition(bgDot + v2f(0.0f - myMusicStep * aDeltaTime, 0));
				myAudioManager->GetInstance()->SetMusicVolume(myMusicVol);
			}
			else if (mySoundMovingIndex == 1 && mySFXVol > 0.0f)
			{
				mySFXVol -= 0.5f * aDeltaTime;
				myVFXDot->SetPosition(SFXDot + v2f(0.0f - myVFXStep * aDeltaTime, 0.0f));
				myAudioManager->GetInstance()->SetSFXVolume(mySFXVol);
			}
		}
	}

	else if (myScreenSettingsActive)
	{
		if (myInput->GetInput()->GetKeyJustDown(Keys::LEFTARROWKey) || myInput->GetController()->IsButtonPressed(Controller::Button::DPadLeft))
		{
			AudioManager::GetInstance()->PlayAudio(AudioList::MenuMove);
			myScreenMovingIndex--;
			if (myScreenMovingIndex < 0)
				myScreenMovingIndex = myResolutionObj.size() - 1;
		}
		else if (myInput->GetInput()->GetKeyJustDown(Keys::RIGHTARROWKey) || myInput->GetController()->IsButtonPressed(Controller::Button::DPadRight))
		{
			AudioManager::GetInstance()->PlayAudio(AudioList::MenuMove);
			myScreenMovingIndex++;
			if (myScreenMovingIndex > myResolutionObj.size() - 1)
				myScreenMovingIndex = 0;
		}
	}
	else if (!mySoundSettingsActive && !myScreenSettingsActive && !myTutorialActtive && !myCreditsActive)
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
	}
}
void OptionsMenu::ActivateMenu()
{
	if (myTutorialActtive || myCreditsActive)
	{
		return;
	}

	for (auto button : myButtons)
		button->SetActive(true);

	myBackground->SetActive(true);
	myTitle->SetActive(true);
	myFireHighlight->SetActive(true);
	myBar->SetActive(true);
	mySoundSettings->SetActive(true);
	myBGDot->SetActive(true);
	myVFXDot->SetActive(true);
	myResolutions->SetActive(true);
	myScreenSizeDot->SetActive(true);
}
void OptionsMenu::DeactivateMenu()
{
	for (auto button : myButtons)
		button->SetActive(false);

	for (auto objects : mySoundObjects)
		objects->SetActive(false);

	for (auto res : myResolutionObj)
		res->SetActive(false);

	myBackground->SetActive(false);
	myTitle->SetActive(false);
	myBackBtn->SetActive(false);
	myBar->SetActive(false);
	mySoundSettings->SetActive(false);
	myBGDot->SetActive(false);
	myVFXDot->SetActive(false);
	myResolutions->SetActive(false);
	myScreenSizeDot->SetActive(false);
	myFireHighlight->SetActive(false);
}
void OptionsMenu::UpdateUIElements(const float& aDeltaTime)
{
	myBackground->UpdateUIObjects(aDeltaTime);
	myTitle->UpdateUIObjects(aDeltaTime);
	myBar->UpdateUIObjects(aDeltaTime);
	mySoundSettings->UpdateUIObjects(aDeltaTime);
	myBGDot->UpdateUIObjects(aDeltaTime);
	myVFXDot->UpdateUIObjects(aDeltaTime);

	myResolutions->UpdateUIObjects(aDeltaTime);
	myScreenSizeDot->UpdateUIObjects(aDeltaTime);

	if (myIsOpenedFromPause)
	{
		my720pHgh->UpdateUIObjects(0);
		my1080pHgh->UpdateUIObjects(0);
		my4KHgh->UpdateUIObjects(0);

		if (myScreenMovingIndex == 1)
		{
			myScreenSizeDot->SetPositionX(my1080pHgh->GetPositionX() + 27.0f);
		}
		else if (myScreenMovingIndex == 2)
		{
			myScreenSizeDot->SetPositionX(my4KHgh->GetPositionX() + 58.0f);
		}
		else
		{
			myScreenSizeDot->SetPositionX(my720pHgh->GetPositionX());
		}

		myTutorial->Update(0);
		myTutorial->SetPosition(myCamera.GetPosition() + v2f(8.0f, 8.0f));

		myCreditsMenu->Update(0);
		myCreditsMenu->SetPosition(myCamera.GetPosition() + v2f(8.0f, 8.0f));

		myFireHighlight->Update(aDeltaTime);
	}

	for (auto button : myButtons)
		button->UpdateButton(true);

	for (auto objects : mySoundObjects)
		objects->UpdateUIObjects(true);

	for (auto res : myResolutionObj)
		res->UpdateUIObjects(true);
}
void OptionsMenu::CheckActiveAnimations()
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
	if (mySoundSettingsActive == true)
	{
		InactivateHighlight();
		for (int i = 0; i < mySoundObjects.size(); i++)
		{
			if (i == mySoundMovingIndex)
			{
				mySoundObjects[i]->SetActive(true);
			}
			else
			{
				mySoundObjects[i]->SetActive(false);
			}
		}
	}
	if (myScreenSettingsActive == true)
	{
		InactivateHighlight();
		for (int i = 0; i < myButtons.size(); i++)
		{
			myButtons[i]->SetIsHighlightActive(false);
		}
		for (int i = 0; i < myResolutionObj.size(); i++)
		{
			if (i == myScreenMovingIndex)
			{
				myResolutionObj[i]->SetActive(true);
			}
			else
			{
				myResolutionObj[i]->SetActive(false);
			}
		}
	}
	if (myCreditsActive == true || myTutorialActtive == true)
	{
		InactivateHighlight();
	}
}
void OptionsMenu::InactivateHighlight()
{
	myFireHighlight->SetActive(false);
	for (int i = 0; i < myButtons.size(); i++)
	{
		myButtons[i]->SetIsHighlightActive(false);
	}
}