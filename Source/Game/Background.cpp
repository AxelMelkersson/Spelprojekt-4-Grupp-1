#include "stdafx.h"
#include "Background.h"
#include "SpriteComponent.h"
#include "Camera.h"
#include "LevelScene.h"
#include "Player.hpp"
#include "AudioManager.h"
#include "rapidjson/istreamwrapper.h"
#include "CameraStaticDistance.hpp"
#include <iostream>

#include "Game.h"


Background::Background(Scene* aLevelScene)
	:
	GameObject(aLevelScene)
{
	SetZIndex(0);

	myGetReferences = {};
	myBackgroundSpeedOneX = {};
	myBackgroundSpeedTwoX = {};
	myBackgroundSpeedThreeX = {};
	myBackgroundSpeedOneY = {};
	myBackgroundSpeedTwoY = {};
	myBackgroundSpeedThreeY = {};
	myBackgroundSpeedSevenX = {};
	myBackgroundSpeedSevenY = {};
	myAddedCameraPos = {};
	myCloudSpeed = {};
	myTotalCameraDistanceX = {};
	myStartingPlayerPos = {};


	myAddedCameraPosX = {};


	myStartingCameraX = -1;

	myCloudDistance = &Distance::myCloudDistance;
	myBackgroundDistanceX = &Distance::myBackgroundDistanceX;
	myStartingCameraPos.x = -1;
	myStartingCameraPos.y = -1;

	assert(&aLevelScene->GetCamera() != NULL);
	myLevelScene = dynamic_cast<LevelScene*>(aLevelScene);
}

Background::~Background()
{
	if (myPlayer != NULL)
	{
		float currentPlayerPos = myPlayer->GetPositionX();
		float calculatedPlayerPos = currentPlayerPos - myStartingPlayerPos;

		if (calculatedPlayerPos > (myCamera->GetBoundSize().x / 2.f) && myTotalCameraDistanceX != 0)
		{
			Distance::myBackgroundDistanceX += myTotalCameraDistanceX;
		}
		else if (calculatedPlayerPos < -(myCamera->GetBoundSize().x / 2.f) && myTotalCameraDistanceX != 0)
		{
			Distance::myBackgroundDistanceX -= myTotalCameraDistanceX;
		}
	}
}

void Background::LoadBackground(const int aIndex)
{
	if (Distance::myCurrentAreaIndex != aIndex)
	{
		Distance::myCurrentAreaIndex = aIndex;
		Distance::myBackgroundDistanceX = {};
	}


	std::ifstream backgroundObjectFile("JSON/Background.json");
	rapidjson::IStreamWrapper backgroundObjectStream(backgroundObjectFile);

	rapidjson::Document backgroundDocuments;
	backgroundDocuments.ParseStream(backgroundObjectStream);


	std::ifstream backgroundPathObjectsFile(backgroundDocuments["BackgroundArray"][aIndex]["FilePath"].GetString());
	rapidjson::IStreamWrapper backgroundPathObjectStream(backgroundPathObjectsFile);

	rapidjson::Document backgroundPathDocuments;
	backgroundPathDocuments.ParseStream(backgroundPathObjectStream);

	LoadBackgrounds(myLevelScene, backgroundPathDocuments);

	myBackgroundSpeedOneX = backgroundDocuments["SpeedVariables"][0]["SpeedOneX"].GetFloat();
	myBackgroundSpeedOneY = backgroundDocuments["SpeedVariables"][0]["SpeedOneY"].GetFloat();

	myBackgroundSpeedTwoX = backgroundDocuments["SpeedVariables"][1]["SpeedTwoX"].GetFloat();
	myBackgroundSpeedTwoY = backgroundDocuments["SpeedVariables"][1]["SpeedTwoY"].GetFloat();

	myBackgroundSpeedThreeX = backgroundDocuments["SpeedVariables"][2]["SpeedThreeX"].GetFloat();
	myBackgroundSpeedThreeY = backgroundDocuments["SpeedVariables"][2]["SpeedThreeY"].GetFloat();

	myBackgroundSpeedFourX = backgroundDocuments["SpeedVariables"][3]["SpeedFourX"].GetFloat();
	myBackgroundSpeedFourY = backgroundDocuments["SpeedVariables"][3]["SpeedFourY"].GetFloat();

	myBackgroundSpeedFiveX = backgroundDocuments["SpeedVariables"][4]["SpeedFiveX"].GetFloat();
	myBackgroundSpeedFiveY = backgroundDocuments["SpeedVariables"][4]["SpeedFiveY"].GetFloat();

	myBackgroundSpeedSixX = backgroundDocuments["SpeedVariables"][5]["SpeedSixX"].GetFloat();
	myBackgroundSpeedSixY = backgroundDocuments["SpeedVariables"][5]["SpeedSixY"].GetFloat();

	myBackgroundSpeedSevenX = backgroundDocuments["SpeedVariables"][6]["SpeedSevenX"].GetFloat();
	myBackgroundSpeedSevenY = backgroundDocuments["SpeedVariables"][6]["SpeedSevenY"].GetFloat();

	myCloudSpeed = backgroundDocuments["Cloud"][0]["SpeedX"].GetFloat();

	backgroundPathObjectsFile.close();
	backgroundObjectFile.close();
}

