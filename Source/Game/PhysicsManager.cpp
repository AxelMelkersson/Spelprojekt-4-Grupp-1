/*
*	Author: Elia R�nning
*/

#include "stdafx.h"
#include "PhysicsManager.h"
#include <vector>
#include "Transform.hpp"
#include "GameObject.h"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"
#include "BashComponent.hpp"
#include "GameWorld.h"
#include "../External/Headers/CU/Utilities.h"

#include "Player.hpp"
#include "EnemyProjectile.h"

PhysicsManager::PhysicsManager()
	: myColliders(std::vector<ColliderComponent*>())
{}

void PhysicsManager::PhysicsUpdate(const float& aDeltaTime, std::vector<GameObject*>& aGameObjects)
{
	UpdateObjectVelocity(aDeltaTime, aGameObjects);

	bool playerCollisionOnly = false;

	for (int index = 0U; index < static_cast<int>(myColliders.size()); ++index)
	{
		ColliderComponent*& collider1 = myColliders[index];
		GameObject* object1 = collider1->GetGameObject();

		if (!object1 || !object1->IsActive())
		{
			continue;
		}

		PhysicsComponent* physics = object1->GetComponent<PhysicsComponent>();
		if (physics == nullptr)
		{
			continue;
		}

		if ((dynamic_cast<Player*>(object1) && !playerCollisionOnly) || (dynamic_cast<EnemyProjectile*>(object1)))
		{
			OnlyPlayerCollision(object1, collider1, physics);

			if (dynamic_cast<Player*>(object1))
			{
				playerCollisionOnly = true;
			}
		}

		if (index < static_cast<int>(myColliders.size()) - 1)
		{
			for (int iterator = index + 1U; iterator < static_cast<int>(myColliders.size()); ++iterator)
			{
				ColliderComponent*& collider2 = myColliders[iterator];
				GameObject* object2 = collider2->GetGameObject();

				if (!object2 || !object2->IsActive())
				{
					continue;
				}

				PhysicsComponent* object2Physics = object2->GetComponent<PhysicsComponent>();
				if (object2Physics == nullptr)
				{
					continue;
				}

				if (physics->GetIsStatic() && object2Physics->GetIsStatic())
				{
					continue;
				}

				if (dynamic_cast<Player*>(object2) && !playerCollisionOnly)
				{
					OnlyPlayerCollision(object2, collider2, object2Physics);
					playerCollisionOnly = true;
				}

				CheckOverlap(object1, object2, physics, object2Physics, collider1, collider2);
			}
		}
	}
}

void PhysicsManager::UpdateObjectVelocity(const float& aDeltaTime, std::vector<GameObject*>& aGameObjects)
{
	for (GameObject* obj : aGameObjects)
	{
		if (!obj || !obj->IsActive())
		{
			continue;
		}
		PhysicsComponent* physics = obj->GetComponent<PhysicsComponent>();
		if (!physics || physics->GetIsStatic())
		{
			continue;
		}

		if (physics->GetApplyGravity())
		{
			physics->SetVelocityY(physics->GetVelocityY() + ourGravity * aDeltaTime);
		}

		obj->SetPosition(obj->GetPosition() + (physics->GetVelocity() * aDeltaTime));
	}
}

void PhysicsManager::CheckOverlap(GameObject* aObj1, GameObject* aObj2, PhysicsComponent* aObj1Physics, PhysicsComponent* aObj2Physics, ColliderComponent* aCollider1, ColliderComponent* aCollider2)
{
	const v2f obj1Size = aCollider1->GetSize();
	const v2f obj2Size = aCollider2->GetSize();

	const v2f obj1min = aObj1->GetPosition() + aCollider1->GetPosition() - (obj1Size * .5f);
	const v2f obj1max = obj1min + obj1Size;
	const v2f obj2min = aObj2->GetPosition() + aCollider2->GetPosition() - (obj2Size * .5f);
	const v2f obj2max = obj2min + obj2Size;

	const bool xAxisOverlap = obj1min.x <= obj2max.x && obj1max.x >= obj2min.x;
	const bool yAxisOverlap = obj1min.y <= obj2max.y && obj1max.y >= obj2min.y;

	const float insensitivity = 5.0f;
	const float onewWayInsensitivity = 2.0f;

	CheckBashCollision(aObj1, aObj2);

	if (OneWayCheck(onewWayInsensitivity, aObj1, aObj2, obj1min, obj1max, obj2min, obj2max))
	{
		return;
	}

	if (xAxisOverlap && yAxisOverlap)
	{
		OverlapCalculation(aObj1, aObj2, aObj1Physics, aObj2Physics, obj1min, obj1max, obj2min, obj2max);
		aObj1->OnCollision(aObj2);
		aObj2->OnCollision(aObj1);
	}
	else if (obj1min.x + insensitivity < obj2max.x && obj1max.x - insensitivity > obj2min.x)
	{
		const float yDifference = obj2min.y - obj1max.y;
		AlmostCollision(aObj1, yDifference);
		AlmostCollision(aObj2, yDifference);
	}
}

