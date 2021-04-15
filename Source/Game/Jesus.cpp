#include "stdafx.h"
#include "Jesus.hpp"

#include "SpriteComponent.h"
#include "AnimationComponent.hpp"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"

#include "../External/Headers/CU/Utilities.h"

#include "Player.hpp"

Jesus::Jesus(Scene* aLevelScene)
	:
	GameObject(aLevelScene),
	myTarget(nullptr),
	myTurnSpeed(0.0f),
	myAcceleration(0.0f),
	myRetardation(0.0f),
	myCurrentSpeed(0.0f),
	myMaxSpeed(0.0f),
	myMinSpeed(0.0f),
	myMaxTurnBeforeRetardation(0.0f),
	myCloseDistance(0.0f),
	myCurrentAnimationIndex(0),
	myEatTarget(false)
{
	
}

Jesus::~Jesus()
{

}

void Jesus::Init(const v2f& aPosition)
{
	Subscribe(eMessageType::PlayerDeath);

	SetPosition(aPosition);
	mySpawnPosition = aPosition;

	SetPivot(v2f(0.5f, 0.5f));

	SetZIndex(145);

	InitAnimations();

	myDirection = v2f(1.0f, 0.0f);
	myTurnSpeed = 300.0f;
	myAcceleration = 0.4f;
	myRetardation = 1.0f;
	myMaxSpeed = 500.0f;
	myMinSpeed = 10.0f;
	myCloseDistance = 100.0f;

	myMaxTurnBeforeRetardation = 0.5f;

	PhysicsComponent* physics = AddComponent<PhysicsComponent>();
	physics->SetCanCollide(false);
	physics->SetIsStatic(false);
	physics->SetApplyGravity(false);

	ColliderComponent* collider = AddComponent<ColliderComponent>();
	collider->SetPosition(v2f(0.0f, 0.0f));
	collider->SetSize(2.0f, 6.0f);

	GameObject::Init();
}

void Jesus::InitAnimations()
{
	SpriteComponent* spriteIdle = AddComponent<SpriteComponent>();
	spriteIdle->SetSpritePath("Sprites/Ghoul/GhoulClose.png");
	spriteIdle->SetSize(v2f(32.0f, 32.0f));

	spriteIdle->SetForceRender(true);

	SpriteComponent* spriteClose = AddComponent<SpriteComponent>();
	spriteClose->SetSpritePath("Sprites/Ghoul/GhoulClose.png");
	spriteClose->SetSize(v2f(32.0f, 32.0f));
	spriteClose->Deactivate();

	SpriteComponent* spriteEat = AddComponent<SpriteComponent>();
	spriteEat->SetSpritePath("Sprites/Ghoul/GhoulEat.png");
	spriteEat->SetSize(v2f(32.0f, 32.0f));
	spriteEat->Deactivate();

	myAnimations[0] = Animation(false, false, false, 0, 2, 2, 0.5f, spriteIdle, 32, 32);
	myAnimations[1] = Animation(false, true, false, 0, 3, 3, 0.15f, spriteClose, 32, 32);
	myAnimations[2] = Animation(false, true, false, 0, 4, 4, 0.08f, spriteEat, 32, 32);
	myAnimations[3] = Animation(true, true, false, 2, 3, 3, 0.15f, spriteClose, 32, 32);

	AnimationComponent* animation = AddComponent<AnimationComponent>();
	animation->SetSprite(spriteIdle);
	animation->SetAnimation(&myAnimations[0]);
	spriteIdle->SetSize(v2f(32.0f, 32.0f));
}

void Jesus::Update(const float& aDeltaTime)
{
	if (myEatTarget)
	{
		if (GetComponent<AnimationComponent>()->GetHasBeenDisplayedOnce() && GetComponent<AnimationComponent>()->GetIsDisplayedOnce())
		{
			dynamic_cast<Player*>(myTarget)->Kill();
		}
	}
	else
	{
		Move(aDeltaTime);
	}

	GameObject::Update(aDeltaTime);

#ifdef _DEBUG
	UpdateImGui();
#endif //DEBUG
}

