#include "stdafx.h"
#include "Button.h"

#include "Player.hpp"
#include "LevelScene.h"
#include "Camera.h"

#include "SpriteComponent.h"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"
#include "AnimationComponent.hpp"

#include "AudioManager.h"

Button::Button(Scene* aLevelScene) : GameObject(aLevelScene)
{
	myHasCollided = false;
	myButtonActive = false;
	mySize = { 0, 0 };

	SetZIndex(95);
}

void Button::Init(const v2f myStartingPosition, const v2f myPositionFromStart)
{
	InitButton(myStartingPosition, myPositionFromStart);
}
void Button::Update(const float& aDeltaTime)
{
	GameObject::Update(aDeltaTime);
}

void Button::InitButton(const v2f myStartingPosition, const v2f myPositionFromStart)
{
	v2f platformPosition = myPositionFromStart;
	mySize = { 16.f, 8.f };

	SetPosition(platformPosition);
	SetPivot({ 0.f, 1.f });

	SpriteComponent* sprite = AddComponent<SpriteComponent>();
	sprite->SetSpritePath("Sprites/Objects/Button.dds");
	sprite->SetSize(mySize);

	myAnimation[0] = Animation(false, true, false, 0, 1, 1, 0.10f, sprite, 16, 8);
	myAnimation[1] = Animation(false, true, false, 0, 2, 2, 0.10f, sprite, 16, 8);

	AnimationComponent* animation = AddComponent<AnimationComponent>();
	animation->SetSprite(sprite);
	animation->SetAnimation(&myAnimation[0]);

	PhysicsComponent* gphys = AddComponent<PhysicsComponent>();
	gphys->SetCanCollide(false);
	gphys->SetIsStatic(true);

	ColliderComponent* collider = AddComponent<ColliderComponent>();
	collider->SetSize({mySize.x, mySize.y * 0.05f});
	collider->SetPosition({ mySize.x / 2.f, -mySize.y * 0.1f });

	GameObject::Init();
}

void Button::OnCollision(GameObject* aGameObject)
{
	Player* player = dynamic_cast<Player*>(aGameObject);

	if (player != NULL)
	{
		v2f velo = player->GetComponent<PhysicsComponent>()->GetVelocity();

		if (myHasCollided == false)
		{
			GetComponent<AnimationComponent>()->SetAnimation(&myAnimation[1]);
			myButtonActive = true;
			myHasCollided = true;
			AudioManager::GetInstance()->PlayAudio(AudioList::ButtonPress);
		}
	}
}
bool Button::GetActiveButton()
{
	return myButtonActive;
}