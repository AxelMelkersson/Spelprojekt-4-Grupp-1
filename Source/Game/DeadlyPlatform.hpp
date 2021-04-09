#pragma once
#include "Platform.h"

class DeadlyPlatform : public Platform
{
public:
	DeadlyPlatform(Scene* aLevelScene);

	void Init(const v2f& aSize, const v2f& aSpriteSize, const v2f& aPosition, const bool& aIsOneway) override;
	void Init(const v2f& aSize, const v2f& aSpriteSize, const v2f& aPosition, const bool& aIsOneway, const int& aMaterial) override;


	void OnCollision(GameObject* aGameObject) override;

private:

};

