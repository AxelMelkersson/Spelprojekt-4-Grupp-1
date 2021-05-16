/*
*	Author: Elia Rönning
*/

#include "stdafx.h"
#include "Scene.h"
#include "Game.h"
#include "GameObject.h"
#include "../External/Headers/CU/Utilities.h"

#include <algorithm>

#include "TileSetLayer.hpp"

std::vector<Scene*> Scene::ourScenes = std::vector<Scene*>();

Scene::Scene()
	: myIsActive(false)
	, myIsLoaded(false)
	, myCamera(Camera())
	, myPhysics(PhysicsManager())
{
	ourScenes.push_back(this);
}
Scene::~Scene()
{
	for (size_t index = 0U; index < ourScenes.size(); ++index)
	{
		if (ourScenes[index] == this)
		{
			ourScenes.erase(ourScenes.begin() + index);
			break;
		}
	}
}

void Scene::Load()
{
	myIsLoaded = true;

	for (GameObject*& obj : myGameObjects)
	{
		obj->Init();
	}
}
void Scene::Unload()
{
	if (!myIsActive) Deactivate();

	myIsLoaded = false;
	myIsActive = false;

	for (GameObject*& obj : myGameObjects)
	{
		obj->Destroy();
	}
}

void Scene::Activate()
{
	if (!myIsLoaded) Load();

	myIsActive = true;

	for (GameObject*& obj : myGameObjects)
	{
		obj->Activate();
	}
}
void Scene::Deactivate()
{
	myIsActive = false;

	for (GameObject*& obj : myGameObjects)
	{
		obj->Deactivate();
	}
}

void Scene::Update(const float& aDeltaTime)
{
	for (size_t index = 0U; index < myGameObjects.size(); ++index)
	{
		GameObject*& object = myGameObjects[index];

		if (object->GetTransform().myShouldBeDestroyed)
		{
 			delete object;
			--index;
			continue;
		}

		object->Update(aDeltaTime);
	}

	myPhysics.PhysicsUpdate(aDeltaTime, myGameObjects);
	myCamera.Update(aDeltaTime);
}

void Scene::Render()
{
	for (GameObject*& obj : myGameObjects)
	{
		if (obj->IsActive())
		{
			obj->Render();
		}
	}
}

void Scene::Manager::Update(const float& aDeltaTime)
{
	for (Scene*& scene : ourScenes)
	{
		if (scene->IsActive())
		{
			scene->Update(aDeltaTime);
		}
	}
}

void Scene::SortGameObjects()
{
	std::sort(myGameObjects.begin(), myGameObjects.end(), [](GameObject* object, GameObject* comparedObject)
	{
		return object->GetZIndex() < comparedObject->GetZIndex();
	});
}

void Scene::Manager::Render()
{
	for (Scene*& scene : ourScenes)
	{
		if (scene->IsActive())
		{
			scene->Render();
		}
	}
}