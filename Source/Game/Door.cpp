#include "stdafx.h"

#include "SpriteComponent.h"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"

#include "Door.h"
#include "Scene.h"
#include "Button.h"

Door::Door(Scene* aLevelScene) : GameObject(aLevelScene), myButton(new Button(aLevelScene))
{
	myDoorIsOpen = false;
	myClosedDoorSprite = nullptr;
	myOpenDoorSprite = nullptr;
}

void Door::Init(const v2f aPosition)
{
	v2f position = aPosition;

	SetPivot({0.f, 0.f});
	SetPosition(position);
	mySize = {8, 24};

	myClosedDoorSprite = AddComponent<SpriteComponent>();
	myClosedDoorSprite->SetSpritePath("Sprites/Objects/DoorClosed.dds");
	myClosedDoorSprite->Activate();
	myClosedDoorSprite->SetSize(mySize);

	myOpenDoorSprite = AddComponent<SpriteComponent>();
	myOpenDoorSprite->SetSpritePath("Sprites/Objects/DoorOpen.dds");
	myOpenDoorSprite->SetSize({ 16.f, 24.f });

	PhysicsComponent* physics = AddComponent<PhysicsComponent>();
	physics->SetCanCollide(true);
	physics->SetIsStatic(true);

	ColliderComponent* collider = AddComponent<ColliderComponent>();
	collider->SetSize({ mySize.x, mySize.y });
	collider->SetPosition({ mySize.x / 2.f, mySize.y / 2.f});

	GameObject::Init();

	myOpenDoorSprite->Deactivate();
}
void Door::Update(const float& aDeltaTime)
{
	if (myButton->GetActiveButton() && !myDoorIsOpen)
	{
		myDoorIsOpen = true;

		myClosedDoorSprite->Deactivate();
		myOpenDoorSprite->Activate();

		GetComponent<PhysicsComponent>()->SetCanCollide(false);
	}

	GameObject::Update(aDeltaTime);
}

void Door::AddButton(v2f aPosition)
{
	myButton->Init(GetPosition(), aPosition);
}