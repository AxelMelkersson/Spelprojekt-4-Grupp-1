/*
*	Author: Elia Rönning
*/

#pragma once
#include "Camera.h"
#include "PhysicsManager.h"
#include <vector>

class Scene
{
public:
	Scene();
	Scene(const Scene& aScene) = default;
	virtual ~Scene();

	virtual void Load();
	virtual void Unload();

	virtual void Activate();
	virtual void Deactivate();

	virtual void Update(const float& aDeltaTime);
	virtual void Render();

	void SortGameObjects();

	struct Manager
	{
		static void Update(const float& aDeltaTime);
		static void Render();
	};

	inline Scene& PushGameObject(GameObject* anObject)
	{
		myGameObjects.push_back(anObject);
		SortGameObjects();
		return *this;
	}

	inline bool RemoveGameObject(GameObject* anObject)
	{
		for (size_t index = 0U; index < myGameObjects.size(); ++index)
		{
			if (myGameObjects[index] == anObject)
			{
				myGameObjects.erase(myGameObjects.begin() + index);
				return true;
			}
		}

		return false;
	}

	inline const bool& IsActive() const
	{
		return myIsActive;
	}
	inline const bool& IsLoaded() const
	{
		return myIsLoaded;
	}

	inline Camera& GetCamera()
	{
		return myCamera;
	}
	inline PhysicsManager& GetPhysics()
	{
		return myPhysics;
	}

private:
	static std::vector<Scene*> ourScenes;

	Camera myCamera;
	PhysicsManager myPhysics;

	std::vector<GameObject*> myGameObjects;

	bool myIsActive;
	bool myIsLoaded;

};
