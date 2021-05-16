#include "stdafx.h"
#include "LevelSelect.hpp"

#include "InputWrapper.h"
#include "AudioManager.h"

#include "GameWorld.h"
#include "Game.h"
#include "LevelManager.hpp"

#include "DataManager.h"

#include "SpriteComponent.h"
#include "AnimationComponent.hpp"
#include "TextComponent.h"

#include "UIButton.h"
#include "UIObject.h"
#include "UIText.h"

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>

LevelSelect::LevelSelect()
{
	myIsSelectingLevel = true;
	myLevelWasStarted = false;
	myLevelIndex = 0;
	myBackButton = nullptr;
	myInput = nullptr;
}

void LevelSelect::Load()
{
	myLevelButtons.clear();

	myInput = CGameWorld::GetInstance()->Input();

	myLevelWasStarted = false;
	myIsSelectingLevel = true;
	myLevelIndex = 0;

	InitiateButtons();
	InitCollectibles();
	InitTexts();

	GameObject* background = new GameObject(this);

	SpriteComponent* sprite = background->AddComponent<SpriteComponent>();
	sprite->SetSpritePath("Sprites/UI/levelSelect/UI_levelSelect_Background_304x164px.dds");
	background->SetZIndex(200);

	background->SetPosition(v2f(8.0f, 8.0f));
	background->SetPivot(v2f(0.0f, 0.0f));

	Scene::Load();
}

void LevelSelect::Update(const float& aDeltaTime)
{
	if (myLevelWasStarted)
	{
		CheckButtonPress();
	}
	
	myLevelWasStarted = true;

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
}

void LevelSelect::CheckButtonPress()
{
	if (myIsSelectingLevel)
	{
		if (static_cast<int>(myLevelButtons.size()) > 0)
		{
			if (myInput->GetInput()->GetKeyJustDown(Keys::LEFTARROWKey) || myInput->GetController()->IsButtonPressed(Controller::Button::DPadLeft))
			{
				myLevelButtons[myLevelIndex]->SetIsHighlightActive(false);
				myLevelIndex--;
				AudioManager::GetInstance()->PlayAudio(AudioList::MenuMove);
				if (myLevelIndex < 0)
				{
					myLevelIndex = myLevelButtons.size() - 1;
				}
				myLevelButtons[myLevelIndex]->SetIsHighlightActive(true);
			}
			else if (myInput->GetInput()->GetKeyJustDown(Keys::RIGHTARROWKey) || myInput->GetController()->IsButtonPressed(Controller::Button::DPadRight))
			{
				myLevelButtons[myLevelIndex]->SetIsHighlightActive(false);
				myLevelIndex++;
				AudioManager::GetInstance()->PlayAudio(AudioList::MenuMove);
				if (myLevelIndex > myLevelButtons.size() - 1)
				{
					myLevelIndex = 0;
				}
				myLevelButtons[myLevelIndex]->SetIsHighlightActive(true);
			}

			ShowArea(myAreaIndexes[myLevelIndex]);
			UpdateLevelCollectibles();
		}
	}
	
	if ((myInput->GetInput()->GetKeyJustDown(Keys::DOWNARROWKey) || myInput->GetController()->IsButtonPressed(Controller::Button::DPadDown)) ||
		myInput->GetInput()->GetKeyJustDown(Keys::UPARROWKey) || myInput->GetController()->IsButtonPressed(Controller::Button::DPadUp))
	{
		if (static_cast<int>(myLevelButtons.size()) > 0)
		{
			myIsSelectingLevel = !myIsSelectingLevel;
			AudioManager::GetInstance()->PlayAudio(AudioList::MenuMove);

			if (myIsSelectingLevel)
			{
				myBackButton->SetIsHighlightActive(false);
				myBackButtonFire->SetActive(false);

				if (static_cast<int>(myLevelButtons.size()) > 0)
				{
					myLevelButtons[myLevelIndex]->SetIsHighlightActive(true);
				}
			}
			else
			{
				if (static_cast<int>(myLevelButtons.size()) > 0)
				{
					myLevelButtons[myLevelIndex]->SetIsHighlightActive(false);
				}

				myBackButton->SetIsHighlightActive(true);
				myBackButtonFire->SetActive(true);
			}
		}
	}

	if (myInput->GetInput()->GetKeyJustDown(Keys::ENTERKey) || myInput->GetController()->IsButtonPressed(Controller::Button::Cross))
	{
		if (myIsSelectingLevel)
		{
			if (static_cast<int>(myLevelButtons.size()) <= 0)
			{
				return;
			}

			CGameWorld::GetInstance()->GetLevelManager().UsedLevelSelect();
			CGameWorld::GetInstance()->GetLevelManager().SetLevelIndex(myLevelIndexes[myLevelIndex]);
			CGameWorld::GetInstance()->GetLevelManager().SingleLoadScene(LevelManager::eScenes::LevelScene);
		}
		else
		{
			CGameWorld::GetInstance()->GetLevelManager().SingleLoadScene(LevelManager::eScenes::MainMenu);
		}
	}
}

