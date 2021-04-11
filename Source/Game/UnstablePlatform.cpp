#include "stdafx.h"
#include "UnstablePlatform.hpp"

#include "SpriteComponent.h"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"
#include "AnimationComponent.hpp"
#include "PostMaster.hpp"

#include "AudioManager.h"

#include "Player.hpp"
#include "LevelScene.h"

UnstablePlatform::UnstablePlatform(Scene* aLevelScene)
	:
	Platform(aLevelScene),
	myDestroyTime(0.0f),
	myRespawnTime(0.0f),
	myTimer(0.0f),
	myCollidedWithPlayer(false),
	myIsDeactivated(false)
{
	SetZIndex(94);
	myMaterial = 1;
}

void UnstablePlatform::Update(const float& aDeltaTime)
{
	if (myCollidedWithPlayer || myIsDeactivated)
	{
		myTimer -= aDeltaTime;
	}

	if (myCollidedWithPlayer && myTimer <= 0)
	{
		myTimer = myRespawnTime;
		myCollidedWithPlayer = false;
		DeactivatePlatform();
	}
	else if(myTimer <= 0 && !myCollidedLastFrame && myIsDeactivated)
	{
		ActivatePlatform();
	}

	myPlayerIsOnTop = false;
	myCollidedLastFrame = false;

	Platform::Update(aDeltaTime);
}

void UnstablePlatform::SetTimerProperties(const float& aDestroyTime, const float& aRespawnTime)
{
	myDestroyTime = aDestroyTime;
	myRespawnTime = aRespawnTime;

	myTimer = myDestroyTime;
}

void UnstablePlatform::OnCollision(GameObject* aGameObject)
{
	Player* player = dynamic_cast<Player*>(aGameObject);
	if (player)
	{	
		myCollidedLastFrame = true;

		if (!myPlayerIsOnTop)
		{
			return;
		}

		player->PlayFootSteps(myMaterial);
		if (!myCollidedWithPlayer && !myIsDeactivated && aGameObject->GetPositionY() < myTransform.myPosition.y)
		{
			player->SetPlatformVelocity(v2f(0.0f, 0.0f));

			PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::UnstablePlatformParticle, this));
			AudioManager::GetInstance()->PlayAudio(AudioList::WeakPlatform);
			myCollidedWithPlayer = true;
			myTimer = myDestroyTime;

			myAnimationComponent->SetAnimation(&myAnimations[0]);
		}
	}
}

void UnstablePlatform::Landed(const int& aOverlapY)
{
	if (aOverlapY >= 0)
	{
		dynamic_cast<Player*>(dynamic_cast<LevelScene*>(this->myScene)->GetPlayer())->PlayLandingSounds(myMaterial);
		myPlayerIsOnTop = true;
	}
}

void UnstablePlatform::SetSpriteToDisable(SpriteComponent* aSprite)
{
	mySpriteToDisable = aSprite;

	myAnimations[0] = Animation(false, true, false, 0, 11, 1, 0.1f, mySpriteToDisable, mySpriteToDisable->GetSizeX(), 16);
	myAnimations[1] = Animation(true, true, false, 10, 11, 1, 0.1f, mySpriteToDisable, mySpriteToDisable->GetSizeX(), 16);

	myAnimationComponent = AddComponent<AnimationComponent>();
	myAnimationComponent->SetSprite(mySpriteToDisable);

	myAnimationComponent->SetAnimation(&myAnimations[1]);
}

void UnstablePlatform::ActivatePlatform()
{
	myAnimationComponent->SetAnimation(&myAnimations[1]);

	GetComponent<PhysicsComponent>()->SetCanCollide(true);
	myIsDeactivated = false;
}

void UnstablePlatform::DeactivatePlatform()
{
	GetComponent<PhysicsComponent>()->SetCanCollide(false);
	myIsDeactivated = true;
}