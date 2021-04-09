#include "stdafx.h"
#include "LevelDoor.hpp"

#include "PhysicsComponent.h"
#include "ColliderComponent.h"

#include "Player.hpp"

#include "PostMaster.hpp"

#include "Scene.h"
#include "Camera.h"

LevelDoor::LevelDoor(Scene* aScene)
	:
	GameObject(aScene)
{
	myType = eDoorType::Entry;
	myWasActivated = false;
}

LevelDoor::~LevelDoor()
{

}

void LevelDoor::Init(const eDoorType aDoorType, const v2f& aSize)
{
	SetPivot(v2f(0.0f, 0.0f));

	myType = aDoorType;

	PhysicsComponent* physics = AddComponent<PhysicsComponent>();
	physics->SetCanCollide(false);
	physics->SetIsStatic(true);
	physics->SetApplyGravity(false);

	ColliderComponent* collider = AddComponent<ColliderComponent>();
	collider->SetPosition({ aSize.x / 2.0f, aSize.y / 2.0f });
	collider->SetSize(aSize);
}

void LevelDoor::OnCollision(GameObject* aGameObject)
{
	if (myWasActivated)
	{
		return;
	}

	Player* player = dynamic_cast<Player*>(aGameObject);
	if (player)
	{
		myWasActivated = true;

		int doorType = 0;
		const v2f roomSize = myScene->GetCamera().GetBoundSize();

		if (GetPosition().x < 0.0f)
		{
			doorType = 0;
		}
		else if (GetPosition().x > roomSize.x)
		{
			doorType = 1;
		}
		else if (GetPosition().y < 0.0f)
		{
			doorType = 2;
		}
		else if (GetPosition().y > roomSize.y)
		{
			doorType = 3;
		}

		if (myType == eDoorType::Exit)
		{
			PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::LoadNext, doorType), true);
		}
		else if (myType == eDoorType::Entry)
		{
			PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::LoadPrevious, doorType), true);
		}
		else if (myType == eDoorType::HiddenRoom)
		{
			PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::LoadHiddenRoom, doorType), true);
		}
		else if (myType == eDoorType::MainRoom)
		{
			PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::LoadMainRoom, doorType), true);
		}
	}
}