void LevelSelect::InitiateButtons()
{
	myAreaTexts.clear();
	myLevelButtons.clear();

	myAreaIndexes.clear();
	myLevelIndexes.clear();

	myLevelCollectibles.clear();

	myAreaTexts.push_back(new UIObject(this));
	myAreaTexts.push_back(new UIObject(this));
	myAreaTexts.push_back(new UIObject(this));

	myAreaTexts[0]->Init("Sprites/UI/levelSelect/UI_levelSelect_Title_TheForest_111x16px.dds", v2f(111.0f, 16.0f), v2f(16.0f, 16.0f), 201);
	myAreaTexts[0]->SetPosition(v2f(16.0f, 16.0f));
	myAreaTexts[0]->SetActive(true);

	myAreaTexts[1]->Init("Sprites/UI/levelSelect/UI_levelSelect_Title_TheVillage_117x20px.dds", v2f(111.0f, 32.0f), v2f(16.0f, 16.0f), 201);
	myAreaTexts[1]->SetPosition(v2f(16.0f, 16.0f));
	myAreaTexts[1]->SetActive(false);

	myAreaTexts[2]->Init("Sprites/UI/levelSelect/UI_levelSelect_Title_TheCastle_106x16px.dds", v2f(111.0f, 16.0f), v2f(16.0f, 16.0f), 201);
	myAreaTexts[2]->SetPosition(v2f(16.0f, 16.0f));
	myAreaTexts[2]->SetActive(false);

	myBackButton = new UIButton(this);
	myBackButton->Init("Sprites/UI/levelSelect/UI_levelSelect_Text_MainMenu_Unmarked_64x16px.dds", v2f(64.0f, 16.0f), v2f(248.0f, 156.0f), "Sprites/UI/levelSelect/UI_levelSelect_Text_MainMenu_Marked_64x16px.dds", 64);
	myBackButton->SetPosition(v2f(248.0f, 156.0f));
	myBackButton->GetComponent<SpriteComponent>()->SetSize(v2f(64.0f, 16.0f));
	myBackButton->SetPivot(v2f(0.0f, 0.0f));
	myBackButton->SetActive(true);

	myBackButtonFire = new UIObject(this);
	myBackButtonFire->InitAnimation("Sprites/UI/levelSelect/UI_levelSelect_Flame_16x16px.dds", v2f(16.0f, 16.0f),8,8, v2f(230.0f, 156), 201);
	myBackButtonFire->SetPosition(v2f(230.0f, 152.0f));
	myBackButtonFire->SetPivot(v2f(0.0f, 0.0f));

	std::ifstream levelSelectFile("JSON/Menus/LevelSelect/LevelSelect.json");
	rapidjson::IStreamWrapper levelSelectStream(levelSelectFile);

	rapidjson::Document levelSelect;
	levelSelect.ParseStream(levelSelectStream);

	int bonfireIndex = 0;
	for (rapidjson::Value::ConstValueIterator iterator = levelSelect["Bonfires"].Begin(); iterator != levelSelect["Bonfires"].End(); ++iterator)
	{
		if (DataManager::GetInstance().GetBonfireState(bonfireIndex))
		{
			UIButton* levelButton = new UIButton(this);
			levelButton->SetPosition(v2f((*iterator)["X"].GetFloat(), (*iterator)["Y"].GetFloat()));
			levelButton->Init("Sprites/UI/levelSelect/UI_levelSelect_Stage_320x180px_Marked.dds", v2f(16.0f, 16.0f), v2f((*iterator)["X"].GetFloat(), (*iterator)["Y"].GetFloat()), "Sprites/UI/levelSelect/UI_levelSelect_Flame_16x16px.dds", 16);
			levelButton->GetComponent<SpriteComponent>()->SetSize(v2f(8.0f, 8.0f));
			levelButton->SetPivot(v2f(0.0f, 0.0f));
			levelButton->SetActive(true);
			levelButton->SetHighlightOffset(v2f(-7.0f, -8.0f));
			myLevelButtons.push_back(levelButton);

			myAreaIndexes.push_back((*iterator)["Area"].GetInt());
			myLevelIndexes.push_back((*iterator)["LevelIndex"].GetInt());
		}

		++bonfireIndex;
	}

	if (static_cast<int>(myLevelButtons.size()) > 0)
	{
		myLevelButtons[0]->SetIsHighlightActive(true);
	}
	else
	{
		myBackButton->SetIsHighlightActive(true);
		myBackButtonFire->SetActive(true);
		myIsSelectingLevel = false;
	}
}

