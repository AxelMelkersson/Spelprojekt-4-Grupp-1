#pragma once
#include "GameObject.h"
class Scene;

class LevelDoor : public GameObject
{
public:
	enum class eDoorType
	{
		Entry, 
		Exit,
		HiddenRoom,
		MainRoom
	};

	LevelDoor(Scene* aScene);
	~LevelDoor();

	void Init(const eDoorType aDoorType, const v2f& aSize);

	void OnCollision(GameObject* aGameObject) override;

private:
	eDoorType myType;

	bool myWasActivated;
};

