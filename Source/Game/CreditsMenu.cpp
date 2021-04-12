#include "stdafx.h"
#include "CreditsMenu.h"

#include "../External/Headers/CU/Utilities.h"
#include "InputWrapper.h"

#include "GameWorld.h"

#include "SpriteComponent.h"
#include "AnimationComponent.hpp"

CreditsMenu::CreditsMenu(Scene* aScene) : GameObject(aScene)
{
	SpriteComponent* background = AddComponent<SpriteComponent>();
	SpriteComponent* credits = AddComponent<SpriteComponent>();
	SpriteComponent* creditsFlame = AddComponent<SpriteComponent>();
	SpriteComponent* creditsBack = AddComponent<SpriteComponent>();

	background->SetSpritePath("Sprites/UI/credits/UI_Credits_304x164px.dds");
	credits->SetSpritePath("Sprites/UI/credits/UI_Credits_Text_241x151px.dds"); 
	creditsFlame->SetSpritePath("Sprites/UI/credits/UI_Credits_Flame_16x16px.dds");
	creditsBack->SetSpritePath("Sprites/UI/credits/UI_Credits_Text_Back_29x16px_Marked.dds");

	SetPivot(v2f(0.0f, 0.0f));

	creditsFlame->SetSize(v2f(16.0f, 16.0f));
	creditsBack->SetSize(v2f(29.0f, 16.0f));

	Animation flameAnimation = Animation(false, false, false, 0, 7, 7, 0.125f, creditsFlame, 16, 16);
	Animation creditsAnimation = Animation(false, false, false, 0, 8, 8, 0.125f, creditsBack, 29, 16);

	AnimationComponent* animation = AddComponent<AnimationComponent>();
	AnimationComponent* creditsAnim = AddComponent<AnimationComponent>();
	animation->SetSprite(creditsFlame);
	animation->SetAnimation(&flameAnimation);
	creditsAnim->SetSprite(creditsBack);
	creditsAnim->SetAnimation(&creditsAnimation);

	background->SetRelativePosition(v2f(0.0f, 0.0f));
	credits->SetRelativePosition(v2f(30.0f, 6.0f));
	creditsFlame->SetRelativePosition(v2f(242.0f, 142.0f));
	creditsBack->SetRelativePosition(v2f(260.0f, 147.0f));
}