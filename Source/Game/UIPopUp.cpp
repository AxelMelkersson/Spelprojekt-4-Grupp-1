#include "stdafx.h"
#include "UIPopUp.h"
#include "DataManager.h"
#include "Scene.h"
#include "UIText.h"
#include <string>
#include <iostream>
#include <vector>
#include "TextComponent.h"
#include "PostMaster.hpp"
#include "Game.h"
#include "SpriteComponent.h"
#include "AnimationComponent.hpp"
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>

UIPopUp::UIPopUp(Scene* aLevelScene)
	:
	GameObject(aLevelScene)
{
	myScene = aLevelScene;
	myCurrentTime = {};
	myCurrentMTime = {};
	myCurrentHTime = {};
	myMaxTime = 4.0f;
	myStayTime = 1.5f;
	myCurrentStayTime = {};
	myCurrentMStayTime = {};
	myCurrentHStayTime = {};
	myLevelIndex = 0;
}

void UIPopUp::InitPopUp()
{
	myCollectibleInfo.clear();
	myCollectibleCollected.clear();

	Subscribe(eMessageType::PopUpMessageE);
	Subscribe(eMessageType::PopUpMessageM);
	Subscribe(eMessageType::PopUpMessageH);

	Config::ourReferenceSize = { 320.f, 180.f };

	myBackgroundE = new UIObject(myScene);
	myBackgroundM = new UIObject(myScene);
	myBackgroundH = new UIObject(myScene);

	myFireEasy = new UIObject(myScene);
	myFireMed = new UIObject(myScene);
	myFireHard = new UIObject(myScene);

	v2f backPos = { Config::ourReferenceSize.x, 15.f };
	v2f firePos = { Config::ourReferenceSize.x + 10.f, Config::ourReferenceSize.y - 150.0f };
	v2f collectiblePos = { Config::ourReferenceSize.x + 25.f, 35.0f };

	myBackgroundE->Init("Sprites/UI/popUp/UI_PopUp_84x32px.dds", { 84.0f, 32.0f }, backPos, 200);
	myBackgroundM->Init("Sprites/UI/popUp/UI_PopUp_84x32px.dds", { 84.0f, 32.0f }, backPos, 200);
	myBackgroundH->Init("Sprites/UI/popUp/UI_PopUp_84x32px.dds", { 84.0f, 32.0f }, backPos, 200);
	myFireEasy->InitAnimation("Sprites/Objects/Collectible3.dds", { 16.0f, 16.0f }, 7, 7, firePos, 201);
	myFireMed->InitAnimation("Sprites/Objects/Collectible2.dds", { 16.0f, 16.0f }, 7, 7, firePos, 201);
	myFireHard->InitAnimation("Sprites/Objects/Collectible1.dds", { 16.0f, 16.0f }, 7, 7, firePos, 201);

	for (int i = 0; i < 8; ++i)
	{
		myCollectibleInfo.push_back(std::vector<int>());
		myCollectibleCollected.push_back(std::vector<int>());

		for (int difficulty = 0; difficulty < 3; ++difficulty)
		{
			myCollectibleInfo[i].push_back(0);
			myCollectibleCollected[i].push_back(0);
		}
	}

	for (int j = 0; j < DataManager::GetInstance().GetCollectableCount(); ++j)
	{
		DataManager::GetInstance().GetCollectableInfoIndex(j);

		++myCollectibleInfo[DataManager::GetInstance().GetCollectableInfoIndex(j).myBonfireID][DataManager::GetInstance().GetCollectableInfoIndex(j).myDifficulty];

		if (DataManager::GetInstance().GetCollectableInfoIndex(j).myCollectedState)
		{
			++myCollectibleCollected[DataManager::GetInstance().GetCollectableInfoIndex(j).myBonfireID][DataManager::GetInstance().GetCollectableInfoIndex(j).myDifficulty];
		}
	}

	myCollectibleString = new UIText(myScene);
	myCollectibleString->Init(std::to_string(myCollectibleCollected[0][0]) + "/" + std::to_string(myCollectibleInfo[0][0]), "Text/Peepo.ttf", EFontSize_48);
	myCollectibleString->SetPosition(collectiblePos);
	myCollectibleString->SetZIndex(201);
	myCollectibleString2 = new UIText(myScene);
	myCollectibleString2->Init(std::to_string(myCollectibleCollected[0][1]) + "/" + std::to_string(myCollectibleInfo[0][1]), "Text/Peepo.ttf", EFontSize_48);
	myCollectibleString2->SetPosition(collectiblePos);
	myCollectibleString2->SetZIndex(201);
	myCollectibleString3 = new UIText(myScene);
	myCollectibleString3->Init(std::to_string(myCollectibleCollected[0][2]) + "/" + std::to_string(myCollectibleInfo[0][2]), "Text/Peepo.ttf", EFontSize_48);
	myCollectibleString3->SetPosition(collectiblePos);
	myCollectibleString3->SetZIndex(201);


	std::ifstream levelSelectFile("JSON/Menus/LevelSelect/LevelSelect.json");
	rapidjson::IStreamWrapper levelSelectStream(levelSelectFile);

	rapidjson::Document levelSelect;
	levelSelect.ParseStream(levelSelectStream);


	int bonfireIndex = 0;
	for (rapidjson::Value::ConstValueIterator iterator = levelSelect["Bonfires"].Begin(); iterator != levelSelect["Bonfires"].End(); ++iterator)
	{
		if (DataManager::GetInstance().GetBonfireState(bonfireIndex))
		{
			myLevelIndex = bonfireIndex;
		}

		++bonfireIndex;
	}

}


