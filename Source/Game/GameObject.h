/*
*	Author: Elia Rönning
*/

#pragma once
#include "Transform.hpp"
#include <vector>
#include "Component.hpp"

class CGameWorld;
class Scene;

class BashComponent;

class GameObject
{
public:
	/* Constructors & Destructor */
	GameObject(Scene* aScene = nullptr);
	GameObject(const GameObject& aGameObject) = default;
	virtual ~GameObject();

	/* Virtual */
	virtual void Init();
	virtual void Update(const float& aDeltaTime);
	virtual void Render();

	virtual void OnCollision(GameObject* anObject) { anObject;  }

	/* Transform */
	const Transform& GetTransform() const;
	const v2f& GetPosition() const;
	const float& GetPositionX() const;
	const float& GetPositionY() const;
	const float& GetRotation() const;
	const v2f& GetPivot() const;

	GameObject& SetPosition(const v2f& aPosition);
	GameObject& SetPositionX(const float& aPosition);
	GameObject& SetPositionY(const float& aPosition);
	GameObject& SetRotation(const float& aRotation);
	GameObject& SetPivot(const v2f& aPivot);

	void SetZIndex(const int aZIndex);
	const int GetZIndex();

	virtual void Landed(const int& aOverlapY) { aOverlapY; }
	virtual void SideCollision(const int& aOverlapX) { aOverlapX; }
	virtual void BashCollision(GameObject* aGameObject, BashComponent* aBashComponent) { aGameObject; aBashComponent; }
	virtual void OnStartBashed() {}
	virtual void OnBashed() {}

	virtual GameObject& Destroy();
	void DeleteInactiveSpriteComponents();
	void DeleteComponents();

	/* Other Setters & Getters */
	inline const bool& IsActive() const
	{
		return myIsActive;
	}
	GameObject& Activate();
	GameObject& Deactivate();

	Scene* GetScene();
	GameObject& SetScene(Scene* aScene);

	/* Components */
	template<class T>
	T* AddComponent()
	{
		T* comp = new T();
		myComponents.push_back(comp);
		return comp;
	}
	template<class T>
	T* GetComponent()
	{
		for (Component*& comp : myComponents)
		{
			T* ptr = dynamic_cast<T*>(comp);
			if (ptr)
			{
				return ptr;
			}
		}

		return nullptr;
	}

protected:
	friend class ParticleEffect;

	Transform myTransform;
	std::vector<Component*> myComponents;

	Scene* myScene;
	
	int myZIndex;

	bool myIsActive; 
};