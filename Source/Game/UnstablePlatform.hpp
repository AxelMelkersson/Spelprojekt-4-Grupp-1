#pragma once
#include "Platform.h"
#include "Animation.hpp"

class SpriteComponent;

class UnstablePlatform : public Platform
{
public:
	UnstablePlatform(Scene* aLevelScene);

	void Update(const float& aDeltaTime) override;

	void SetTimerProperties(const float& aDestroyTime, const float& aRespawnTime);

	void OnCollision(GameObject* aGameObject) override;

	void Landed(const int& aOverlapY) override;

	void SetSpriteToDisable(SpriteComponent* aSprite);

	void ActivatePlatform();
	void DeactivatePlatform();

	bool GetDeactivatedPlatform();

private:
	SpriteComponent* mySpriteToDisable;
	AnimationComponent* myAnimationComponent;

	Animation myAnimations[2];

	float myDestroyTime;
	float myRespawnTime;
	float myTimer;

	bool myActivatedIdleParticle;
	bool myCollidedWithPlayer;
	bool myCollidedLastFrame;
	bool myIsDeactivated;

	bool myPlayerIsOnTop;

};

