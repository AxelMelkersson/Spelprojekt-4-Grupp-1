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
	myCurrentTime = 0;
	myCurrentStayTime = 0;
	myMaxTime = 4.0f;
	myStayTime = 1.5f;
}

void UIPopUp::InitPopUp()
{
	myCollectibleInfo.clear();
	myCollectibleCollected.clear();

	Subscribe(eMessageType::PopUpMessageE);
	Subscribe(eMessageType::PopUpMessageM);
	Subscribe(eMessageType::PopUpMessageH);

	Config::ourReferenceSize = { 320.f, 180.f };

	myBackground = new UIObject(myScene);

	v2f backPos = { Config::ourReferenceSize.x - 5, 15.f };
	v2f firePos = { Config::ourReferenceSize.x + 10.f, Config::ourReferenceSize.y - 150.0f };
	v2f collectiblePos = { Config::ourReferenceSize.x + 25.f, 35.0f };

	myFires.push_back(new UIObject(myScene));
	myFires.push_back(new UIObject(myScene));
	myFires.push_back(new UIObject(myScene));

	myBackground->Init("Sprites/UI/popUp/UI_collectiblePickupScreen.dds", { 84.0f, 32.0f }, backPos, 200);

	myFires[0]->InitAnimation("Sprites/Objects/Collectible3.dds", { 16.0f, 16.0f }, 7, 7, firePos, 201);
	myFires[1]->InitAnimation("Sprites/Objects/Collectible2.dds", { 16.0f, 16.0f }, 7, 7, firePos, 201);
	myFires[2]->InitAnimation("Sprites/Objects/Collectible1.dds", { 16.0f, 16.0f }, 7, 7, firePos, 201);

	myCollectibleInfo.push_back(0);
	myCollectibleInfo.push_back(0);
	myCollectibleInfo.push_back(0);

	myCollectibleCollected.push_back(0);
	myCollectibleCollected.push_back(0);
	myCollectibleCollected.push_back(0);

	for (int j = 0; j < DataManager::GetInstance().GetCollectableCount(); ++j)
	{
		CollectableInfo collectibleInfo = DataManager::GetInstance().GetCollectableInfoIndex(j);

		++myCollectibleInfo[collectibleInfo.myDifficulty];

		if (collectibleInfo.myCollectedState)
		{
			++myCollectibleCollected[collectibleInfo.myDifficulty];
		}
	}

	myCollectibleString = new UIText(myScene);
	myCollectibleString->Init(std::to_string(myCollectibleCollected[0]) + "/" + std::to_string(myCollectibleInfo[0]), "Text/Peepo.ttf", EFontSize_48);
	myCollectibleString->SetPosition(collectiblePos);
	myCollectibleString->SetZIndex(201);
}


void UIPopUp::Update(const float& aDeltaTime)
{
	if (static_cast<int>(myShowQueue.size()) <= 0)
	{
		return;
	}

	myCollectibleString->SetText(myShowQueueText[0]);

	myFires[myShowQueue[0]]->SetActive(true);
	myBackground->SetActive(true);
	myCollectibleString->Activate();

	myCurrentTime += aDeltaTime;
	SetNewPositions(aDeltaTime);
	myBackground->UpdateUIObjects(aDeltaTime);
	myFires[myShowQueue[0]]->UpdateUIObjects(aDeltaTime);

	if (myCurrentTime > myMaxTime)
	{
		Deactivate(static_cast<ePopUpTypes>(myShowQueue[0]));
		myShowQueue.erase(myShowQueue.begin() + 0);
		myShowQueueText.erase(myShowQueueText.begin() + 0);
	}

}

void UIPopUp::Activate(ePopUpTypes aType)
{
	myShowQueue.push_back(static_cast<int>(aType));

	UpdateCollectibles();
}

