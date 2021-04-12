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
{
	myScene = aLevelScene;
	myCurrentTime = {};
	myMaxTime = 7.0f;
	myStayTime = 1.5f;
	myCurrentStayTime = {};
	myLevelIndex = 0;
}

UIPopUp::~UIPopUp()
{
	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::PopUpMessageE);
	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::PopUpMessageM);
	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::PopUpMessageH);
}

void UIPopUp::InitPopUp()
{
	myCollectibleInfo.clear();
	myCollectibleCollected.clear();

	PostMaster::GetInstance().AddSubcriber(this, eMessageType::PopUpMessageE);
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::PopUpMessageM);
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::PopUpMessageH);

	Config::ourReferenceSize = { 320.f, 180.f };

	myBackground = std::make_unique<UIObject>(myScene);
	myFireEasy = std::make_unique<UIObject>(myScene);
	myFireMed = std::make_unique<UIObject>(myScene);
	myFireHard = std::make_unique<UIObject>(myScene);

	v2f backPos = { Config::ourReferenceSize.x, 15.f };
	v2f firePos = { Config::ourReferenceSize.x + 10.f, Config::ourReferenceSize.y - 150.0f };
	v2f collectiblePos = { Config::ourReferenceSize.x + 25.f, 35.0f };



	myBackground->Init("Sprites/UI/popUp/UI_PopUp_84x32px.dds", { 84.0f, 32.0f }, backPos, 201);
	myFireEasy->InitAnimation("Sprites/Objects/Collectible3.dds", { 16.0f, 16.0f }, 7, 7, firePos, 202);
	myFireMed->InitAnimation("Sprites/Objects/Collectible2.dds", { 16.0f, 16.0f }, 7, 7, firePos, 202);
	myFireHard->InitAnimation("Sprites/Objects/Collectible1.dds", { 16.0f, 16.0f }, 7, 7, firePos, 202);

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

	myCollectibleString = std::make_unique<UIText>(myScene);
	myCollectibleString->Init(std::to_string(myCollectibleCollected[0][0]) + "/" + std::to_string(myCollectibleInfo[0][0]), "Text/Peepo.ttf", EFontSize_48);
	myCollectibleString->SetPosition(collectiblePos);
	myCollectibleString->SetZIndex(202);
	myCollectibleString2 = std::make_unique<UIText>(myScene);
	myCollectibleString2->Init(std::to_string(myCollectibleCollected[0][1]) + "/" + std::to_string(myCollectibleInfo[0][1]), "Text/Peepo.ttf", EFontSize_48);
	myCollectibleString2->SetPosition(collectiblePos);
	myCollectibleString3 = std::make_unique<UIText>(myScene);
	myCollectibleString3->Init(std::to_string(myCollectibleCollected[0][2]) + "/" + std::to_string(myCollectibleInfo[0][2]), "Text/Peepo.ttf", EFontSize_48);
	myCollectibleString3->SetPosition(collectiblePos);

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
	if (myEasyActive)
	{
		myCurrentTime += aDeltaTime;

		UpdateCollectibles();
		SetNewPositions(aDeltaTime);
		myBackground->UpdateUIObjects(aDeltaTime);
		myFireEasy->UpdateUIObjects(aDeltaTime);

	}
	else if (myMedActive)
	{
		myCurrentTime += aDeltaTime;
		myBackground->UpdateUIObjects(aDeltaTime);
		myFireMed->UpdateUIObjects(aDeltaTime);
		UpdateCollectibles();
		SetNewPositions(aDeltaTime);

	}
	else if (myHardActive == true)
	{
		myCurrentTime += aDeltaTime;
		myBackground->UpdateUIObjects(aDeltaTime);
		myFireHard->UpdateUIObjects(aDeltaTime);
		UpdateCollectibles();
		SetNewPositions(aDeltaTime);
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
		Deactivate();
		myEasyActive = false;
		myMedActive = false;
		myHardActive = false;
	}

}