void Background::Update(const float& aDeltaTime)
{
	CheckReferences();
	AddStartingCameraPos();
	UpdateBackground(aDeltaTime);
}

const void Background::UpdateBackground(const float& aDeltaTime)
{
	if (myCamera->GetActive() && myAddedCameraPos == false)
	{
		if (myStartingCameraPos.y == myCamera->GetPositionY())
		{
			myStartingPlayerPos = myPlayer->GetPositionX();
			myAddedCameraPos = true;
		}

		myStartingCameraPos = { *myBackgroundDistanceX, myCamera->GetPositionY() };
	}

	MoveBackground(aDeltaTime);
}


const void Background::MoveBackground(const float& aDeltaTime)
{
	CalculateCameraPositions(aDeltaTime);

	myTotalCameraDistanceX = Distance::myStartingCameraPos + myCamera->GetPositionX();
}

const void Background::LoadBackgrounds(Scene* aLevelScene, rapidjson::Document& someDocuments)
{
	int backgroundSizeArray = someDocuments["BackgroundPaths"].Size();

	for (int i = 0; i < backgroundSizeArray; i++)
	{
		v2f offset = { someDocuments["OffSets"][i]["PositionX"].GetFloat() , someDocuments["OffSets"][i]["PositionY"].GetFloat() };
		CreateBackgrounds(aLevelScene, someDocuments["BackgroundPaths"][i]["FilePath"].GetString(), i, offset);
	}
}

