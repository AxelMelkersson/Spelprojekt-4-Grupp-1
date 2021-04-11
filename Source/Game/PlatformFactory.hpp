#pragma once
#include "../External/Headers/CU/Vector2.hpp"
#include <vector>

class Platform;
class MovingPlatform;
class UnstablePlatform;
class DestructiblePlatform;
class DeadlyPlatform;

class Scene;

static class PlatformFactory
{
public:

	Platform* CreateStaticPlatform(Scene* aLevelScene, const v2f& aPosition, const v2f& aCollisionSize, const v2f& aSpriteSize, const bool aIsOneWay, const int aMaterial);
	MovingPlatform* CreateMovingPlatform(Scene* aLevelScene, const v2f& aPosition, const v2f& aCollisionSize, const v2f& aSpriteSize, const std::vector<v2f>& aWayPoints, const float& aSpeed);
	UnstablePlatform* CreateUnstablePlatform(Scene* aLevelScene, const v2f& aPosition, const v2f& aCollisionSize, const v2f& aSpriteSize, const float& aDestroyTime, const float& aRespawnTime);
	DestructiblePlatform* CreateDestructiblePlatform(Scene* aLevelScene, const v2f& aPosition, const v2f& aCollisionSize, const v2f& aSpriteSize);
	DeadlyPlatform* CreateDeadlyPlatform(Scene* aLevelScene, const v2f& aPosition, const v2f& aCollisionSize, const v2f& aSpriteSize, const int& aMaterial);
	

private:


};

