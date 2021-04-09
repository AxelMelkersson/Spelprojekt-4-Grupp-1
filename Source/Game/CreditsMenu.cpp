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

	background->SetSpritePath("Sprites/UI/tutorialMenu/UI_tutorialMenu_Background.dds");
	credits->SetSpritePath("Sprites/UI/creditsMenu/Credits.png");
	creditsFlame->SetSpritePath("Sprites/UI/tutorialMenu/UI_tutorialMenu_Flame_16x16px.dds");

	SetPivot(v2f(0.0f, 0.0f));

	creditsFlame->SetSize(v2f(16.0f, 16.0f));

	Animation flameAnimation = Animation(false, false, false, 0, 7, 7, 0.125f, creditsFlame, 16, 16);

	AnimationComponent* animation = AddComponent<AnimationComponent>();
	animation->SetSprite(creditsFlame);
	animation->SetAnimation(&flameAnimation);

	background->SetRelativePosition(v2f(0.0f, 0.0f));
	credits->SetRelativePosition(v2f(24.0f, 24.0f));
	creditsFlame->SetRelativePosition(v2f(202.0f, 142.0f));
}