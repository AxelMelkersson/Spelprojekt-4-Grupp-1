#include "stdafx.h"
#include "SpringObject.h"

#include "LevelScene.h"
#include "AudioManager.h"

#include "SpriteComponent.h"
#include "AnimationComponent.hpp"
#include "ColliderComponent.h"
#include "PhysicsComponent.h"

#include "Player.hpp"
#include "Game.h"
#include "rapidjson/istreamwrapper.h"

SpringObject::SpringObject(Scene* aLevelScene) : GameObject(aLevelScene)
{
	mySpringActive = false;;
	myRetardation = 0.0f;
	myVelocityForce = 0.0f;
	myTimer = 0.0f;
	mySpringTimerCooldown = 0.0f;

	SetZIndex(94);
}

void SpringObject::Init(const v2f aPosition)
{
	InitSprings(aPosition);
	LoadJson();
	GameObject::Init();
}
void SpringObject::Update(const float& aDeltaTime)
{
	myTimer += aDeltaTime;

	if (mySpringActive)
	{
		GameObject::Update(aDeltaTime);

		if (GetComponent<AnimationComponent>()->GetHasBeenDisplayedOnce())
		{
			mySpringActive = false;
		}
	}
}

void SpringObject::OnCollision(GameObject* aGameObject)
{
	Player* player = dynamic_cast<Player*>(aGameObject);

	if (player != NULL)
	{
		v2f velo = player->GetComponent<PhysicsComponent>()->GetVelocity();

		if (myTimer > mySpringTimerCooldown)
		{
			if (velo.y < 0.f)
				player->ActivateSpringForce(-myVelocityForce, myRetardation, false);
			else
				player->ActivateSpringForce(-myVelocityForce, myRetardation, true);

			AudioManager::GetInstance()->PlayAudio(AudioList::PlayerJumpPad);
			mySpringActive = true;
			myTimer = {};
			GetComponent<AnimationComponent>()->SetAnimation(&myAnimation[1]);
			GetComponent<AnimationComponent>()->SetNextAnimation(&myAnimation[2]);
		}
	}
}

void SpringObject::InitSprings(const v2f aPosition)
{
	mySpringTimerCooldown = 0.1f;
	myPosition = aPosition;
	mySize = { 16.0f, 16.0f };

	SetPosition(myPosition);
	SetPivot(v2f(0.5f, 1.0f));

	CreateGroundSpring();

	PhysicsComponent* physics = AddComponent<PhysicsComponent>();
	physics->SetCanCollide(false);
	physics->SetIsStatic(true);

	ColliderComponent* collider = AddComponent<ColliderComponent>();
	collider->SetSize({ mySize.x, mySize.y * 0.01f });
	collider->SetPosition({ 0.f, -mySize.y * 0.2f });
}
void SpringObject::CreateGroundSpring()
{
	SpriteComponent* sprite = AddComponent<SpriteComponent>();
	sprite->SetSpritePath("Sprites/Objects/Mushroom.dds");
	sprite->SetSize(mySize);

	AnimationComponent* animation = AddComponent<AnimationComponent>();
	animation->SetSprite(sprite);
	myAnimation[0] = Animation(false, true, false, 0, 1, 1, 0.055f, sprite, 16, 16);
	myAnimation[1] = Animation(false, true, false, 0, 4, 4, 0.055f, sprite, 16, 16);
	myAnimation[2] = Animation(false, true, false, 4, 14, 14, 0.1f, sprite, 16, 16);
	animation->SetAnimation(&myAnimation[0]);
	sprite->SetSize(mySize);
}
void SpringObject::LoadJson()
{
	std::ifstream springObjectFile("JSON/SpringObject.json");
	rapidjson::IStreamWrapper springsObjectStream(springObjectFile);

	rapidjson::Document springDocuments;
	springDocuments.ParseStream(springsObjectStream);

	myRetardation = springDocuments["Retardation"].GetFloat();
	myVelocityForce = springDocuments["Velocity"].GetFloat();

	springObjectFile.close();
}