const void Background::CreateBackgrounds(Scene* aLevelScene, const std::string aPath, const int aIndex, const v2f anOffset)
{
	switch (aIndex)
	{
	case 0:
	{
		myBackgroundSprite1 = new GameObject(aLevelScene);
		SpriteComponent* sprite = myBackgroundSprite1->AddComponent<SpriteComponent>();
		sprite->SetSpritePath(aPath);
		myBackgroundSprite1->SetZIndex(0);
		myOffsetBackground1 = anOffset;
		break;
	}
	case 1:
	{
		myBackgroundSprite2 = new GameObject(aLevelScene);
		SpriteComponent* sprite = myBackgroundSprite2->AddComponent<SpriteComponent>();
		sprite->SetSpritePath(aPath);
		myBackgroundSprite2->SetZIndex(1);
		myOffsetBackground2 = anOffset;
		break;
	}
	case 2:
	{
		myBackgroundSprite3 = new GameObject(aLevelScene);
		SpriteComponent* sprite = myBackgroundSprite3->AddComponent<SpriteComponent>();
   		sprite->SetSpritePath(aPath);
		myBackgroundSprite3->SetZIndex(2);
		myOffsetBackground3 = anOffset;
		break;
	}
	case 3:
	{
		myBackgroundSprite4 = new GameObject(aLevelScene);
		SpriteComponent* sprite = myBackgroundSprite4->AddComponent<SpriteComponent>();
		sprite->SetSpritePath(aPath);
		myBackgroundSprite4->SetZIndex(3);
		myOffsetBackground4 = anOffset;
		break;
	}
	case 4:
	{
		myBackgroundSprite5 = new GameObject(aLevelScene);
		mySpriteComp5.mySpriteOne = myBackgroundSprite5->AddComponent<SpriteComponent>();
		mySpriteComp5.mySpriteOne->SetSpritePath(aPath);
		mySpriteComp5.mySpriteTwo = myBackgroundSprite5->AddComponent<SpriteComponent>();
		mySpriteComp5.mySpriteTwo->SetSpritePath(aPath);
		mySpriteComp5.mySpriteTwo->SetRelativePosition({ mySpriteComp5.mySpriteOne->GetImageSize().x, 0.f });
		mySpriteComp5.mySpriteZoom = mySpriteComp5.mySpriteOne->GetImageSize().x / Config::ourReferenceSize.x;
		mySpriteComp5.myMovedRightSprite = true;
		myBackgroundSprite5->SetZIndex(4);
		myOffsetBackground5 = anOffset;
		break;
	}
	case 5:
	{
		myBackgroundSprite6 = new GameObject(aLevelScene);
		mySpriteComp6.mySpriteOne = myBackgroundSprite6->AddComponent<SpriteComponent>();
		mySpriteComp6.mySpriteOne->SetSpritePath(aPath);
		mySpriteComp6.mySpriteTwo = myBackgroundSprite6->AddComponent<SpriteComponent>();
		mySpriteComp6.mySpriteTwo->SetSpritePath(aPath);
		mySpriteComp6.mySpriteTwo->SetRelativePosition({ mySpriteComp6.mySpriteOne->GetImageSize().x, 0.f });
		mySpriteComp6.mySpriteZoom = mySpriteComp6.mySpriteOne->GetImageSize().x / Config::ourReferenceSize.x;
		mySpriteComp6.myMovedRightSprite = true;
		myBackgroundSprite6->SetZIndex(5);
		myOffsetBackground6 = anOffset;
		break;
	}
	case 6:
	{
		myBackgroundSprite7 = new GameObject(aLevelScene);
		mySpriteComp7.mySpriteOne = myBackgroundSprite7->AddComponent<SpriteComponent>();
		mySpriteComp7.mySpriteOne->SetSpritePath(aPath);
		mySpriteComp7.mySpriteOne->SetRelativePosition({ GetHalfImageSize(myBackgroundSprite7).x, 0.f });
		//mySpriteComp7.mySpriteTwo = myBackgroundSprite7->AddComponent<SpriteComponent>();
		//mySpriteComp7.mySpriteTwo->SetSpritePath(aPath);
		//mySpriteComp7.mySpriteTwo->SetRelativePosition({ mySpriteComp7.mySpriteOne->GetImageSize().x, 0.f });
		mySpriteComp7.mySpriteZoom = mySpriteComp7.mySpriteOne->GetImageSize().x / Config::ourReferenceSize.x;
		//mySpriteComp7.myMovedRightSprite = true;
		myBackgroundSprite7->SetZIndex(6);
		myOffsetBackground7 = anOffset;
		break;
	}
	default:
	{
		break;
	}
	}

}


const void Background::AddStartingCameraPos()
{
	if (myStartingCameraX != myCamera->GetPosition().x && myAddedCameraPosX == false)
	{
		myStartingCameraX = myCamera->GetPosition().x;
	}
	else
	{
		myAddedCameraPosX = true;
	}
}