void PhysicsManager::OverlapCalculation(GameObject* aObj1, GameObject* aObj2, PhysicsComponent* aObj1Physics, PhysicsComponent* aObj2Physics, const v2f& aObj1Min, const v2f& aObj1Max, const v2f& aObj2Min, const v2f& aObj2Max)
{
	if (!aObj2Physics->GetCanCollide() || !aObj1Physics->GetCanCollide())
	{
		return;
	}

	const float overlapX1 = aObj1Max.x - aObj2Min.x;
	const float overlapX2 = aObj1Min.x - aObj2Max.x;
	const float overlapX = Utils::Abs(overlapX1) < Utils::Abs(overlapX2) ? overlapX1 : overlapX2;

	const float overlapY1 = aObj1Min.y - aObj2Max.y;
	const float overlapY2 = aObj1Max.y - aObj2Min.y;

	const float overlapY = Utils::Abs(overlapY1) < Utils::Abs(overlapY2) ? overlapY1 : overlapY2;

	const bool& obj1Static = aObj1Physics->GetIsStatic();
	const bool& obj2Static = aObj2Physics->GetIsStatic();


	if (Utils::Abs(overlapX) > Utils::Abs(overlapY))
	{
		if (!obj1Static)
		{
			aObj1->SetPositionY(aObj1->GetPositionY() - overlapY);
			aObj1Physics->SetVelocityY(0.0f);
		}
		if (!obj2Static)
		{
			aObj2->SetPositionY(aObj2->GetPositionY() + overlapY);
			aObj2Physics->SetVelocityY(0.0f);
		}

		aObj2->Landed(overlapY2);
		aObj1->Landed(overlapY1);
	}
	else
	{
		if (!obj1Static)
		{
			aObj1->SetPositionX(aObj1->GetPositionX() - overlapX);
			aObj1Physics->SetVelocityX(0.0f);
		}
		if (!obj2Static)
		{
			aObj2->SetPositionX(aObj2->GetPositionX() + overlapX);
			aObj2Physics->SetVelocityX(0.0f);
		}

		aObj1->SideCollision(overlapX1);
		aObj2->SideCollision(overlapX2);
	}
}

const void PhysicsManager::AlmostCollision(GameObject* aObject, const float& aYDistance)
{
	Player* player = dynamic_cast<Player*>(aObject);
	if (player)
	{
		player->TryLetJumpWhenFalling(aYDistance);
	}
}

bool PhysicsManager::OneWayCheck(const float& aInSensitivity, GameObject* aObj1, GameObject* aObj2, const v2f& aObj1Min, const v2f& aObj1Max, const v2f& aObj2Min, const v2f& aObj2Max)
{
	if (aObj1->GetComponent<PhysicsComponent>()->GetCollisionType() == PhysicsComponent::eCollisionType::OneWay)
	{
		float playerPlatformVelocity = 0;

		Player* player = dynamic_cast<Player*>(aObj2);

		if (player)
		{
			playerPlatformVelocity = player->GetPlatformVelocity().y;
		}

		const float objVelocity = aObj2->GetComponent<PhysicsComponent>()->GetVelocityY() - playerPlatformVelocity;

		const float adjustedInsensitivity = aInSensitivity + objVelocity * CGameWorld::GetInstance()->GetTimer()->GetDeltaTime();
		
		if (aObj2Max.y > aObj1Min.y + adjustedInsensitivity || objVelocity < 0)
		{
			return true;
		}
	}
	else if (aObj2->GetComponent<PhysicsComponent>()->GetCollisionType() == PhysicsComponent::eCollisionType::OneWay)
	{
		float playerPlatformVelocity = 0;

		Player* player = dynamic_cast<Player*>(aObj1);

		if (player)
		{
			playerPlatformVelocity = player->GetPlatformVelocity().y;
		}

		const float objVelocity = aObj1->GetComponent<PhysicsComponent>()->GetVelocityY() - playerPlatformVelocity;

		const float adjustedInsensitivity = aInSensitivity + objVelocity * CGameWorld::GetInstance()->GetTimer()->GetDeltaTime();

		if (aObj1Max.y > aObj2Min.y + adjustedInsensitivity || objVelocity < 0)
		{
			return true;
		}
	}

	return false;
}

void PhysicsManager::CheckBashCollision(GameObject* aObj1, GameObject* aObj2)
{
	BashComponent* obj1BashComponent = aObj1->GetComponent<BashComponent>();
	BashComponent* obj2BashComponent = aObj2->GetComponent<BashComponent>();

	if (obj1BashComponent)
	{
		aObj2->BashCollision(aObj1, obj1BashComponent);
	}

	if (obj2BashComponent)
	{
		aObj1->BashCollision(aObj2, obj2BashComponent);
	}
}

void PhysicsManager::OnlyPlayerCollision(GameObject* aPlayer, ColliderComponent* aPlayerCollider, PhysicsComponent* aPlayerPhysics)
{
	for (int iterator = 0; iterator < static_cast<int>(myOnlyPlayerCollisionColliders.size()); ++iterator)
	{
		CheckOverlap(aPlayer, myOnlyPlayerCollisionColliders[iterator]->GetGameObject(), aPlayerPhysics, myOnlyPlayerCollisionColliders[iterator]->GetGameObject()->GetComponent<PhysicsComponent>(), aPlayerCollider, myOnlyPlayerCollisionColliders[iterator]);
	}
}

void PhysicsManager::RemoveCollider(ColliderComponent* aColliderComponent)
{
	if (!aColliderComponent)
	{
		return;
	}

	if (!aColliderComponent->GetCollideOnlywithPlayer())
	{
		for (int collider = 0; collider < myColliders.size(); ++collider)
		{
			if (myColliders[collider] == aColliderComponent)
			{
				myColliders.erase(myColliders.begin() + collider);
			}
		}
	}
	else
	{
		for (int collider = 0; collider < myOnlyPlayerCollisionColliders.size(); ++collider)
		{
			if (myOnlyPlayerCollisionColliders[collider] == aColliderComponent)
			{
				myOnlyPlayerCollisionColliders.erase(myOnlyPlayerCollisionColliders.begin() + collider);
			}
		}
	}
}