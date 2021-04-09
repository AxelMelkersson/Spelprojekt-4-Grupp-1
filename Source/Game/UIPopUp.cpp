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

UIPopUp::UIPopUp(Scene* aLevelScene)
{
	myScene = aLevelScene;
	myCurrentTime = {};
	myMaxTime = 4.0f;
	myStayTime = 1.5f;
	myCurrentStayTime = {};
}

UIPopUp::~UIPopUp()
{

	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::PopUpMessageE);
	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::PopUpMessageM);
	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::PopUpMessageH);


}

void UIPopUp::InitPopUp()
{
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::PopUpMessageE);
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::PopUpMessageM);
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::PopUpMessageH);

	Config::ourReferenceSize = { 320.f, 180.f };



	myBackground = std::make_unique<UIObject>(myScene);
	myFireEasy = std::make_unique<UIObject>(myScene);
	myFireMed = std::make_unique<UIObject>(myScene);
	myFireHard = std::make_unique<UIObject>(myScene);

	v2f position = myScene->GetCamera().GetPosition();
	v2f position2 = myScene->GetCamera().GetViewportSize();
	v2f backPos = { position.x + Config::ourReferenceSize.x, position.y + 15.f };
	v2f firePos = { position.x + Config::ourReferenceSize.x + 15.f, position.y + 30.0f };


	myBackground->Init("Sprites/UI/popUp/UI_PopUp_84x32px.dds", { 84.0f, 32.0f }, backPos, 201);
	myFireEasy->InitAnimation("Sprites/Objects/Collectible3.dds", { 16.0f, 16.0f }, firePos, 202);
	myFireMed->InitAnimation("Sprites/Objects/Collectible2.dds", { 16.0f, 16.0f }, firePos, 202);
	myFireHard->InitAnimation("Sprites/Objects/Collectible1.dds", { 16.0f, 16.0f }, firePos, 202);

	//for (int i = 0; i < 8; ++i)
	//{
	//	myCollectibleInfo.push_back(std::vector<int>());
	//	myCollectibleCollected.push_back(std::vector<int>());

	//	for (int difficulty = 0; difficulty < 3; ++difficulty)
	//	{
	//		myCollectibleInfo[i].push_back(0);
	//		myCollectibleCollected[i].push_back(0);
	//	}
	//}

	//for (int j = 0; j < DataManager::GetInstance().GetCollectableCount(); ++j)
	//{
	//	DataManager::GetInstance().GetCollectableInfoIndex(j);

	//	++myCollectibleInfo[DataManager::GetInstance().GetCollectableInfoIndex(j).myBonfireID][DataManager::GetInstance().GetCollectableInfoIndex(j).myDifficulty];

	//	if (DataManager::GetInstance().GetCollectableInfoIndex(j).myCollectedState)
	//	{
	//		++myCollectibleCollected[DataManager::GetInstance().GetCollectableInfoIndex(j).myBonfireID][DataManager::GetInstance().GetCollectableInfoIndex(j).myDifficulty];

	//	}
	//}

	//myCollectibleInfo.clear();
	//myCollectibleCollected.clear();

	//myLevelCollectibles.push_back(new UIText(myScene));
	//myLevelCollectibles.push_back(new UIText(myScene));
	//myLevelCollectibles.push_back(new UIText(myScene));


	//myLevelCollectibles[0]->Init(std::to_string(myCollectibleCollected[0][0]) + "/" + std::to_string(myCollectibleInfo[0][0]), "Text/Peepo.ttf", EFontSize_48);
	//myLevelCollectibles[0]->SetPosition(v2f(290.0f, 30.0f));
	//myLevelCollectibles[0]->GetComponent<TextComponent>()->SetColor(Tga2D::CColor(0.0f, 0.5f, 1.0f, 1.0f));

	//myLevelCollectibles[1]->Init(std::to_string(myCollectibleCollected[0][1]) + "/" + std::to_string(myCollectibleInfo[0][1]), "Text/Peepo.ttf", EFontSize_48);
	//myLevelCollectibles[1]->SetPosition(v2f(290.0f, 30.0f));
	//myLevelCollectibles[1]->GetComponent<TextComponent>()->SetColor(Tga2D::CColor(0.0f, 0.5f, 1.0f, 1.0f));

	//myLevelCollectibles[2]->Init(std::to_string(myCollectibleCollected[0][2]) + "/" + std::to_string(myCollectibleInfo[0][2]), "Text/Peepo.ttf", EFontSize_48);
	//myLevelCollectibles[2]->SetPosition(v2f(290.0f, 30.0f));
	//myLevelCollectibles[2]->GetComponent<TextComponent>()->SetColor(Tga2D::CColor(0.0f, 0.5f, 1.0f, 1.0f));

}