void UIPopUp::Update(const float& aDeltaTime)
{
	UpdateCollectibles();
	if (myEasyActive)
	{

		myCurrentTime += aDeltaTime;

		SetNewPositions(aDeltaTime);
		myBackgroundE->UpdateUIObjects(aDeltaTime);
		myFireEasy->UpdateUIObjects(aDeltaTime);
	}
	if (myMedActive)
	{
		if (myEasyActive == false && myHardActive == false)
		{
			myCurrentMTime += aDeltaTime;

			//UpdateCollectibles();
			SetNewMedPositions(aDeltaTime);
			myBackgroundM->UpdateUIObjects(aDeltaTime);
			myFireMed->UpdateUIObjects(aDeltaTime);
		}
	}
	if (myHardActive)
	{
		myCurrentHTime += aDeltaTime;
		UpdateCollectibles();
		SetNewHardPositions(aDeltaTime);
		myBackgroundH->UpdateUIObjects(aDeltaTime);
		myFireHard->UpdateUIObjects(aDeltaTime);
	}
	//else
	//{
	//	Deactivate();
	//	myEasyActive = false;
	//	myMedActive = false;
	//	myHardActive = false;
	//}
	if (myCurrentTime > myMaxTime)
	{
		Deactivate(ePopUpTypes::Easy);
	}
	else if (myCurrentMTime > myMaxTime)
	{
		Deactivate(ePopUpTypes::Med);
	}
	else if (myCurrentHTime > myMaxTime)
	{
		Deactivate(ePopUpTypes::Hard);
	}

}

void UIPopUp::Activate(ePopUpTypes aType)
{
	switch (aType)
	{
	case ePopUpTypes::Easy:
	{
		myFireEasy->SetActive(true);
		myBackgroundE->SetActive(true);
		myCollectibleString->Activate();
		myEasyActive = true;
		break;
	}
	case ePopUpTypes::Med:
	{
		myFireMed->SetActive(true);
		myBackgroundM->SetActive(true);
		myCollectibleString2->Activate();
		myMedActive = true;
		break;
	}
	case ePopUpTypes::Hard:
		myFireHard->SetActive(true);
		myBackgroundH->SetActive(true);
		myCollectibleString3->Activate();
		myHardActive = true;
		break;
	}
}

void UIPopUp::Notify(const Message& aMessage)
{
	switch (aMessage.myMessageType)
	{
	case eMessageType::PopUpMessageE:
	{
		Activate(ePopUpTypes::Easy);
		break;
	}
	case eMessageType::PopUpMessageM:
	{
		Activate(ePopUpTypes::Med);
		break;
	}
	case eMessageType::PopUpMessageH:
	{
		Activate(ePopUpTypes::Hard);
		break;
	}

	}

}

void UIPopUp::Deactivate(ePopUpTypes aType)
{

	switch (aType)
	{
	case ePopUpTypes::Easy:
		myCurrentTime = 0.0f;
		myCurrentStayTime = 0.f;
		myIsMaxLeft = false;
		myFireEasy->SetActive(false);
		myBackgroundE->SetActive(false);
		myCollectibleString->Deactivate();
		myEasyActive = false;
		break;
	case ePopUpTypes::Med:
		myCurrentMTime = 0.0f;
		myCurrentMStayTime = 0.f;
		myIsMMaxLeft = false;
		myFireMed->SetActive(false);
		myBackgroundM->SetActive(false);
		myCollectibleString2->Deactivate();
		myMedActive = false;
		break;
	case ePopUpTypes::Hard:
		myCurrentHTime = 0.0f;
		myCurrentHStayTime = 0.f;
		myIsHMaxLeft = false;
		myFireHard->SetActive(false);
		myBackgroundH->SetActive(false);
		myCollectibleString3->Deactivate();
		myHardActive = false;
		break;
	}
}