const void Background::CalculateCameraPositions(const float& aDeltaTime)
{
	v2f cameraPos = myCamera->GetPosition();
	cameraPos.x = cameraPos.x - myStartingCameraX + myStartingCameraPos.x;
	cameraPos.y = cameraPos.y - myStartingCameraPos.y;

	v2f backgroundSpeedOne;
	backgroundSpeedOne = { -(cameraPos.x) * myBackgroundSpeedOneX,
						   (cameraPos.y) * myBackgroundSpeedOneY };

	v2f backgroundSpeedThree;
	backgroundSpeedThree = { -(cameraPos.x) * myBackgroundSpeedThreeX,
							 -(cameraPos.y) * myBackgroundSpeedThreeY };

	v2f backgroundSpeedFour;
	backgroundSpeedFour = { -(cameraPos.x) * myBackgroundSpeedFourX,
							-(cameraPos.y) * myBackgroundSpeedFourY };

	v2f backgroundSpeedFive;
	backgroundSpeedFive = { -(cameraPos.x) * myBackgroundSpeedFiveX,
						   -(cameraPos.y) * myBackgroundSpeedFiveY };

	v2f backgroundSpeedSix;
	backgroundSpeedSix = { -(cameraPos.x) * myBackgroundSpeedSixX,
						  -(cameraPos.y) * myBackgroundSpeedSixY };

	v2f backgroundSpeedSeven;
	backgroundSpeedSeven = { -(cameraPos.x) * myBackgroundSpeedSevenX,
						  -(cameraPos.y) * myBackgroundSpeedSevenY };


	v2f cameraPosition = myCamera->GetPosition();

	myBackgroundSprite1->SetPosition(cameraPosition + backgroundSpeedOne + GetHalfImageSize(myBackgroundSprite1) + myOffsetBackground1);
	myBackgroundSprite3->SetPosition(cameraPosition + backgroundSpeedThree + GetHalfImageSize(myBackgroundSprite3) + myOffsetBackground3);
	myBackgroundSprite4->SetPosition(cameraPosition + backgroundSpeedFour + GetHalfImageSize(myBackgroundSprite4) + myOffsetBackground4);
	myBackgroundSprite5->SetPosition(cameraPosition + backgroundSpeedFive + GetHalfImageSize(myBackgroundSprite5) + myOffsetBackground5);
	myBackgroundSprite6->SetPosition(cameraPosition + backgroundSpeedSix + GetHalfImageSize(myBackgroundSprite6) + myOffsetBackground6);
	//myBackgroundSprite7->SetPosition(cameraPosition + backgroundSpeedSeven + GetHalfImageSize(myBackgroundSprite7) + myOffsetBackground7);
	//myBackgroundSprite5->SetPosition({ myBackgroundSprite5->GetPositionX() + -40.f * aDeltaTime, GetHalfImageSize(myBackgroundSprite5).y });
	//myBackgroundSprite6->SetPosition({ myBackgroundSprite6->GetPositionX() + -40.f * aDeltaTime, GetHalfImageSize(myBackgroundSprite6).y });
	myBackgroundSprite7->SetPosition({ myBackgroundSprite7->GetPositionX() + 40.f * aDeltaTime, GetHalfImageSize(myBackgroundSprite7).y });

	

	*myCloudDistance = *myCloudDistance + (aDeltaTime * myCloudSpeed);

	v2f backgroundSpeedTwo = { *myCloudDistance, 0.f };

	myBackgroundSprite2->SetPosition(cameraPosition + backgroundSpeedTwo + GetHalfImageSize(myBackgroundSprite2) + myOffsetBackground2);

	if (*myCloudDistance < -960)
		*myCloudDistance = 220.f;
}

const void Background::ResetDistanceValues()
{
	Distance::myBackgroundDistanceX = {};
}

const void Background::CheckResetLoop()
{

}


const v2f Background::GetHalfImageSize(GameObject* aSprite)
{
	assert(aSprite->GetComponent<SpriteComponent>() != NULL);

	v2f spriteSize = aSprite->GetComponent<SpriteComponent>()->GetImageSize() / 2.f;

	return spriteSize;
}

const void Background::CheckReferences()
{
	if (!myGetReferences)
	{
		myLevelScene = dynamic_cast<LevelScene*>(myLevelScene);
		myPlayer = dynamic_cast<Player*>(myLevelScene->GetPlayer());
		myCamera = &myLevelScene->GetCamera();
		myGetReferences = true;
	}
}