void UIPopUp::Notify(const Message& aMessage)
{
	switch (aMessage.myMessageType)
	{
		case eMessageType::PopUpMessageE:
			Activate(ePopUpTypes::Easy);
			break;
		case eMessageType::PopUpMessageM:
			Activate(ePopUpTypes::Med);
			break;
		case eMessageType::PopUpMessageH:
			Activate(ePopUpTypes::Hard);
			break;
	}
}

void UIPopUp::Deactivate(ePopUpTypes aType)
{
	myCurrentTime = 0.0f;
	myCurrentStayTime = 0.f;

	myIsMaxLeft = false;
	
	myBackground->SetActive(false);
	myCollectibleString->Deactivate();

	v2f backPos = { Config::ourReferenceSize.x - 5.0f, 15.f };
	v2f firePos = { Config::ourReferenceSize.x + 10.f, Config::ourReferenceSize.y - 150.0f };
	v2f collectiblePos = { Config::ourReferenceSize.x + 25.f, 35.0f };

	myBackground->SetPosition(backPos);
	myCollectibleString->SetPosition(collectiblePos);

	myFires[0]->SetActive(false);
	myFires[1]->SetActive(false);
	myFires[2]->SetActive(false);

	myFires[0]->SetPosition(firePos);
	myFires[1]->SetPosition(firePos);
	myFires[2]->SetPosition(firePos);
}

void UIPopUp::SetNewPositions(const float& aDeltaTime)
{
	Config::ourReferenceSize = { 320.f, 180.f };

	if (static_cast<int>(myShowQueue.size()) > 0)
	{
		if (myIsMaxLeft == false)
		{
			if (myBackground->GetStartPosition().x < Config::ourReferenceSize.x - 60.f)
			{
				myIsMaxLeft = true;
			}
			else
			{
				myBackground->SetPositionX(myBackground->GetStartPosition().x - 50.f * aDeltaTime);
				myFires[myShowQueue[0]]->SetPositionX(myFires[myShowQueue[0]]->GetStartPosition().x - 50.f * aDeltaTime);
				myCollectibleString->SetPosition(myCollectibleString->GetPosition() - v2f(50.f * aDeltaTime, 0));
			}
		}
		else if (myIsMaxLeft == true)
		{
			myCurrentStayTime += aDeltaTime;
			if (myCurrentStayTime > myStayTime)
			{
				myBackground->SetPositionX(myBackground->GetStartPosition().x + 50.f * aDeltaTime);
				myFires[myShowQueue[0]]->SetPositionX(myFires[myShowQueue[0]]->GetStartPosition().x + 50.f * aDeltaTime);
				myCollectibleString->SetPosition(myCollectibleString->GetPosition() + v2f(50.f * aDeltaTime, 0));
			}
		}
	}
}


void UIPopUp::UpdateCollectibles()
{
	if (static_cast<int>(myShowQueue.size()) <= 0)
	{
		return;
	}

	myCollectibleInfo.clear();
	myCollectibleCollected.clear();

	myCollectibleInfo.push_back(0);
	myCollectibleInfo.push_back(0);
	myCollectibleInfo.push_back(0);

	myCollectibleCollected.push_back(0);
	myCollectibleCollected.push_back(0);
	myCollectibleCollected.push_back(0);

	for (int j = 0; j < DataManager::GetInstance().GetCollectableCount(); ++j)
	{
		CollectableInfo collectibleInfo = DataManager::GetInstance().GetCollectableInfoIndex(j);

		++myCollectibleInfo[collectibleInfo.myDifficulty];

		if (collectibleInfo.myCollectedState)
		{
			++myCollectibleCollected[collectibleInfo.myDifficulty];
		}
	}

	myShowQueueText.push_back(std::to_string(myCollectibleCollected[myShowQueue[static_cast<int>(myShowQueue.size() - 1)]]) + "/" + std::to_string(myCollectibleInfo[myShowQueue[static_cast<int>(myShowQueue.size() - 1)]]));
}