void LevelSelect::ShowArea(const int& aIndex)
{
	myAreaTexts[0]->SetActive(false);
	myAreaTexts[1]->SetActive(false);
	myAreaTexts[2]->SetActive(false);

	myAreaTexts[aIndex]->SetActive(true);
}

void LevelSelect::InitCollectibles()
{
	GameObject* collectible1 = new GameObject(this);
	GameObject* collectible2 = new GameObject(this);
	GameObject* collectible3 = new GameObject(this);

	SpriteComponent* collectibleSprite1 = collectible1->AddComponent<SpriteComponent>();
	SpriteComponent* collectibleSprite2 = collectible2->AddComponent<SpriteComponent>();
	SpriteComponent* collectibleSprite3 = collectible3->AddComponent<SpriteComponent>();

	collectibleSprite1->SetSpritePath("Sprites/Objects/Collectible3.dds");
	collectibleSprite2->SetSpritePath("Sprites/Objects/Collectible2.dds");
	collectibleSprite3->SetSpritePath("Sprites/Objects/Collectible1.dds");

	collectibleSprite1->SetSize(v2f(16.0f, 16.0f));
	collectibleSprite2->SetSize(v2f(16.0f, 16.0f));
	collectibleSprite3->SetSize(v2f(16.0f, 16.0f));

	AnimationComponent* collectibleAnimationComponent1 = collectible1->AddComponent<AnimationComponent>();
	AnimationComponent* collectibleAnimationComponent2 = collectible2->AddComponent<AnimationComponent>();
	AnimationComponent* collectibleAnimationComponent3 = collectible3->AddComponent<AnimationComponent>();

	collectibleAnimationComponent1->SetSprite(collectibleSprite1);
	collectibleAnimationComponent2->SetSprite(collectibleSprite2);
	collectibleAnimationComponent3->SetSprite(collectibleSprite3);

	Animation collectibleAnimation1 = Animation(false, false, false, 0, 7, 7, 0.125f, collectibleSprite1, 16.0f, 16.0f);
	Animation collectibleAnimation2 = Animation(false, false, false, 0, 7, 7, 0.125f, collectibleSprite2, 16.0f, 16.0f);
	Animation collectibleAnimation3 = Animation(false, false, false, 0, 7, 7, 0.125f, collectibleSprite3, 16.0f, 16.0f);

	collectibleAnimationComponent1->SetAnimation(&collectibleAnimation1);
	collectibleAnimationComponent2->SetAnimation(&collectibleAnimation2);
	collectibleAnimationComponent3->SetAnimation(&collectibleAnimation3);

	collectible1->SetZIndex(201);
	collectible2->SetZIndex(201);
	collectible3->SetZIndex(201);

	collectible1->SetPosition(v2f(166.0f, 17.0f));
	collectible2->SetPosition(v2f(218.0f, 17.0f));
	collectible3->SetPosition(v2f(270.0f, 17.0f));

	collectible1->SetPivot(v2f(0.0f, 0.0f));
	collectible2->SetPivot(v2f(0.0f, 0.0f));
	collectible3->SetPivot(v2f(0.0f, 0.0f));
}

