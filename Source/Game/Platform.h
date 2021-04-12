#pragma once
#include "../External/Headers/CU/Vector2.hpp"
#include "GameObject.h"
#include <vector>

class Scene;

class Platform : public GameObject
{
public:
	enum class ePlatformType
	{
		Static,
		Moving,
		Unstable,
		Destructible
	};

	Platform(Scene* aScene);
	~Platform();

	virtual void Init(const v2f& aSize, const v2f& aSpriteSize, const v2f& aPosition, const bool& aIsOneway);
	virtual void Init(const v2f& aSize, const v2f& aSpriteSize, const v2f& aPosition, const bool& aIsOneway, const int& aMaterialIndex); // Temp

	virtual void Update(const float& aDeltaTime) override;

	virtual void OnCollision(GameObject* aGameObject) override;
	void Landed(const int& aOverlapY) override;

	void ResetVelocity();
protected:
	int myMaterial = {};
private:
};

