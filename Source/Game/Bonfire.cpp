#include "stdafx.h"
#include "Bonfire.hpp"

#include "SpriteComponent.h"
#include "AnimationComponent.hpp"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"

#include "Player.hpp"
#include "Collectible.hpp"

#include "../External/Headers/CU/Utilities.h"

#include "PostMaster.hpp"
#include "AudioManager.h"
#include "DataManager.h"

Bonfire::Bonfire(Scene* aScene, const unsigned int anIndex) : GameObject(aScene), myBonfireIndex(anIndex)
{
	SetPivot(v2f(0.5f, 1.0f));
	SetZIndex(91);

	myCollectibleIndex = 0;
	myTurnInDistance = 50.0f;
	myTurnInSpeed = 50.0f;
	myActivateParticle = false;

	myHasBeenActivated = DataManager::GetInstance().GetBonfireState(anIndex);

	SpriteComponent* spriteIdle = AddComponent<SpriteComponent>();
	spriteIdle->SetSpritePath("Sprites/Objects/Bonfire.dds");
	spriteIdle->SetSize(v2f(32.0f, 32.0f));

	PhysicsComponent* physics = AddComponent<PhysicsComponent>();
	physics->SetCanCollide(false);
	physics->SetIsStatic(true);
	physics->SetApplyGravity(false);

	ColliderComponent* collider = AddComponent<ColliderComponent>();
	collider->SetSize(32.0f, 32.0f);

	myAnimations[0] = Animation(false, false, false, 0, 1, 1, 0.1f, spriteIdle, 32, 32);
	myAnimations[1] = Animation(false, true, false, 0, 2, 2, 0.1f, spriteIdle, 32, 32);

	AnimationComponent* animation = AddComponent<AnimationComponent>();
	animation->SetSprite(spriteIdle);
	animation->SetAnimation(&myAnimations[0]);
	spriteIdle->SetSize(v2f(32.0f, 32.0f));

	if (myHasBeenActivated)
	{
		GetComponent<AnimationComponent>()->SetAnimation(&myAnimations[1]);
	}

	PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::BonfireIdleParticle, GetPosition()));
}

void Bonfire::Update(const float& aDeltaTime)
{
	if (!myActivateParticle)
	{
		PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::BonfireIdleParticle, GetPosition()));
		myActivateParticle = true;
	}

	GameObject::Update(aDeltaTime);
}

void Bonfire::OnCollision(GameObject* aGameObject)
{
	Player* player = dynamic_cast<Player*>(aGameObject);
	if (player)
	{
		if (!myHasBeenActivated)
		{
			GetComponent<AnimationComponent>()->SetAnimation(&myAnimations[1]);
		}
		
		PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::PlayerReachedBonfire, 0));

		if (!myHasBeenActivated)
		{
			myHasBeenActivated = true;
			AudioManager::GetInstance()->PlayAudio(AudioList::BonfireActivated);
			DataManager::GetInstance().SaveBonfireState(myBonfireIndex, myHasBeenActivated);
			PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::BonfireWakeupTopParticle, GetPosition()));
			PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::BonfireWakeupExplosionParticle, GetPosition()));
		}
	}
}
const unsigned int Bonfire::GetBonfireIndex() const
{
	return myBonfireIndex;
}