void Jesus::Move(const float& aDeltaTime)
{
	if (myTarget)
	{
		const float calculatedTurnSpeed = myTurnSpeed / (myCurrentSpeed + 1.0f);
		const v2f directionToTarget = (myTarget->GetPosition() - GetPosition()).GetNormalized();

		if (Utils::Abs(directionToTarget.x - myDirection.x) > myMaxTurnBeforeRetardation || Utils::Abs(directionToTarget.y - myDirection.y) > myMaxTurnBeforeRetardation)
		{
			myCurrentSpeed = Utils::Lerp(myCurrentSpeed, myMinSpeed, myRetardation * aDeltaTime);
		}
		else
		{
			myCurrentSpeed = Utils::Lerp(myCurrentSpeed, myMaxSpeed, myAcceleration * aDeltaTime);
		}

		myDirection.x = Utils::Lerp(myDirection.x, directionToTarget.x, calculatedTurnSpeed * aDeltaTime);
		myDirection.y = Utils::Lerp(myDirection.y, directionToTarget.y, calculatedTurnSpeed * aDeltaTime);

		if (myDirection.x >= 0.0f)
		{
			for (Animation& animation : myAnimations)
			{
				animation.mySpriteComponent->SetSizeX(32.0f);
			}
		}
		else
		{
			for (Animation& animation : myAnimations)
			{
				animation.mySpriteComponent->SetSizeX(-32.0f);
			}
		}

		myTransform.myPosition += myDirection * myCurrentSpeed * aDeltaTime;

		AnimationStates();
	}
}

void Jesus::AnimationStates()
{
	const float distance = (myTarget->GetPosition() - GetPosition()).Length();
	if (distance < myCloseDistance && myCurrentAnimationIndex != 1)
	{
		GetComponent<AnimationComponent>()->SetAnimation(&myAnimations[1]);
		myCurrentAnimationIndex = 1;
	}
	else if(distance > myCloseDistance && myCurrentAnimationIndex != 0)
	{
		GetComponent<AnimationComponent>()->SetAnimation(&myAnimations[0]);
		myCurrentAnimationIndex = 0;
	}
}

void Jesus::SetTarget(GameObject* aTarget)
{
	myTarget = aTarget;
}

void Jesus::OnCollision(GameObject* aGameObject)
{
	Player* player = dynamic_cast<Player*>(aGameObject);
	if (player && !myEatTarget)
	{
		GetComponent<AnimationComponent>()->SetAnimation(&myAnimations[2]);
		GetComponent<AnimationComponent>()->SetNextAnimation(&myAnimations[3]);
		myEatTarget = true;
		player->Eaten();
	}
}

void Jesus::Reset()
{
	SetPosition(mySpawnPosition);
	myCurrentAnimationIndex = 0;
	GetComponent<AnimationComponent>()->SetAnimation(&myAnimations[0]);
	myEatTarget = false;
}

void Jesus::Notify(const Message& aMessage)
{
	if (aMessage.myMessageType == eMessageType::PlayerDeath)
	{
		Reset();
	}
}


#ifdef _DEBUG
void Jesus::UpdateImGui()
{
	ImGui::Begin("Jesus", &myIsActive, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::InputFloat("Max Speed", &myMaxSpeed, 0.0f, 2000.0f);
	ImGui::InputFloat("Min Speed", &myMinSpeed, 0.0f, 2000.0f);
	ImGui::InputFloat("Acceleration", &myAcceleration, 0.0f, 100.0f);
	ImGui::InputFloat("Retardation", &myRetardation, 0.0f, 100.0f);
	ImGui::InputFloat("Turn Speed", &myTurnSpeed, 0.0f, 2000.0f);
	ImGui::InputFloat("Max Turn Before Retardation", &myMaxTurnBeforeRetardation, 0.0f, 2.0f);
	ImGui::InputFloat("Close Distance", &myCloseDistance, 0.0f, 2000.0f);

	ImGui::End();
}
#endif // DEBUG