#include "stdafx.h"
#include "SpeechBubble.h"
#include "SpriteComponent.h"
#include "TextComponent.h"

void SpeechBubble::Init(std::vector<std::string> someMessages)
{
	for (int i = 0; someMessages.size(); ++i)
	{
		myMessages.push_back(someMessages[i]);
	}

	SpriteComponent* spriteComp = new SpriteComponent;
	spriteComp->SetSpritePath("Sprites/UI/popUp/UI_PopUp_84x32px.dds");
	spriteComp->SetRelativePosition(GetPosition());
}

//void SpeechBubble::Update(const float& aDeltaTime)
//{
//}
//
//void SpeechBubble::Render()
//{
//}
