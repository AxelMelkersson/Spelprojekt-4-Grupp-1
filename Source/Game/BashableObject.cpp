#include "stdafx.h"
#include "BashableObject.hpp"

#include "SpriteComponent.h"
#include "AnimationComponent.hpp"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"
#include "BashComponent.hpp"
#include "WaypointComponent.hpp"

BashableObject::BashableObject(Scene* aLevelScene) : GameObject(aLevelScene)
{
	WaypointComponent* waypointComponent = AddComponent<WaypointComponent>();
	waypointComponent->SetOwner(this);

	SetZIndex(97);
}

void BashableObject::Init(const v2f& aPosition, const float& aRadius)
{
	SetPosition(aPosition);
	SetPivot(v2f(0.5f, 0.5f));

	BashComponent* bashComponent = AddComponent<BashComponent>();
	bashComponent->SetRadius(aRadius);

	myHighlight = AddComponent<SpriteComponent>();
	myHighlight->SetSpritePath("Sprites/Objects/BashableInRange.dds");
	myHighlight->SetSize(v2f(16.0f, 16.0));

	myIdle = AddComponent<SpriteComponent>();
	myIdle->SetSpritePath("Sprites/Objects/Bashable.dds");
	myIdle->SetSize(v2f(16.0f, 16.0));

	myAnimations[0] = Animation(false, false, false, 0, 7, 7, 0.125f, myIdle, 16, 16);
	myAnimations[1] = Animation(false, false, false, 0, 7, 7, 0.125f, myHighlight, 16, 16);

	AnimationComponent* animation = AddComponent<AnimationComponent>();
	animation->SetSprite(myIdle);
	animation->SetAnimation(&myAnimations[0]);
	myIdle->SetSize(v2f(16.0f, 16.0));

	AnimationComponent* animationHighlight = AddComponent<AnimationComponent>();
	animationHighlight->SetSprite(myHighlight);
	animationHighlight->SetAnimation(&myAnimations[1]);
	myHighlight->SetSize(v2f(16.0f, 16.0));
	myHighlight->Deactivate();

	PhysicsComponent* physics = AddComponent<PhysicsComponent>();
	physics->SetCanCollide(false);
	physics->SetIsStatic(false);
	physics->SetApplyGravity(false);

	ColliderComponent* collider = AddComponent<ColliderComponent>();
	collider->SetSize(v2f(50.0f, 50.0f));

	GameObject::Init();
}
void BashableObject::Update(const float& aDeltaTime)
{
	GetComponent<WaypointComponent>()->Move(aDeltaTime);

	GameObject::Update(aDeltaTime);
}

void BashableObject::Highlight()
{
	myHighlight->Activate();
	myIdle->Deactivate();
}

void BashableObject::Dehighlight()
{
	myIdle->Activate();
	myHighlight->Deactivate();
}