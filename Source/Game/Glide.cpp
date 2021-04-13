#include "stdafx.h"
#include "Glide.hpp"

#include "Scene.h"

#include "SpriteComponent.h"
#include "AnimationComponent.hpp"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"
#include "AudioManager.h"

#include "Player.hpp"

Glide::Glide(Scene* aScene)
	:
	GameObject(aScene)
{
	SetZIndex(97);

	SpriteComponent* sprite = AddComponent<SpriteComponent>();
	sprite->SetSpritePath("Sprites/Objects/GlidePowerup2.dds");
	sprite->SetSize(v2f(16.0f, 16.0f));

	AnimationComponent* animation = AddComponent<AnimationComponent>();
	animation->SetSprite(sprite);

	myAnimations[0] = Animation(false, false, false, 0, 9, 9, 0.125f, sprite, 16, 16);

	animation->SetAnimation(&myAnimations[0]);

	PhysicsComponent* physics = AddComponent<PhysicsComponent>();
	physics->SetCanCollide(false);
	physics->SetIsStatic(false);
	physics->SetApplyGravity(false);

	ColliderComponent* collider = AddComponent<ColliderComponent>();
	collider->SetSize(16.0f, 16.0f);

	myDisappearTime = 1.0f;
	myTimer = myDisappearTime;

	myIsTaken = false;
}

Glide::~Glide()
{

}

void Glide::Init(const v2f& aPosition)
{
	SetPosition(aPosition);

	GameObject::Init();
}

void Glide::Update(const float& aDeltaTime)
{
	myTimer -= aDeltaTime;
	if (myTimer <= 0 && myIsTaken)
	{
		Activate();
		myIsTaken = false;
	}

	GameObject::Update(aDeltaTime);
}

void Glide::OnCollision(GameObject* aGameObject)
{
	Player* player = dynamic_cast<Player*>(aGameObject);
	if (player)
	{
		if (!myIsTaken)
		{
			Deactivate();

			player->StartGliding();

			myTimer = myDisappearTime;
			myIsTaken = true;
			AudioManager::GetInstance()->PlayAudio(AudioList::HoverActivate);
		}
	}
}