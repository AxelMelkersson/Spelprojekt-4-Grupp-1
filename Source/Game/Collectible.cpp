#include "stdafx.h"
#include "Collectible.hpp"
#include "Player.hpp"

#include "SpriteComponent.h"
#include "AnimationComponent.hpp"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"
#include "AudioManager.h"
#include "PostMaster.hpp"

#include "../External/Headers/CU/Utilities.h"

#include "GameWorld.h"

#include "Random.hpp"

#include "Bonfire.hpp"

Collectible::Collectible(Scene* aLevelScene, const unsigned int anID, const unsigned int aBonfireID)
	:
	GameObject(aLevelScene),
	myTarget(nullptr),
	mySpeed(1.0f),
	myIdleMovementSpeed(5.0f),
	myIdleMovementDistance(8.0f),
	myMinRadiusFromTarget(25.0f),
	myTimeOffset(0.0f),
	myType(eCollectibleType::Easy),
	myWasCollected(false),
	myWasTurnedIn(false),
	myID(anID),
	myBonfireID(aBonfireID)
{
	Subscribe(eMessageType::PlayerSafeLanded);
	Subscribe(eMessageType::PlayerDeath);
}
Collectible::~Collectible()
{

}

void Collectible::Init(const v2f& aPosition, eCollectibleType aType)
{
	SetZIndex(129);

	SetPosition(aPosition);
	mySpawnPosition = aPosition;
	myTargetPosition = aPosition;

	myWasCollected = DataManager::GetInstance().GetCollectableInfo(myID).myCollectedState;
	myWasTurnedIn = DataManager::GetInstance().GetCollectableInfo(myID).myCollectedState;

	myTimeOffset = Utils::RandomFloat(0.0f, 6.0f);
	myType = aType;

	std::string spritePath;
	std::string spritePickupPath;

	switch (aType)
	{
		case eCollectibleType::Easy:
			spritePath = "Sprites/Objects/Collectible3.dds";
			spritePickupPath = "Sprites/Objects/Collectible3Pickup.dds";
			break;
		case eCollectibleType::Medium:
			spritePath = "Sprites/Objects/Collectible2.dds";
			spritePickupPath = "Sprites/Objects/Collectible2Pickup.dds";
			break;
		case eCollectibleType::Hard:
			spritePath = "Sprites/Objects/Collectible1.dds";
			spritePickupPath = "Sprites/Objects/Collectible1Pickup.dds";
			break;
	}

	SpriteComponent* spriteIdle = AddComponent<SpriteComponent>();
	spriteIdle->SetSpritePath(spritePath);
	spriteIdle->SetSize(v2f(16.0f, 16.0f));

	SpriteComponent* spritePickup = AddComponent<SpriteComponent>();
	spritePickup->SetSpritePath(spritePickupPath);
	spritePickup->SetSize(v2f(16.0f, 16.0f));
	spritePickup->Deactivate();

	myAnimations[0] = Animation(false, false, false, 0, 7, 7, 0.14f, spriteIdle, 16, 16);
	myAnimations[1] = Animation(false, true, false, 0, 8, 8, 0.09f, spritePickup, 16, 16);

	AnimationComponent* animation = AddComponent<AnimationComponent>();
	animation->SetSprite(spriteIdle);
	animation->SetAnimation(&myAnimations[0]);
	spriteIdle->SetSize(v2f(16.0f, 16.0));

	PhysicsComponent* physics = AddComponent<PhysicsComponent>();
	physics->SetCanCollide(false);
	physics->SetIsStatic(false);
	physics->SetApplyGravity(false);

	ColliderComponent* collider = AddComponent<ColliderComponent>();
	collider->SetSize(v2f(24.0f, 24.0f));

	GameObject::Init();
}

void Collectible::Update(const float& aDeltaTime)
{
	constexpr float tau = 6.283185307f;

	myTimeOffset += aDeltaTime;
	if (myTimeOffset > tau)
	{
		myTimeOffset -= tau;
	}

	if (myTarget)
	{
		if (myMinRadiusFromTarget * myMinRadiusFromTarget < (myTarget->GetPosition() - myTransform.myPosition).LengthSqr())
		{
			myTargetPosition = myTarget->GetPosition() + v2f(0.0f, -myMinRadiusFromTarget);
		}
	}

	const float offset = sin(myTimeOffset * myIdleMovementSpeed) * myIdleMovementDistance;
	myTransform.myPosition.x = Utils::Lerp(myTransform.myPosition.x, myTargetPosition.x, mySpeed * aDeltaTime);
	myTransform.myPosition.y = Utils::Lerp(myTransform.myPosition.y, myTargetPosition.y + offset, mySpeed * aDeltaTime);

	if (myWasTurnedIn)
	{
		TurnIn();
	}

#ifdef _DEBUG
	ImGuiUpdate();
#endif // DEBUG

	GameObject::Update(aDeltaTime);
}

void Collectible::OnCollision(GameObject* aGameObject)
{
	if (!myWasCollected && !myWasTurnedIn)
	{
		Player* player = dynamic_cast<Player*>(aGameObject);
		if (player)
		{
			//SetAnimation;
			myWasCollected = true;
			DataManager::GetInstance().SaveCollectedCollectible(myID);
			myTarget = aGameObject;
			AudioManager::GetInstance()->PlayAudio(AudioList::CollectableV1);
			PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::CollectibleCollectedParticle, GetPosition()));
		}
	}
}

void Collectible::Reset()
{
	myTarget = nullptr;
	myWasCollected = false;
	SetPosition(mySpawnPosition);
	myTargetPosition = mySpawnPosition;
}

void Collectible::TurnIn()
{
	if (!myWasTurnedIn)
	{
		GetComponent<AnimationComponent>()->SetAnimation(&myAnimations[1]);
		myWasTurnedIn = true;
	}
	else if (GetComponent<AnimationComponent>()->GetIsDisplayedOnce() && GetComponent<AnimationComponent>()->GetHasBeenDisplayedOnce())
	{
		Destroy();
	}
}

void Collectible::Notify(const Message& aMessage)
{
	if (aMessage.myMessageType == eMessageType::PlayerSafeLanded)
	{
		if (myWasCollected)
		{
			TurnIn();
		}
	}
	else if (aMessage.myMessageType == eMessageType::PlayerDeath)
	{
		Reset();
	}
}

void Collectible::ImGuiUpdate()
{
	ImGui::Begin("Collectible", &myIsActive, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::InputFloat("Speed", &mySpeed, 0.0f, 200.0f);
	ImGui::InputFloat("Radius From Target", &myMinRadiusFromTarget, 0.0f, 200.0f);
	ImGui::InputFloat("Idle Movement Speed", &myIdleMovementSpeed, 0.0f, 100.0f);
	ImGui::InputFloat("Idle Movement Distance", &myIdleMovementDistance, 0.0f, 200.0f);

	ImGui::End();
}