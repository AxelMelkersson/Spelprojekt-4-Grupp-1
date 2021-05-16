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
#include "UIPopUp.h"

#include "../External/Headers/CU/Utilities.h"

#include "GameWorld.h"
#include "AudioManager.h"
#include "DataManager.h"
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
	myWasCollectedBefore(false),
	myWasCollected(false),
	myWasTurnedIn(false),
	myWasTurnedInWhenEnteringDoor(false),
	myID(anID),
	myBonfireID(aBonfireID),
	myFlashTime(0.15f),
	myFlashing(false),
	myFlashFrameIndex(0)
{
	Subscribe(eMessageType::PlayerSafeLanded);
	Subscribe(eMessageType::PlayerDeath);
	Subscribe(eMessageType::PlayerEnterDoor);
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
	std::string whiteSpritePath;
	switch (aType)
	{
	case eCollectibleType::Easy: {
		spritePath = "Sprites/Objects/Collectible3.dds";
		spritePickupPath = "Sprites/Objects/Collectible3Pickup.dds";
		break;
	}
	case eCollectibleType::Medium: {
		spritePath = "Sprites/Objects/Collectible2.dds";
		spritePickupPath = "Sprites/Objects/Collectible2Pickup.dds";
		break;
	}
	case eCollectibleType::Hard: {
		spritePath = "Sprites/Objects/Collectible1.dds";
		spritePickupPath = "Sprites/Objects/Collectible1Pickup.dds";
		break;
	}
	}
	whiteSpritePath = "Sprites/Objects/CollectibleWhite.dds";

	SpriteComponent* spriteIdle = AddComponent<SpriteComponent>();
	spriteIdle->SetSpritePath(spritePath);
	spriteIdle->SetSize(v2f(16.0f, 16.0f));

	SpriteComponent* spritePickup = AddComponent<SpriteComponent>();
	spritePickup->SetSpritePath(spritePickupPath);
	spritePickup->SetSize(v2f(16.0f, 16.0f));
	spritePickup->Deactivate();

	SpriteComponent* whiteSprite = AddComponent<SpriteComponent>();
	whiteSprite->SetSpritePath(whiteSpritePath);
	whiteSprite->SetSize(v2f(16.0f, 16.0f));
	whiteSprite->Deactivate();

	if (DataManager::GetInstance().GetCollectableInfo(myID).myCollectedState)
	{
		spriteIdle->SetColor(v4f(1.0f, 1.0f, 1.0f, 0.3f));
		spritePickup->SetColor(v4f(1.0f, 1.0f, 1.0f, 0.3f));
		whiteSprite->SetColor(v4f(1.0f, 1.0f, 1.0f, 0.3f));

		myWasCollectedBefore = true;
	}

	myAnimations[0] = Animation(false, false, false, 0, 7, 7, 0.14f, spriteIdle, 16, 16);
	myAnimations[1] = Animation(false, true, false, 0, 8, 8, 0.09f, spritePickup, 16, 16);
	myAnimations[2] = Animation(false, false, false, 0, 7, 7, 0.14f, whiteSprite, 16, 16);

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

	if (myFlashing)
	{
		myFlashTime -= aDeltaTime;
		if (GetComponent<AnimationComponent>()->GetCurrentIndex() != myFlashFrameIndex)
		{
			GetComponent<AnimationComponent>()->SetUpdateTime(0.14f);
			myAnimations[2].myUpdateTime = 0.14f;
		}
		if (myFlashTime <= 0)
		{
			myAnimations[2].myUpdateTime = 0.14f;
			myFlashFrameIndex = GetComponent<AnimationComponent>()->GetCurrentIndex();
			GetComponent<AnimationComponent>()->SetAnimation(&myAnimations[0], myFlashFrameIndex);
			myFlashing = false;
		}
	}
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
	Player* player = dynamic_cast<Player*>(aGameObject);
	if (player)
	{
		if (!myWasCollected && !myWasTurnedIn)
		{
			myTarget = aGameObject;
			AudioManager::GetInstance()->PlayAudio(AudioList::CollectableV1);
			ActivateTrailEffect();

			myFlashing = true;
			myFlashFrameIndex = GetComponent<AnimationComponent>()->GetCurrentIndex();
			myAnimations[2].myUpdateTime = GetComponent<AnimationComponent>()->GetTimer();
			GetComponent<AnimationComponent>()->SetAnimation(&myAnimations[2], myFlashFrameIndex);
		}
		myWasCollected = true;
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
		AudioManager::GetInstance()->ForceStop(AudioList::CollectibleDown);
		AudioManager::GetInstance()->PlayIfAvailable(AudioList::CollectibleDown);
		GetComponent<AnimationComponent>()->SetAnimation(&myAnimations[1]);
		myWasTurnedIn = true;

		DataManager::GetInstance().SaveCollectedCollectible(myID);
		if (!myWasCollectedBefore && !myWasTurnedInWhenEnteringDoor)
		{
			CheckPopUpMessages();
		}

		PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::TurnedInCollectible, 0));
	}
	else if (GetComponent<AnimationComponent>()->GetIsDisplayedOnce() && GetComponent<AnimationComponent>()->GetHasBeenDisplayedOnce())
	{
		AudioManager::GetInstance()->ForceStop(AudioList::CollectibleGone);
		AudioManager::GetInstance()->PlayAudio(AudioList::CollectibleGone);
		ActivateCollectedEffect();
		Destroy();
	}
}

void Collectible::Notify(const Message& aMessage)
{
	if (aMessage.myMessageType == eMessageType::PlayerSafeLanded)
	{
		if (myWasCollected && !myFlashing)
		{
			TurnIn();
		}
	}
	else if (aMessage.myMessageType == eMessageType::PlayerDeath)
	{
		Reset();
	}
	else if (aMessage.myMessageType == eMessageType::PlayerEnterDoor)
	{
		if (myWasCollected && !myWasCollectedBefore)
		{
			myWasTurnedInWhenEnteringDoor = true;
			PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::PopUpNextLevel, static_cast<int>(myType)), true);
			TurnIn();
		}
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

const void Collectible::CheckPopUpMessages()
{
	if (myType == eCollectibleType::Easy)
	{
		PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::PopUpMessageE, 0));
	}
	else if (myType == eCollectibleType::Medium)
	{
		PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::PopUpMessageM, 0));

	}
	else if (myType == eCollectibleType::Hard)
	{
		PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::PopUpMessageH, 0));
	}
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
