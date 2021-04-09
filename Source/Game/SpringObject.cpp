#include "stdafx.h"
#include "LevelScene.h"
#include "SpringObject.h"
#include "SpriteComponent.h"
#include "AnimationComponent.hpp"
#include "ColliderComponent.h"
#include "PhysicsComponent.h"
#include "Player.hpp"
#include "Game.h"
#include "rapidjson/istreamwrapper.h"
#include "AudioManager.h"
#include <iostream>

SpringObject::SpringObject(Scene* aLevelScene) : GameObject(aLevelScene)
{
	mySpringActive = {};
	myRetardation = {};
	myVelocityForce = {};
	myTimer = {};

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
			GetComponent<AnimationComponent>()->SetAnimation(&myAnimation);
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
	myAnimation = Animation(false, false, false, 0, 4, 4, 0.06f, sprite, 16, 16);
	animation->SetAnimation(&myAnimation);

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

//#ifdef _DEBUG
//void SpringObject::ImGuiUpdate()
//{
//	ImGui::Begin("Spring", &myIsActive, ImGuiWindowFlags_AlwaysAutoResize);
//
//	ImGui::SliderFloat("Spring Velocity Force", &myVelocityForce, 0.0f, 2000.0f);
//	ImGui::SliderFloat("Spring Velocity Retardation", &myRetardation, 0.0f, 5.0f);
//
//	ImGui::End();
//}
//#endif // _DEBUG
