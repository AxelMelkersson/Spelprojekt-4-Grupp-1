#include "stdafx.h"
#include "UnstablePlatform.hpp"

#include "SpriteComponent.h"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"

#include "AudioManager.h"

#include "Player.hpp"

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
	else if(myTimer <= 0 && !myCollidedLastFrame)
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

		if (!myCollidedWithPlayer && !myIsDeactivated && aGameObject->GetPositionY() < myTransform.myPosition.y)
		{
			player->PlayFootSteps(myMaterial);
			player->SetPlatformVelocity(v2f(0.0f, 0.0f));

			AudioManager::GetInstance()->PlayAudio(AudioList::WeakPlatform);
			myCollidedWithPlayer = true;
			myTimer = myDestroyTime;
		}
	}
}

void UnstablePlatform::Landed(const int& aOverlapY)
{
	if (aOverlapY >= 0)
	{
		myPlayerIsOnTop = true;
	}
}

void UnstablePlatform::SetSpriteToDisable(SpriteComponent* aSprite)
{
	mySpriteToDisable = aSprite;
}

void UnstablePlatform::ActivatePlatform()
{
	GetComponent<PhysicsComponent>()->SetCanCollide(true);
	mySpriteToDisable->Activate();
	myIsDeactivated = false;
}

void UnstablePlatform::DeactivatePlatform()
{
	GetComponent<PhysicsComponent>()->SetCanCollide(false);
	mySpriteToDisable->Deactivate();
	myIsDeactivated = true;
}