void UIPopUp::Update(const float& aDeltaTime)
{
	if (myEasyActive)
	{
		myCurrentTime += aDeltaTime;

		myBackground->UpdateUIObjects(aDeltaTime);
		myFireEasy->UpdateUIObjects(aDeltaTime);
		SetNewPositions(aDeltaTime);
	}
	if (myMedActive)
	{
		myCurrentTime += aDeltaTime;
		myBackground->UpdateUIObjects(aDeltaTime);
		myFireMed->UpdateUIObjects(aDeltaTime);
		SetNewPositions(aDeltaTime);

	}
	if (myHardActive == true)
	{
		myCurrentTime += aDeltaTime;
		myBackground->UpdateUIObjects(aDeltaTime);
		myFireHard->UpdateUIObjects(aDeltaTime);
		SetNewPositions(aDeltaTime);

	}
	if (myCurrentTime > myMaxTime)
	{
		ResetObjects();
		Deactivate();
	}
}

void UIPopUp::Activate(ePopUpTypes aType)
{
	DataManager::GetInstance().GetCollectableCount();

	switch (aType)
	{
	case ePopUpTypes::Easy:
		myFireEasy->SetActive(true);
		myBackground->SetActive(true);
		myEasyActive = true;
		break;
	case ePopUpTypes::Med:
		myFireMed->SetActive(true);
		myBackground->SetActive(true);
		myMedActive = true;
		break;
	case ePopUpTypes::Hard:
		myFireHard->SetActive(true);
		myBackground->SetActive(true);
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
	myBackground->SetActive(false);
	myCurrentTime = 0.0f;

	myFireEasy->SetActive(false);
	myFireMed->SetActive(false);
	myFireHard->SetActive(false);

}

void UIPopUp::SetNewPositions(const float& aDeltaTime)
{
	if (myEasyActive)
	{
		if (myIsMaxLeft == false)
		{
			if (myBackground->GetPositionX() < 265.f)
			{
				myIsMaxLeft = true;
			}
			myBackground->SetPositionX(myBackground->GetPositionX() - 50.f * aDeltaTime);
			myFireEasy->SetPositionX(myFireEasy->GetPositionX() - 50.f * aDeltaTime);
		}
		else if (myIsMaxLeft == true)
		{
			myCurrentStayTime += aDeltaTime;
			if (myCurrentStayTime > myStayTime)
			{
				myBackground->SetPositionX(myBackground->GetPositionX() + 50.f * aDeltaTime);
				myFireEasy->SetPositionX(myFireEasy->GetPositionX() + 50.f * aDeltaTime);

			}
		}

	}
	if (myMedActive)
	{
		if (myIsMaxLeft == false)
		{
			if (myBackground->GetPositionX() < 265.f)
			{
				myIsMaxLeft = true;
			}
			myBackground->SetPositionX(myBackground->GetPositionX() - 50.f * aDeltaTime);
			myFireMed->SetPositionX(myFireMed->GetPositionX() - 50.f * aDeltaTime);
		}
		else if (myIsMaxLeft == true)
		{
			myCurrentStayTime += aDeltaTime;
			if (myCurrentStayTime > myStayTime)
			{
				myBackground->SetPositionX(myBackground->GetPositionX() + 50.f * aDeltaTime);
				myFireMed->SetPositionX(myFireMed->GetPositionX() + 50.f * aDeltaTime);

			}
		}
	}
	if (myHardActive)
	{
		if (myIsMaxLeft == false)
		{
			if (myBackground->GetPositionX() < 265.f)
			{
				myIsMaxLeft = true;
			}
			myBackground->SetPositionX(myBackground->GetPositionX() - 50.f * aDeltaTime);
			myFireHard->SetPositionX(myFireHard->GetPositionX() - 50.f * aDeltaTime);
		}
		else if (myIsMaxLeft == true)
		{
			myCurrentStayTime += aDeltaTime;
			if (myCurrentStayTime > myStayTime)
			{
				myBackground->SetPositionX(myBackground->GetPositionX() + 50.f * aDeltaTime);
				myFireHard->SetPositionX(myFireHard->GetPositionX() + 50.f * aDeltaTime);

			}
		}
	}


}


void UIPopUp::ResetObjects()
{
	v2f position = myScene->GetCamera().GetPosition();
	v2f backPos = { position.x + Config::ourReferenceSize.x, position.y + 15.f };
	v2f firePos = { position.x + Config::ourReferenceSize.x + 15.f, position.y + 30.0f };


	myCurrentStayTime = 0.f;
	myIsMaxLeft = false;

	myBackground->SetPosition(backPos);
	myFireEasy->SetPosition(firePos);
	myFireMed->SetPosition(firePos);
	myFireHard->SetPosition(firePos);

	myEasyActive = false;
	myMedActive = false;
	myHardActive = false;
}


