#include "stdafx.h"
#include "UIObject.h"
#include "LevelScene.h"
#include "SpriteComponent.h"
#include "AnimationComponent.hpp"


UIObject::UIObject(Scene* aLevelScene)
	:
	GameObject(aLevelScene)
{
	myCamera = &aLevelScene->GetCamera();
	//myPosition = {};
	myIsActive = {};
}

void UIObject::Init(const std::string aPathString, const v2f aSize, const v2f aPosition, int aZIndex)
{
	SetZIndex(aZIndex);
	SetPivot({0.f, 0.f});
	myStartPosition = aPosition;
	
	SpriteComponent* sprite = AddComponent<SpriteComponent>();
	sprite->SetSpritePath(aPathString);
	sprite->SetSize(aSize);

	GameObject::Init();
}

void UIObject::InitAnimation(const std::string aPathString, const v2f aSize, int aAnimationFrames, int aColumns, const v2f aPosition, int aZIndex)
{
	SetZIndex(aZIndex);
	SetPivot({ 0.5f, 0.5f });
	myStartPosition = aPosition;

	SpriteComponent* sprite = AddComponent<SpriteComponent>();
	sprite->SetSpritePath(aPathString);
	sprite->SetSize(aSize);
	AnimationComponent* animation = AddComponent<AnimationComponent>();
	animation->SetSprite(sprite);
	Animation idleAnimation = Animation(false, false, false, 0, aAnimationFrames, aColumns, 0.10f, sprite, 16, 16);
	animation->SetAnimation(&idleAnimation);

	GameObject::Init();
}

void UIObject::SetPosition(const v2f& aPosition)
{
	myStartPosition = aPosition;
	myTransform.myPosition = aPosition;
}

void UIObject::SetPositionX(const float& aPositionX)
{
	myStartPosition.x = aPositionX;
	myTransform.myPosition.x = aPositionX;
}

void UIObject::SetPositionY(const float& aPositionY)
{
	myStartPosition.y = aPositionY;
	myTransform.myPosition.y = aPositionY;
}

void UIObject::UpdateUIObjects(const float& aDeltaTime)
{
	
	GameObject::SetPosition(myCamera->GetPosition() + myStartPosition);

	GameObject::Update(aDeltaTime);
}

void UIObject::Render()
{
	if (myIsActive)
		GameObject::Render();
}

void UIObject::SetActive(const bool aActiveState)
{
	myIsActive = aActiveState;
}

const v2f UIObject::GetStartPosition()
{
	return myStartPosition;
}