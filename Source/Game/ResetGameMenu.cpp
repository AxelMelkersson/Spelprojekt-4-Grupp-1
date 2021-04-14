#include "stdafx.h"
#include "../External/Headers/CU/Utilities.h"
#include "InputWrapper.h"
#include "ResetGameMenu.h"
#include "GameWorld.h"
#include "SpriteComponent.h"
#include "AnimationComponent.hpp"
#include "TextComponent.h"

ResetGameMenu::ResetGameMenu(Scene* aScene)
	: GameObject(aScene)
{
	SpriteComponent* background = AddComponent<SpriteComponent>();
	TextComponent* text = AddComponent<TextComponent>();

	background->SetSpritePath("Sprites/UI/dialogueScreen/UI_dialogueScreen.dds");
	background->SetRelativePosition(v2f(0.0f, 0.0f));
	
	SetPivot(v2f(0.0f, 0.0f));

	text->CreateText("Text/alagard.ttf", EFontSize_36, 0);
	text->SetText("Do you really want \nto reset your progress?");
	text->SetColor({ 1,1,1,1 });
	text->SetRelativePosition(GetPosition().x + 110.f, GetPosition().y + 70.f);
	v2f yesPos = { 50.f, 50.f };


}


