#include "stdafx.h"
#include "UIText.h"
#include "Game.h"
#include "TextComponent.h"

UIText::UIText(Scene* aLevelScene)
	:
	GameObject(aLevelScene)
{
}

void UIText::Init(const std::string aTextString, const char* aPathAndName, EFontSize aFontSize, unsigned char aBorderSize)
{
	v2f referenceSize = Config::ourReferenceSize;

	myText = AddComponent<TextComponent>();
	myText->CreateText(aPathAndName, static_cast<EFontSize>(aFontSize), aBorderSize);
	myText->SetText(aTextString);
	myText->SetRelativePosition(referenceSize.x * 0.5f, referenceSize.y * 0.5f);
	myText->Activate();
}

void UIText::SetPosition(const v2f aPosition)
{
	myText->SetRelativePosition(aPosition.x, aPosition.y);
}

