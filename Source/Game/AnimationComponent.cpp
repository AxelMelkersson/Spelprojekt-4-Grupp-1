/*
* Author: Axel Melkersson
*/

#include "stdafx.h"
#include "AnimationComponent.hpp"
#include "GameObject.h"
#include "GameWorld.h"
#include "SpriteComponent.h"
#include "../External/Headers/CU/Utilities.h"

AnimationComponent::AnimationComponent()
	: myTimer(0.0f),
	myDisplayOnce(false),
	myHasBeenDisplayed(false),
	myHasReachedEnd(false),
	myIsBackwards(false),
	myIsBoomerang(false),
	myNextAnimation(nullptr),
	myUpdateTime(0.0f),
	myAnimationFrameCount(0),
	myColumns(0),
	mySpriteIndex(0),
	mySprite(nullptr),
	myBoundsX(0),
	myBoundsY(0)
{}
AnimationComponent::AnimationComponent(SpriteComponent* aSpriteComponent, const int& aAnimationFrameCount, const int& aColumns, const float& aUpdateTime, const int aBoundsX, const int aBoundsY)
{
	myIsBackwards = false;
	myHasReachedEnd = false;
	myIsBoomerang = false;

	myDisplayOnce = false;
	myHasBeenDisplayed = false;

	myNextAnimation = nullptr;

	myBoundsX = aBoundsX;
	myBoundsY = aBoundsY;

	mySprite = aSpriteComponent;

	mySpriteIndex = 0;

	myTimer = aUpdateTime;
	myUpdateTime = aUpdateTime;

	myAnimationFrameCount = aAnimationFrameCount;
	myColumns = aColumns;

	uint32_t anX = 0;
	uint32_t aY = 0;
	mySprite->SetSpriteRectPixel(anX, aY, myBoundsX, myBoundsY);
}

void AnimationComponent::Update(Transform& aTransform, GameObject& aGameObject)
{
	aTransform;
	aGameObject;
	aTransform;

	if (myHasBeenDisplayed && myDisplayOnce)
	{
		return;
	}

	if (myAnimationFrameCount == 0)
	{
		return;
	}

	myTimer -= CGameWorld::GetInstance()->GetTimer()->GetDeltaTime();

	if (myTimer < 0)
	{
		if (myIsBoomerang && myHasReachedEnd)
		{
			--mySpriteIndex;
			if (mySpriteIndex <= 0)
			{
				myHasReachedEnd = false;
			}
		}
		else
		{
			if (myIsBackwards)
			{
				--mySpriteIndex;
			}
			else
			{
				++mySpriteIndex;
			}
		}

		if ((mySpriteIndex >= myAnimationFrameCount && !myIsBackwards) || (mySpriteIndex < 0 && myIsBackwards) || (myIsBoomerang && (mySpriteIndex >= myAnimationFrameCount || mySpriteIndex < 0)))
		{
			
			myHasBeenDisplayed = true;

			if (myIsBackwards)
			{
				mySpriteIndex = myAnimationFrameCount - 1;
			}
			else if (myIsBoomerang)
			{
				--mySpriteIndex;
			}
			else
			{
				mySpriteIndex = 0;
			}

			myHasReachedEnd = true;

			if (myDisplayOnce)
			{
				if (myNextAnimation)
				{
					SetAnimation(myNextAnimation);
				}
				return;
			}
		}

		const uint32_t xOffset = static_cast<uint32_t>(mySpriteIndex % myColumns) * myBoundsX;
		const uint32_t yOffset = static_cast<uint32_t>(mySpriteIndex / myColumns) * myBoundsY;

		const v2f imageSize = mySprite->GetImageSize();

		const uint32_t xTexel = static_cast<uint32_t>(1.0f / imageSize.x);
		const uint32_t yTexel = static_cast<uint32_t>(1.0f / imageSize.y);

		myTimer = myUpdateTime;
		mySprite->SetSpriteRectPixel(xTexel + xOffset, yTexel + yOffset, xOffset + myBoundsX - xTexel, yOffset + myBoundsY - yTexel);
	}
}

