/*
*	Author: Elia Rönning
*/

#include "stdafx.h"
#include "GameObject.h"
#include "Transform.hpp"
#include "GameWorld.h"
#include "Scene.h"
#include "SpriteComponent.h"
#include <math.h>

#include "ColliderComponent.h"
#include "PhysicsManager.h"

GameObject::GameObject(Scene* aScene)
	: myIsActive(false)
	, myZIndex(0)
	, myScene(aScene)
	, myTransform(Transform())
	, myComponents(std::vector<Component*>())
{
	myScene->PushGameObject(this);
}
GameObject::~GameObject()
{
	for (Component*& comp : myComponents)
	{
		if (comp)
		{
			myScene->GetPhysics().RemoveCollider(dynamic_cast<ColliderComponent*>(comp));

			delete comp;
			comp = nullptr;
		}
	}

	if (myScene)
	{
		myScene->RemoveGameObject(this);
	}
}

void GameObject::Init()
{
	for (Component*& comp : myComponents)
	{
		if (comp != nullptr)
		{
			comp->Init(myTransform, *this);
		}
	}
}
void GameObject::Update(const float& aDeltaTime)
{
	aDeltaTime;
	for (Component*& comp : myComponents)
	{
		if (comp != nullptr)
		{
			comp->Update(myTransform, *this);
		}
	}
}
void GameObject::Render()
{
	for (Component*& comp : myComponents)
	{
		if (comp != nullptr)
		{
			comp->Render(myTransform, *this);
		}
	}
}

const Transform& GameObject::GetTransform() const
{
	return myTransform;
}
const v2f& GameObject::GetPosition() const
{
	return myTransform.myPosition;
}
const float& GameObject::GetPositionX() const
{
	return myTransform.myPosition.x;
}
const float& GameObject::GetPositionY() const
{
	return myTransform.myPosition.y;
}
const float& GameObject::GetRotation() const
{
	return myTransform.myRotation;
}
const v2f& GameObject::GetPivot() const
{
	return myTransform.myPivot;
}

GameObject& GameObject::SetPosition(const v2f& aPosition)
{
	myTransform.myPosition = aPosition;

	return *this;
}

GameObject& GameObject::SetPositionX(const float& aPosition)
{
	myTransform.myPosition.x = aPosition;

	return *this;
}
GameObject& GameObject::SetPositionY(const float& aPosition)
{
	myTransform.myPosition.y = aPosition;

	return *this;
}

GameObject& GameObject::SetRotation(const float& aRotation)
{
	myTransform.myRotation = aRotation;

	return *this;
}

GameObject& GameObject::SetPivot(const v2f& aPivot)
{
	myTransform.myPivot = aPivot;

	return *this;
}

void GameObject::SetZIndex(const int aZIndex)
{
	myZIndex = aZIndex;
	myScene->SortGameObjects();
}

const int GameObject::GetZIndex()
{
	return myZIndex;
}

const bool GameObject::GetShouldBeDestroyed()
{
	return myTransform.myShouldBeDestroyed;
}

GameObject& GameObject::Destroy()
{
	myTransform.myShouldBeDestroyed = true;

	return *this;
}

void GameObject::DeleteInactiveSpriteComponents()
{
	for (int i = 0; i < myComponents.size(); i++)
	{
		SpriteComponent* cast = dynamic_cast<SpriteComponent*>(myComponents[i]);
		if (cast != NULL)
		{
			if (cast->IsActive() == false)
			{
				delete myComponents[i];
				myComponents.erase(myComponents.begin() + i);
				break;
			}
		}
	}
}

void GameObject::DeleteComponents()
{
	for (int i = myComponents.size() - 1; i >= 0; i--)
	{
		myComponents.erase(myComponents.begin() + i);
	}
}

GameObject& GameObject::Activate()
{
	myIsActive = true;

	return *this;
}
GameObject& GameObject::Deactivate()
{
	myIsActive = false;

	return *this;
}

Scene* GameObject::GetScene()
{
	return myScene;
}
GameObject& GameObject::SetScene(Scene* aScene)
{
	myScene = aScene;

	return *this;
}