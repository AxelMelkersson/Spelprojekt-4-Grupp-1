#include "stdafx.h"
#include "UIPopUp.h"

#include "Scene.h"


UIPopUp::UIPopUp(Scene* aLevelScene)
{
	myScene = aLevelScene;
	myCurrentTime = {};
	myMaxTime = 2.0f;
}

void UIPopUp::InitPopUp()
{
	myBackground = std::make_unique<UIObject>(myScene);
	myFireEasy = std::make_unique<UIObject>(myScene);
	myFireMed = std::make_unique<UIObject>(myScene);
	myFireHard = std::make_unique<UIObject>(myScene);

	v2f backPos = { 265.f, 15.f };
	v2f fireEPos = { 280.0f, 30.0f };
	v2f fireMPos = { 280.0f, 30.0f };
	v2f fireHPos = { 280.0f, 30.0f };

	myBackground->Init("Sprites/UI/popUp/UI_PopUp_84x32px.dds", { 84.0f, 32.0f }, backPos, 201);
	myFireEasy->InitAnimation("Sprites/UI/pauseMenu/UI_Collectible_Soul_Green_16x16px.dds", { 16.0f, 16.0f }, fireEPos, 202);
	myFireMed->InitAnimation("Sprites/UI/pauseMenu/UI_Collectible_Soul_Yellow_16x16px.dds", { 16.0f, 16.0f }, fireMPos, 202);
	myFireHard->InitAnimation("Sprites/UI/pauseMenu/UI_Collectible_Soul_Red_16x16px.dds", { 16.0f, 16.0f }, fireHPos, 202);


}

void UIPopUp::Update(const float& aDeltaTime)
{
	if (myEasyActive)
	{
		myCurrentTime += aDeltaTime;
		myBackground->UpdateUIObjects(aDeltaTime);
		myFireEasy->UpdateUIObjects(aDeltaTime);
		Activate(ePopUpTypes::Easy);
	}
	if (myMedActive)
	{
		myCurrentTime += aDeltaTime;
		myBackground->UpdateUIObjects(aDeltaTime);
		myFireMed->UpdateUIObjects(aDeltaTime);
		Activate(ePopUpTypes::Med);
	}
	if (myHardActive == true)
	{
		myCurrentTime += aDeltaTime;
		myBackground->UpdateUIObjects(aDeltaTime);
		myFireHard->UpdateUIObjects(aDeltaTime);
		Activate(ePopUpTypes::Hard);
	}
	if (myCurrentTime > myMaxTime)
	{
		myEasyActive = false;
		myMedActive = false;
		myHardActive = false;
		Deactivate();
	}
}

void UIPopUp::Activate(ePopUpTypes aType)
{
	myBackground->SetActive(true);
	switch (aType)
	{
	case ePopUpTypes::Easy:
		myFireEasy->SetActive(true);
		break;
	case ePopUpTypes::Med:
		myFireMed->SetActive(true);
		break;
	case ePopUpTypes::Hard:
		myFireHard->SetActive(true);
		break;
	}
}

void UIPopUp::Deactivate()
{
	myBackground->SetActive(false);
	myCurrentTime = 0.0f;

	myFireEasy->SetActive(false);
	myFireMed->SetActive(false);
	myFireHard->SetActive(false);

}