void AnimationComponent::SetSprite(SpriteComponent* aSpriteComponent)
{
	mySprite = aSpriteComponent;
}
void AnimationComponent::SetAnimation(Animation* aAnimation)
{
	mySprite->Deactivate();
	mySprite = aAnimation->mySpriteComponent;
	mySprite->Activate();
	
	myBoundsX = aAnimation->myBoundsX;
	myBoundsY = aAnimation->myBoundsY;
	
	SetAnimation(mySprite, aAnimation->myAnimationFrameCount, aAnimation->myColumns, aAnimation->myUpdateTime);
	
	mySpriteIndex = aAnimation->mySpriteIndex;

	myIsBoomerang = aAnimation->myIsBoomerang;
	myIsBackwards = aAnimation->myIsBackwards;
	myDisplayOnce = aAnimation->myDisplayOnce;

	const uint32_t xOffset = static_cast<uint32_t>(mySpriteIndex % myColumns) * myBoundsX;
	const uint32_t yOffset = static_cast<uint32_t>(mySpriteIndex / myColumns) * myBoundsY;

	mySprite->SetSpriteRectPixel(xOffset, yOffset, xOffset + myBoundsX, yOffset + myBoundsY);
}
void AnimationComponent::SetAnimation(Animation* aAnimation, const unsigned int aFrame)
{
	mySprite->Deactivate();
	mySprite = aAnimation->mySpriteComponent;
	mySprite->Activate();

	myBoundsX = aAnimation->myBoundsX;
	myBoundsY = aAnimation->myBoundsY;

	SetAnimation(mySprite, aAnimation->myAnimationFrameCount, aAnimation->myColumns, aAnimation->myUpdateTime);

	mySpriteIndex = aFrame;

	myIsBoomerang = aAnimation->myIsBoomerang;
	myIsBackwards = aAnimation->myIsBackwards;
	myDisplayOnce = aAnimation->myDisplayOnce;

	const uint32_t xOffset = static_cast<uint32_t>(mySpriteIndex % myColumns) * myBoundsX;
	const uint32_t yOffset = static_cast<uint32_t>(mySpriteIndex / myColumns) * myBoundsY;

	mySprite->SetSpriteRectPixel(xOffset, yOffset, xOffset + myBoundsX, yOffset + myBoundsY);
}

void AnimationComponent::SetAnimation(SpriteComponent* aSpriteComponent, const int& aAnimationFrameCount, const int& aColumns, const float& aUpdateTime)
{
	myIsBackwards = false;
	myHasReachedEnd = false;
	myIsBoomerang = false;

	myDisplayOnce = false;
	myHasBeenDisplayed = false;

	myNextAnimation = nullptr;

	mySprite = aSpriteComponent;
	mySprite->Activate();

	mySpriteIndex = 0;

	myTimer = aUpdateTime;
	myUpdateTime = aUpdateTime;

	myAnimationFrameCount = aAnimationFrameCount;
	myColumns = aColumns;

	uint32_t anX = 0;
	uint32_t aY = 0;
	mySprite->SetSpriteRectPixel(anX, aY, myBoundsX, myBoundsY);
}
void AnimationComponent::SetNextAnimation(Animation* aAnimation)
{
	myNextAnimation = aAnimation;
}
void AnimationComponent::SetDisplayOnce(const bool aIsDisplayedOnce)
{
	myDisplayOnce = aIsDisplayedOnce;
}
void AnimationComponent::SetBoomerang(const bool aIsBoomerang)
{
	myIsBoomerang = aIsBoomerang;
}
void AnimationComponent::SetUpdateTime(const float anUpdateTime)
{
	myUpdateTime = anUpdateTime;
}
void AnimationComponent::ContinueToNextAnimation()
{
	if (myNextAnimation != nullptr)
	{
		SetAnimation(myNextAnimation);
	}
}

bool AnimationComponent::GetIsDisplayedOnce()
{
	return myDisplayOnce;
}
bool AnimationComponent::GetHasBeenDisplayedOnce()
{
	return myHasBeenDisplayed;
}
const int AnimationComponent::GetCurrentIndex() const
{
	return mySpriteIndex;
}
const float AnimationComponent::GetTimer() const
{
	return myTimer;
}
bool AnimationComponent::GetHasEnded()
{
	return myHasReachedEnd;
}
