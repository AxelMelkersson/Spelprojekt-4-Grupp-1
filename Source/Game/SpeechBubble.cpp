#include "stdafx.h"
#include "SpeechBubble.h"
#include "SpriteComponent.h"
#include "TextComponent.h"

#include "DataManager.h"

SpeechBubble::SpeechBubble(Scene* aScene)
	:
	GameObject(aScene)
{
}

void SpeechBubble::Init(const int aBonfireID, const v2f aPos)
{
	SetPosition({ aPos.x - 10, aPos.y - 50 });
	SetZIndex(200);

	ParseText(aBonfireID);

	SpriteComponent* spriteComp = AddComponent<SpriteComponent>();
	spriteComp->SetSpritePath("Sprites/UI/popUp/UI_PopUp_84x32px.dds");
	spriteComp->SetSize({ 160, 64 });
	spriteComp->SetRelativePosition({ GetPosition().x - 110, GetPosition().y - 130 });

	TextComponent* textComp = AddComponent<TextComponent>();
	textComp->CreateText("Text/alagard.ttf", EFontSize::EFontSize_36, 0);
	textComp->SetRelativePosition(GetPosition().x - 30, GetPosition().y - 50);
	textComp->SetText(myText[0]);
}

void SpeechBubble::Speak()
{

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

std::vector<std::string> SpeechBubble::SplitText(std::string someText)
{
	return { someText };
}