void UIPopUp::SetNewPositions(const float& aDeltaTime)
{
	Config::ourReferenceSize = { 320.f, 180.f };

	if (myEasyActive)
	{
		if (myIsMaxLeft == false)
		{
			if (myBackgroundE->GetStartPosition().x < Config::ourReferenceSize.x - 55.f)
			{
				myIsMaxLeft = true;
			}
			else
			{
				myBackgroundE->SetPositionX(myBackgroundE->GetStartPosition().x - 50.f * aDeltaTime);
				myFireEasy->SetPositionX(myFireEasy->GetStartPosition().x - 50.f * aDeltaTime);
				myCollectibleString->SetPosition(myCollectibleString->GetPosition() - v2f(50.f * aDeltaTime, 0));
			}
		}
		else if (myIsMaxLeft == true)
		{
			myCurrentStayTime += aDeltaTime;
			if (myCurrentStayTime > myStayTime)
			{
				myBackgroundE->SetPositionX(myBackgroundE->GetStartPosition().x + 50.f * aDeltaTime);
				myFireEasy->SetPositionX(myFireEasy->GetStartPosition().x + 50.f * aDeltaTime);
				myCollectibleString->SetPosition(myCollectibleString->GetPosition() + v2f(50.f * aDeltaTime, 0));
			}
		}
	}
}

void UIPopUp::SetNewMedPositions(const float& aDeltaTime)
{
	Config::ourReferenceSize = { 320.f, 180.f };

	if (myMedActive)
	{
		if (myIsMMaxLeft == false)
		{
			if (myBackgroundM->GetStartPosition().x < Config::ourReferenceSize.x - 55.f)
			{
				myIsMMaxLeft = true;
			}
			myBackgroundM->SetPositionX(myBackgroundM->GetStartPosition().x - 50.f * aDeltaTime);
			myFireMed->SetPositionX(myFireMed->GetStartPosition().x - 50.f * aDeltaTime);
			myCollectibleString2->SetPosition(myCollectibleString2->GetPosition() - v2f(50.f * aDeltaTime, 0));
		}
		else if (myIsMMaxLeft == true)
		{
			myCurrentMStayTime += aDeltaTime;
			if (myCurrentMStayTime > myStayTime)
			{
				myBackgroundM->SetPositionX(myBackgroundM->GetStartPosition().x + 50.f * aDeltaTime);
				myFireMed->SetPositionX(myFireMed->GetStartPosition().x + 50.f * aDeltaTime);
				myCollectibleString2->SetPosition(myCollectibleString2->GetPosition() + v2f(50.f * aDeltaTime, 0));
			}
		}
	}
}

void UIPopUp::SetNewHardPositions(const float& aDeltaTime)
{
	Config::ourReferenceSize = { 320.f, 180.f };

	if (myHardActive)
	{
		if (myIsHMaxLeft == false)
		{
			if (myBackgroundH->GetStartPosition().x < Config::ourReferenceSize.x - 55.f)
			{
				myIsHMaxLeft = true;
			}
			myBackgroundH->SetPositionX(myBackgroundH->GetStartPosition().x - 50.f * aDeltaTime);
			myFireHard->SetPositionX(myFireHard->GetStartPosition().x - 50.f * aDeltaTime);
			myCollectibleString3->SetPosition(myCollectibleString3->GetPosition() - v2f(50.f * aDeltaTime, 0));
		}
		else if (myIsHMaxLeft == true)
		{
			myCurrentHStayTime += aDeltaTime;
			if (myCurrentHStayTime > myStayTime)
			{
				myBackgroundH->SetPositionX(myBackgroundH->GetPositionX() + 50.f * aDeltaTime);
				myFireHard->SetPositionX(myFireHard->GetPositionX() + 50.f * aDeltaTime);
				myCollectibleString3->SetPosition(myCollectibleString3->GetPosition() + v2f(50.f * aDeltaTime, 0));

			}
		}
	}
}


void UIPopUp::UpdateCollectibles()
{

	myCollectibleString->GetComponent<TextComponent>()->SetText(std::to_string(myCollectibleCollected[myLevelIndex][0]) + "/" + std::to_string(myCollectibleInfo[myLevelIndex][0]));
	myCollectibleString2->GetComponent<TextComponent>()->SetText(std::to_string(myCollectibleCollected[myLevelIndex][1]) + "/" + std::to_string(myCollectibleInfo[myLevelIndex][1]));
	myCollectibleString3->GetComponent<TextComponent>()->SetText(std::to_string(myCollectibleCollected[myLevelIndex][2]) + "/" + std::to_string(myCollectibleInfo[myLevelIndex][2]));

}
