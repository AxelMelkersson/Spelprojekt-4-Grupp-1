#include "stdafx.h"
#include "Platform.h"

#include "../External/Headers/CU/Utilities.h"

#include "PhysicsComponent.h"
#include "ColliderComponent.h"
#include "AudioManager.h"
#include "LevelScene.h"

#include "Player.hpp"

#include "PostMaster.hpp"

Platform::Platform(Scene* aScene) : GameObject(aScene)
{

}

void Platform::Init(const v2f& aSize, const v2f& aSpriteSize, const v2f& aPosition, const bool& aIsOneway)
{
	SetPosition(aPosition);
	SetPivot({ 0.0f, 0.0f });

	PhysicsComponent* gphys = AddComponent<PhysicsComponent>();
	gphys->SetCanCollide(true);
	gphys->SetIsStatic(true);

	if (aIsOneway)
	{
		gphys->SetCollisionType(PhysicsComponent::eCollisionType::OneWay);
	}

	ColliderComponent* collider = AddComponent<ColliderComponent>();
	collider->SetCollideOnlyWithPlayer(true);
	collider->SetPosition({ aSpriteSize.x / 2.0f, aSpriteSize.y / 2.0f });
	collider->SetSize(aSize);

	GameObject::Init();
}

void Platform::Init(const v2f& aSize, const v2f& aSpriteSize, const v2f& aPosition, const bool& aIsOneway, const int& aMaterialIndex)
{
	myMaterial = aMaterialIndex;
	Platform::Init(aSize, aSpriteSize, aPosition, aIsOneway);
}

void Platform::Update(const float& aDeltaTime)
{
	GameObject::Update(aDeltaTime);
}

void Platform::OnCollision(GameObject* aGameObject)
{
	Player* player = dynamic_cast<Player*>(aGameObject);
	if (player)
	{
		player->PlayFootSteps(myMaterial);
		player->SetPlatformVelocity(v2f(0.0f, 0.0f));
		if (player->GetHasLanded() && (player->GetPositionY() <= myTransform.myPosition.y))
		{
			PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::PlayerSafeLanded, 0));
		}
	}
}

void Platform::Landed(const int& aOverlapY)
{
	dynamic_cast<Player*>(dynamic_cast<LevelScene*>(this->myScene)->GetPlayer())->PlayLandingSounds(myMaterial);
}

void Platform::ResetVelocity()
{
	GetComponent<PhysicsComponent>()->SetVelocity({ 0.f, 0.f });
}