void LevelSelect::InitTexts()
{
	myTotalCollectibleInfo.clear();
	myTotalCollectibleInfoCollected.clear();

	myCollectibleInfo.clear();
	myCollectibleInfoCollected.clear();

	DataManager& dataManager = DataManager::GetInstance();
	int collectiblesInfoSize = dataManager.GetCollectableCount();


	for (int bonfire = 0; bonfire < 8; ++bonfire)
	{
		myCollectibleInfo.push_back(std::vector<int>());
		myCollectibleInfoCollected.push_back(std::vector<int>());

		for (int collectibleDifficulty = 0; collectibleDifficulty < 3; ++collectibleDifficulty)
		{
			myCollectibleInfo[bonfire].push_back(0);
			myCollectibleInfoCollected[bonfire].push_back(0);
		}
	}

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
		++myCollectibleInfo[collectibleInfo.myBonfireID][collectibleInfo.myDifficulty];

		if (collectibleInfo.myCollectedState)
		{
			++myTotalCollectibleInfoCollected[collectibleInfo.myDifficulty];
			++myCollectibleInfoCollected[collectibleInfo.myBonfireID][collectibleInfo.myDifficulty];
		}
	}

	UIText* totalText = new UIText(this);
	totalText->Init("Total", "Text/Peepo.ttf", EFontSize_48);
	totalText->SetPosition(v2f(16.0f, 60.0f));

	UIText* totalEasyText = new UIText(this);
	totalEasyText->Init(std::to_string(myTotalCollectibleInfoCollected[0]) + "/" + std::to_string(myTotalCollectibleInfo[0]), "Text/Peepo.ttf", EFontSize_48);
	totalEasyText->SetPosition(v2f(166.0f, 60.0f));

	UIText* totalMediumText = new UIText(this);
	totalMediumText->Init(std::to_string(myTotalCollectibleInfoCollected[1]) + "/" + std::to_string(myTotalCollectibleInfo[1]), "Text/Peepo.ttf", EFontSize_48);
	totalMediumText->SetPosition(v2f(218.0f, 60.0f));

	UIText* totalHardText = new UIText(this);
	totalHardText->Init(std::to_string(myTotalCollectibleInfoCollected[2]) + "/" + std::to_string(myTotalCollectibleInfo[2]), "Text/Peepo.ttf", EFontSize_48);
	totalHardText->SetPosition(v2f(270.0f, 60.0f));

	myLevelCollectibles.push_back(new UIText(this));
	myLevelCollectibles.push_back(new UIText(this));
	myLevelCollectibles.push_back(new UIText(this));
	myLevelCollectibles.push_back(new UIText(this));

	myLevelCollectibles[0]->Init("Area 1", "Text/Peepo.ttf", EFontSize_48);
	myLevelCollectibles[0]->SetPosition(v2f(16.0f, 46.0f));
	myLevelCollectibles[0]->GetComponent<TextComponent>()->SetColor(Tga2D::CColor(0.0f, 0.5f, 1.0f, 1.0f));

	myLevelCollectibles[1]->Init(std::to_string(myCollectibleInfoCollected[0][0]) + "/" + std::to_string(myCollectibleInfo[0][0]), "Text/Peepo.ttf", EFontSize_48);
	myLevelCollectibles[1]->SetPosition(v2f(166.0f, 46.0f));
	myLevelCollectibles[1]->GetComponent<TextComponent>()->SetColor(Tga2D::CColor(0.0f, 0.5f, 1.0f, 1.0f));

	myLevelCollectibles[2]->Init(std::to_string(myCollectibleInfoCollected[0][1]) + "/" + std::to_string(myCollectibleInfo[0][1]), "Text/Peepo.ttf", EFontSize_48);
	myLevelCollectibles[2]->SetPosition(v2f(218.0f, 46.0f));
	myLevelCollectibles[2]->GetComponent<TextComponent>()->SetColor(Tga2D::CColor(0.0f, 0.5f, 1.0f, 1.0f));

	myLevelCollectibles[3]->Init(std::to_string(myCollectibleInfoCollected[0][2]) + "/" + std::to_string(myCollectibleInfo[0][2]), "Text/Peepo.ttf", EFontSize_48);
	myLevelCollectibles[3]->SetPosition(v2f(270.0f, 46.0f));
	myLevelCollectibles[3]->GetComponent<TextComponent>()->SetColor(Tga2D::CColor(0.0f, 0.5f, 1.0f, 1.0f));
}

void LevelSelect::UpdateLevelCollectibles()
{
	myLevelCollectibles[0]->GetComponent<TextComponent>()->SetText("Area " + std::to_string(myLevelIndex + 1));

	myLevelCollectibles[1]->GetComponent<TextComponent>()->SetText(std::to_string(myCollectibleInfoCollected[myLevelIndex][0]) + "/" + std::to_string(myCollectibleInfo[myLevelIndex][0]));
	myLevelCollectibles[2]->GetComponent<TextComponent>()->SetText(std::to_string(myCollectibleInfoCollected[myLevelIndex][1]) + "/" + std::to_string(myCollectibleInfo[myLevelIndex][1]));
	myLevelCollectibles[3]->GetComponent<TextComponent>()->SetText(std::to_string(myCollectibleInfoCollected[myLevelIndex][2]) + "/" + std::to_string(myCollectibleInfo[myLevelIndex][2]));
}