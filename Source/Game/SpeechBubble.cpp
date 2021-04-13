#include "stdafx.h"
#include "SpeechBubble.h"
#include "SpriteComponent.h"
#include "TextComponent.h"

#include "DataManager.h"
#include "GameWorld.h"

SpeechBubble::SpeechBubble(Scene* aScene)
	:
	GameObject(aScene),
	myIsSpeaking(false),
	myHasFadedIn(false),
	myHasFadedOut(false),
	myPlayerIsStandingByTheBonfire(false),
	myRowNumber(5),
	myAlpha(0.0f),
	myFadeSpeed(0.5f),
	mySpeakTime(5.f),
	myTimeIHasSpoken(0.f)
{}

void SpeechBubble::Init(const int aBonfireID, const v2f aPos)
{
	SetPosition({ aPos.x - 10, aPos.y - 50 });
	SetZIndex(200);

	ParseText(aBonfireID);

	SpriteComponent* spriteComp = AddComponent<SpriteComponent>();
	spriteComp->SetSpritePath("Sprites/UI/popUp/UI_PopUp_84x32px.dds");
	spriteComp->SetSize({ 160, 64 });
	spriteComp->SetRelativePosition({ GetPosition().x - 110, GetPosition().y - 130 });
	spriteComp->SetColor({1, 1, 1, myAlpha });

	for (int i = 0; i < myRowNumber; ++i)
	{
		TextComponent* textComp = AddComponent<TextComponent>();
		textComp->CreateText("Text/Peepo.ttf", EFontSize::EFontSize_36, 0);
		textComp->SetRelativePosition(GetPosition().x - 30, GetPosition().y - 50 + i * 10);
		textComp->SetColor({ 1, 1, 1, myAlpha });

		if (i < myText.size())
		{
			textComp->SetText(myText[i]);
		}
		else
		{
			textComp->SetText("");
		}
		myTextComps.push_back(textComp);
	}
}

void SpeechBubble::Update(const float& aDeltaTime)
{	
	if (myIsSpeaking)
	{
		if (!myHasFadedIn)
		{
			if (myAlpha >= 1.f)
			{
				myAlpha = 1.f;
				myHasFadedIn = true;
				myHasFadedOut = false;
			}
			else
			{
				Fade(true, aDeltaTime);
			}
		}
		else if(!myHasFadedOut)
		{
			myTimeIHasSpoken += aDeltaTime;
			if (myTimeIHasSpoken >= mySpeakTime && !myPlayerIsStandingByTheBonfire)
			{
				if (myAlpha <= 0.f)
				{
					myAlpha = 0.f;
					myHasFadedOut = true;
					myHasFadedIn = false;
					myIsSpeaking = false;
					myTimeIHasSpoken = 0.f;
				}
				else
				{
					Fade(false, aDeltaTime);
				}
			}
		}
	}

	myPlayerIsStandingByTheBonfire = false;
	GameObject::Update(aDeltaTime);
}

void SpeechBubble::Speak()
{
	myIsSpeaking = true;
	myPlayerIsStandingByTheBonfire = true;
}

bool SpeechBubble::GetIsSpeaking()
{
	return myIsSpeaking;
}

void SpeechBubble::ParseText(const int aIndex)
{
	rapidjson::Document& levelDoc = rapidjson::Document();
	DataManager::GetInstance().ReadFileIntoDocument("JSON/Story.json", levelDoc);

	if (levelDoc.IsObject())
	{
		rapidjson::Value::ConstValueIterator bonfireText = levelDoc["BonfireStory"].Begin() + aIndex;

		if ((*bonfireText).IsObject())
		{
			for (rapidjson::Value::ConstValueIterator text = (*bonfireText)["text"].Begin(); text != (*bonfireText)["text"].End(); ++text)
			{
				myText.push_back((*text).GetString());
			}
		}
	}
}

void SpeechBubble::Fade(const bool aIsFadingIn, const float& aDeltaTime)
{
	GetComponent<SpriteComponent>()->SetColor({ 1, 1, 1, myAlpha });
	for (int i = 0; i < myRowNumber; ++i)
	{
		myTextComps[i]->SetColor({ 1, 1, 1, myAlpha });
	}
	if (aIsFadingIn)
	{
		myAlpha += aDeltaTime * myFadeSpeed;
	}
	else
	{
		myAlpha -= aDeltaTime * myFadeSpeed;
	}
}