void UIPopUp::Activate(ePopUpTypes aType)
{
	switch (aType)
	{
	case ePopUpTypes::Easy:
		myFireEasy->SetActive(true);
		myBackground->SetActive(true);
		myCollectibleString->Activate();
		myEasyActive = true;
		break;
	case ePopUpTypes::Med:
		myFireMed->SetActive(true);
		myBackground->SetActive(true);
		myCollectibleString2->Activate();
		myMedActive = true;
		break;
	case ePopUpTypes::Hard:
		myFireHard->SetActive(true);
		myBackground->SetActive(true);
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

void UIPopUp::Deactivate()
{
	myCurrentTime = 0.0f;
	myCurrentStayTime = 0.f;
	myIsMaxLeft = false;
	myBackground->SetActive(false);
	myFireEasy->SetActive(false);
	myFireMed->SetActive(false);
	myFireHard->SetActive(false);
	myCollectibleString->Deactivate();
	myCollectibleString2->Deactivate();
	myCollectibleString3->Deactivate();

}

void UIPopUp::SetNewPositions(const float& aDeltaTime)
{
	Config::ourReferenceSize = { 320.f, 180.f };

	if (myEasyActive)
	{
		if (myIsMaxLeft == false)
		{
			if (myBackground->GetStartPosition().x < Config::ourReferenceSize.x - 55.f)
			{ 

				myIsMaxLeft = true;
			}
			else
			{
				myBackground->SetPositionX(myBackground->GetStartPosition().x - 50.f * aDeltaTime);
				myFireEasy->SetPositionX(myFireEasy->GetStartPosition().x - 50.f * aDeltaTime);
				myCollectibleString->SetPosition(myCollectibleString->GetPosition() - v2f(50.f * aDeltaTime, 0));
			}
		}
		else if (myIsMaxLeft == true)
		{
			myCurrentStayTime += aDeltaTime;
			if (myCurrentStayTime > myStayTime)
			{
				myBackground->SetPositionX(myBackground->GetStartPosition().x + 50.f * aDeltaTime);
				myFireEasy->SetPositionX(myFireEasy->GetStartPosition().x + 50.f * aDeltaTime);
				myCollectibleString->SetPosition(myCollectibleString->GetPosition() + v2f(50.f * aDeltaTime, 0));
			}
		}

	}
	if (myMedActive)
	{
		if (myIsMaxLeft == false)
		{
			if (myBackground->GetPositionX() < Config::ourReferenceSize.x - 55.f)
			{
				myIsMaxLeft = true;
			}
			myBackground->SetPositionX(myBackground->GetStartPosition().x - 50.f * aDeltaTime);
			myFireMed->SetPositionX(myFireMed->GetStartPosition().x - 50.f * aDeltaTime);
			myCollectibleString2->SetPosition(myCollectibleString2->GetPosition() - v2f(50.f * aDeltaTime, 0));
		}
		else if (myIsMaxLeft == true)
		{
			myCurrentStayTime += aDeltaTime;
			if (myCurrentStayTime > myStayTime)
			{
				myBackground->SetPositionX(myBackground->GetStartPosition().x + 50.f * aDeltaTime);
				myFireMed->SetPositionX(myFireMed->GetStartPosition().x + 50.f * aDeltaTime);
				myCollectibleString2->SetPosition(myCollectibleString2->GetPosition() + v2f(50.f * aDeltaTime, 0));
			}
		}
	}
	if (myHardActive)
	{
		if (myIsMaxLeft == false)
		{
			if (myBackground->GetPositionX() < Config::ourReferenceSize.x - 55.f)
			{
				myIsMaxLeft = true;
			}
			myBackground->SetPositionX(myBackground->GetStartPosition().x - 50.f * aDeltaTime);
			myFireHard->SetPositionX(myFireHard->GetStartPosition().x - 50.f * aDeltaTime);
			myCollectibleString3->SetPosition(myCollectibleString3->GetPosition() - v2f(50.f * aDeltaTime, 0));
		}
		else if (myIsMaxLeft == true)
		{
			myCurrentStayTime += aDeltaTime;
			if (myCurrentStayTime > myStayTime)
			{
				myBackground->SetPositionX(myBackground->GetPositionX() + 50.f * aDeltaTime);
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
