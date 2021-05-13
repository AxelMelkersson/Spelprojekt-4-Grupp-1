/*
* Author: Axel Melkersson
*/

#pragma once
#include "Component.hpp"
#include "Animation.hpp"

class SpriteComponent;

class AnimationComponent : public Component
{
public:
	AnimationComponent();
	AnimationComponent(SpriteComponent* aSpriteComponent, const int& aAnimationFrameCount, const int& aColumns, const float& aUpdateTime, const int aBoundsX, const int aBoundsY);

	void SetSprite(SpriteComponent* aSpriteComponent);

	void SetAnimation(SpriteComponent* aSpriteComponent, const int& aAnimationFrameCount, const int& aColumns, const float& aUpdateTime);
	void SetDisplayOnce(const bool aIsDisplayedOnce);
	void SetBoomerang(const bool aIsBoomerang);

	void Update(Transform& aTransform, GameObject& aGameObject) override;

	void SetAnimation(Animation* aAnimation);
	void SetAnimation(Animation* aAnimation, const unsigned int aFrame);

	void SetNextAnimation(Animation* aAnimation);

	void SetUpdateTime(const float anUpdateTime);

	void ContinueToNextAnimation();

	const int GetCurrentIndex() const;
	const float GetTimer() const;
	bool GetIsDisplayedOnce();
	bool GetHasBeenDisplayedOnce();
	bool GetHasEnded();

private:
	SpriteComponent* mySprite;
	Animation* myNextAnimation;

	int mySpriteIndex;
	int myAnimationFrameCount;
	int myColumns;

	uint32_t myBoundsX;
	uint32_t myBoundsY;

	float myTimer;
	float myUpdateTime;

	bool myIsBackwards;
	bool myDisplayOnce;
	bool myHasBeenDisplayed;

	bool myIsBoomerang;
	bool myHasReachedEnd;
};

