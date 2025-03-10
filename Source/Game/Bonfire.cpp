#include "stdafx.h"
#include "Bonfire.hpp"

#include "SpriteComponent.h"
#include "AnimationComponent.hpp"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"

#include "Player.hpp"
#include "Collectible.hpp"
#include "SpeechBubble.h"

#include "../External/Headers/CU/Utilities.h"

#include "PostMaster.hpp"
#include "AudioManager.h"
#include "DataManager.h"

Bonfire::Bonfire(Scene* aScene, const unsigned int anIndex, const v2f aPos) : GameObject(aScene), myBonfireIndex(anIndex)
{
	SetPosition(aPos);
	SetPivot(v2f(0.5f, 1.0f));
	SetZIndex(91);

	myCollectibleIndex = 0;
	myTurnInDistance = 50.0f;
	myTurnInSpeed = 50.0f;
	myActivateParticle = false;

	myHasBeenActivated = DataManager::GetInstance().GetBonfireState(anIndex);

	SpriteComponent* spriteIdle = AddComponent<SpriteComponent>();
	spriteIdle->SetSpritePath("Sprites/Objects/BonfiredDeactivated.dds");
	spriteIdle->SetSize(v2f(24.0f, 24.0f));

	SpriteComponent* spriteActivate = AddComponent<SpriteComponent>();
	spriteActivate->SetSpritePath("Sprites/Objects/BonfiredActivated.dds");
	spriteActivate->SetSize(v2f(24.0f, 24.0f));

	PhysicsComponent* physics = AddComponent<PhysicsComponent>();
	physics->SetCanCollide(false);
	physics->SetIsStatic(true);
	physics->SetApplyGravity(false);

	ColliderComponent* collider = AddComponent<ColliderComponent>();
	collider->SetSize(32.0f, 32.0f);

	myAnimations[0] = Animation(false, true, false, 0, 1, 1, 0.1f, spriteIdle, 24, 24);
	myAnimations[1] = Animation(false, false, false, 0, 10, 10, 0.1f, spriteActivate, 24, 24);

	AnimationComponent* animation = AddComponent<AnimationComponent>();
	animation->SetSprite(spriteIdle);
	animation->SetAnimation(&myAnimations[0]);
	spriteIdle->SetSize(v2f(24.0f, 24.0f));

	spriteActivate->Deactivate();

	if (myHasBeenActivated)
	{
		GetComponent<AnimationComponent>()->SetAnimation(&myAnimations[1]);
	}

	mySpeechBubble = new SpeechBubble(aScene);
	mySpeechBubble->Init(myBonfireIndex, GetPosition());
}

void Bonfire::Update(const float& aDeltaTime)
{
	if (!myActivateParticle && myHasBeenActivated)
	{
		PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::BonfireIdleParticleEasy, GetPosition()));
		PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::BonfireIdleParticleMedium, GetPosition()));
		PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::BonfireIdleParticleHard, GetPosition()));
		myActivateParticle = true;
	}

	GameObject::Update(aDeltaTime);
}

void Bonfire::OnCollision(GameObject* aGameObject)
{
	mySpeechBubble->Speak();

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
			PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::BonfireWakeupTopParticleEasy, GetPosition()));
			PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::BonfireWakeupExplosionParticleEasy, GetPosition()));
			PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::BonfireWakeupTopParticleMedium, GetPosition()));
			PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::BonfireWakeupExplosionParticleMedium, GetPosition()));
			PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::BonfireWakeupTopParticleHard, GetPosition()));
			PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::BonfireWakeupExplosionParticleHard, GetPosition()));
		}
	}
}
const unsigned int Bonfire::GetBonfireIndex() const
{
	return myBonfireIndex;
}