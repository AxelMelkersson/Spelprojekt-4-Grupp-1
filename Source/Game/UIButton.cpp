#include "stdafx.h"
#include "UIButton.h"
#include "SpriteComponent.h"
#include "Player.hpp"
#include "LevelScene.h"
#include "Camera.h"
#include <iostream>
#include "AnimationComponent.hpp"


UIButton::UIButton(Scene* aLevelScene)
	:
	GameObject(aLevelScene),
	myCamera(aLevelScene->GetCamera())
{
	myBtnHighlighted = {};
	myIsActive = {};
	myIsUnlocked = false;
}

UIButton::~UIButton()
{
}

void UIButton::Init(const std::string aPathString, const v2f aSize, const v2f aPosition, const std::string aAnimationPathString, const int aBoundX, const std::string aLockedPathString, bool aIsUnlocked)
{
	myIsUnlocked = aIsUnlocked;
	SetZIndex(202);
	myPosition = aPosition;

	SpriteComponent* sprite = AddComponent<SpriteComponent>();
	if (myIsUnlocked == false)
	{
		sprite->SetSpritePath(aLockedPathString);
		myPosition.x += 1;
	}
	else
	{
		sprite->SetSpritePath(aPathString);
	}

	mySprite = AddComponent<SpriteComponent>();
	mySprite->SetSpritePath(aAnimationPathString);

	AnimationComponent* animation = AddComponent<AnimationComponent>();
	animation->SetSprite(mySprite);
	Animation idleAnimation = Animation(false, false, false, 0, 8, 8, 0.15f, mySprite, aBoundX, 16);
	animation->SetAnimation(&idleAnimation);
	mySprite->SetSize(aSize);

	SetPivot({0.f, 0.f});

	GameObject::Init();
}

void UIButton::Update(const float& aDeltaTime)
{
	UpdateButton(aDeltaTime);
	GameObject::Update(aDeltaTime);
}

void UIButton::UpdateButton(const float& aDeltaTime)
{
	SetPosition(myCamera.GetPosition() + myPosition);

	if (myBtnHighlighted && myIsUnlocked)
	{
		mySprite->Activate();
		GameObject::Update(aDeltaTime);
	}
	else
		mySprite->Deactivate();
}

void UIButton::Render()
{
	if (myIsActive)
		GameObject::Render();
}

void UIButton::SetIsHighlightActive(const bool aHighlightBool)
{
	myBtnHighlighted = aHighlightBool;
}


void UIButton::SetActive(const bool aActiveState)
{
	myIsActive = aActiveState;
}

void UIButton::SetHighlightOffset(const v2f& aOffset)
{
	mySprite->SetRelativePosition(aOffset);
}

bool UIButton::GetIsUnlocked() const
{
	return myIsUnlocked;
}
