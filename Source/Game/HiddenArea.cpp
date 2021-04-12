#include "stdafx.h"

#include "HiddenArea.hpp"
#include "Player.hpp"

#include "SpriteBatchComponent.h"
#include "SpriteComponent.h"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"

#include "AudioManager.h"

#include "Scene.h"

#include "../External/Headers/CU/Utilities.h"

HiddenArea::HiddenArea(Scene* aLevelScene, const v2f& aPosition, const v2f& aSize)
	:
	GameObject(aLevelScene),
	myPlayerCollided(false),
	myHiddenSpriteBatch(nullptr),
	myOpacity(1.0f),
	myOpacityChangeSpeed(3.0f),
	myHasBeenFound(false),
	myTimer(0.5f)
{
	SetZIndex(130);
	SetPosition(aPosition);
	SetPivot(v2f(0.0f, 0.0f));

	PhysicsComponent* physics = AddComponent<PhysicsComponent>();
	physics->SetCanCollide(false);
	physics->SetIsStatic(false);
	physics->SetApplyGravity(false);

	ColliderComponent* collider = AddComponent<ColliderComponent>();
	collider->SetPosition({ aSize.x / 2.0f, aSize.y / 2.0f });
	collider->SetSize(aSize);
}

void HiddenArea::Update(const float& aDeltaTime)
{
	myTimer -= aDeltaTime;
	if (myTimer <= 0)
	{
		myTimer = 0;
	}

	if (myPlayerCollided)
	{
		myOpacity = Utils::Lerp(myOpacity, 0.0f, myOpacityChangeSpeed * aDeltaTime);
	}

	myHiddenSpriteBatch->SetOpacity(myOpacity);

	GameObject::Update(aDeltaTime);
}

void HiddenArea::SetBatch(SpritebatchComponent* aHiddenSpriteBatch)
{
	myHiddenSpriteBatch = aHiddenSpriteBatch;
}
void HiddenArea::OnCollision(GameObject* aGameObject)
{
	Player* player = dynamic_cast<Player*>(aGameObject);
	if (player)
	{
		myPlayerCollided = true;
		if (myTimer > 0)
		{
			myHasBeenFound = true;
			myOpacity = 0.0f;
		}
		else if (!myHasBeenFound)
		{
			AudioManager::GetInstance()->PlayAudio(AudioList::HiddenRoomUnlock);
			myHasBeenFound = true;
		}
	}
}