#include "stdafx.h"
#include "Collectible.hpp"

#include "Player.hpp"
#include "Bonfire.hpp"

#include "SpriteComponent.h"
#include "AnimationComponent.hpp"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"
#include "AudioManager.h"
#include "PostMaster.hpp"

#include "../External/Headers/CU/Utilities.h"

#include "GameWorld.h"
#include "AudioManager.h"
#include "Random.hpp"

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

void Collectible::Init(const v2f& aPosition, eCollectibleType aType)
{
	SetZIndex(129);

	SetPosition(aPosition);
	mySpawnPosition = aPosition;
	myTargetPosition = aPosition;

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

	if (DataManager::GetInstance().GetCollectableInfo(myID).myCollectedState)
	{
		spriteIdle->SetColor(v4f(1.0f, 1.0f, 1.0f, 0.5f));
		spritePickup->SetColor(v4f(1.0f, 1.0f, 1.0f, 0.5f));
	}

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
			myWasCollected = true;
			myTarget = aGameObject;
			AudioManager::GetInstance()->PlayAudio(AudioList::CollectableV1);
			ActivateTrailEffect();
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
		AudioManager::GetInstance()->PlayAudio(AudioList::CollectibleDown);
		GetComponent<AnimationComponent>()->SetAnimation(&myAnimations[1]);
		myWasTurnedIn = true;

		DataManager::GetInstance().SaveCollectedCollectible(myID);
		PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::TurnedInCollectible, 0));
	}
	else if (GetComponent<AnimationComponent>()->GetIsDisplayedOnce() && GetComponent<AnimationComponent>()->GetHasBeenDisplayedOnce())
	{
		ActivateCollectedEffect();
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

const void Collectible::ActivateTrailEffect()
{
	if (myType == eCollectibleType::Easy)
	{
		PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::CollectibleTrailEffectEasy, this));
	}
	else if (myType == eCollectibleType::Medium)
	{
		PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::CollectibleTrailEffectMedium, this));
	}
	else if (myType == eCollectibleType::Hard)
	{
		PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::CollectibleTrailEffectHard, this));
	}
}

const void Collectible::ActivateCollectedEffect()
{
	if (myType == eCollectibleType::Easy)
	{
		PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::CollectibleCollectedParticleEasy, GetPosition()));
	}
	else if (myType == eCollectibleType::Medium)
	{
		PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::CollectibleCollectedParticleMedium, GetPosition()));
	}
	else if (myType == eCollectibleType::Hard)
	{
		PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::CollectibleCollectedParticleHard, GetPosition()));
	}
}
