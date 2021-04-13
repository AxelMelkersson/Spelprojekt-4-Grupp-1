/*
*	Author: Elia Rönning
*/

#include "stdafx.h"
#include "Camera.h"
#include "Game.h"
#include "GameObject.h"

#include "../External/Headers/CU/Utilities.h"
#include <random>

Camera::Camera()
	: myX(0.0f)
	, myY(0.0f)
	, myAlpha(1.0f)

	, myIsFollowing(false)
	, myTarget(nullptr)
	, myOffset({ 0.0f, 0.0f })
	, myLerp({ 1.0f, 1.0f })
	, myUseStaticTarget(false)
	, myStaticTarget({ 0.0f, 0.0f })

	, myHasBounds(false)
	, myBounds({ 0.0f, 0.0f })
	, myBoundSize({ 0.0f, 0.0f })
	, myWorldViewSize({ 320.0f, 180.0f })

	, myIsShaking(false)
	, myShakeDuration(-1.0f)
	, myShakeIntensity({ 0.0f, 0.0f })
	, myShakeDropoff(1.0f)
	, myCurrentShakeOffset({ 0.0f, 0.0f })

	, myZoom(1.0f)
	, myInverseZoom(1.0f)
	, myCameraActive(false)
{}

void Camera::Update(const float& aDeltaTime)
{
	if (myIsFollowing)
	{
		const v2f& targetPos = GetTargetPosition();

			SetPosition
			({
				myLesserThanViewPortX * Utils::Lerp<float>(myX, targetPos.x - myWorldViewSize.x * 0.5f, myLerp.x * aDeltaTime),
				myLesserThanViewPortY * Utils::Lerp<float>(myY, targetPos.y - myWorldViewSize.y * 0.5f, myLerp.y * aDeltaTime)
			});

		SetActive();
	}

	if (myIsShaking)
	{
		ShakeUpdate(aDeltaTime);
	}
}

/* Getters */
const v2f Camera::GetPosition() const
{
	return { myX, myY };
}
const float& Camera::GetPositionX() const
{
	return myX;
}
const float& Camera::GetPositionY() const
{
	return myY;
}
const float& Camera::GetAlpha() const
{
	return myAlpha;
}

/* Setters */
Camera& Camera::SetPosition(const v2f & aPosition)
{
	SetPositionX(aPosition.x);
	SetPositionY(aPosition.y);

	return *this;
}
Camera& Camera::SetPositionX(const float& anX)
{
	myX = anX;

	if (myHasBounds)
	{
		if (myX < myBounds.x)
		{
			myX = myBounds.x;
		}
		else if (myX > myBounds.x + myBoundSize.x - myWorldViewSize.x)
		{
			myX = myBounds.x + myBoundSize.x - myWorldViewSize.x;
		}
	}

	return *this;
}
Camera& Camera::SetPositionY(const float& aY)
{
	myY = aY;

	if (myHasBounds)
	{
		if (myY < myBounds.y)
		{
			myY = myBounds.y;
		}
		else if (myY > myBounds.y + myBoundSize.y - myWorldViewSize.y)
		{
			myY = myBounds.y + myBoundSize.y - myWorldViewSize.y;
		}
	}

	return *this;
}
Camera& Camera::SetAlpha(const float& anAlpha)
{
	myAlpha = anAlpha;

	return *this;
}

/* Camera Follow */
/* Getters */
const bool& Camera::IsFollowing() const
{
	return myIsFollowing;
}
GameObject*& Camera::GetTarget()
{
	return myTarget;
}
const v2f& Camera::GetOffset() const
{
	return myOffset;
}
const v2f& Camera::GetLerp() const
{
	return myLerp;
}

const v2f& Camera::GetTargetPosition() const
{
	return myUseStaticTarget ? myStaticTarget : myTarget->GetPosition();
}

/* Setters */
Camera& Camera::SetTarget(GameObject*& aTarget)
{
	myTarget = aTarget;

	return *this;
}
Camera& Camera::SetOffset(const v2f& anOffset)
{
	myOffset = anOffset;

	return *this;
}
Camera& Camera::SetOffsetX(const float& anX)
{
	myOffset.x = anX;

	return *this;
}
Camera& Camera::SetOffsetY(const float& aY)
{
	myOffset.y = aY;

	return *this;
}
Camera& Camera::SetLerp(const v2f& aLerp)
{
	myLerp = aLerp;

	return *this;
}
Camera& Camera::SetLerpX(const float& anX)
{
	myLerp.x = anX;

	return *this;
}
Camera& Camera::SetLerpY(const float& aY)
{
	myLerp.y = aY;

	return *this;
}



Camera& Camera::StartFollowing(GameObject* aGameObject, const v2f& aLerp, const v2f& anOffset)
{
	myUseStaticTarget = false;
	myTarget = aGameObject;
	myLerp = aLerp;
	myOffset = anOffset;
	myIsFollowing = true;

	return *this;
}
Camera& Camera::StartFollowing(const v2f& aStaticTarget, const v2f& aLerp, const v2f& anOffset)
{
	myUseStaticTarget = true;
	myStaticTarget = aStaticTarget;
	myLerp = aLerp;
	myOffset = anOffset;
	myIsFollowing = true;

	return *this;
}
Camera& Camera::ResumeFollowing()
{
	if (myTarget != nullptr)
	{
		myIsFollowing = true;
	}

	return *this;
}
Camera& Camera::StopFollowing()
{
	myIsFollowing = false;

	return *this;
}

/* Camera Bounds */
Camera& Camera::SetBounds(const v2f& aCoordinate, const v2f& aSize)
{
	myHasBounds = true;
	myBounds = aCoordinate;
	myBoundSize = aSize;

	myLesserThanViewPortX = true;
	myLesserThanViewPortY = true;

	constexpr float viewPortSizeX = 40.0f * 8.0f; //40 tiles
	constexpr float viewPortSizeY = 23.0f * 8.0f; //23 tiles

	if (aSize.x <= viewPortSizeX)
	{
		myLesserThanViewPortX = false;
	}
	if (aSize.y <= viewPortSizeY)
	{
		myLesserThanViewPortY = false;
	}

	return *this;
}

Camera& Camera::SetBounds(const float& anX, const float& aY, const float& aWidth, const float& aHeight)
{
	return SetBounds({ anX, aY }, { aWidth, aHeight });
}

Camera& Camera::ResetBounds()
{
	myHasBounds = false;

	return *this;
}

const bool& Camera::HasBounds() const
{
	return myHasBounds;
}

const v2f& Camera::GetBounds() const
{
	return myBounds;
}

const v2f& Camera::GetBoundSize() const
{
	return myBoundSize;
}

const bool Camera::GetActive() const
{
	return myCameraActive;
}

void Camera::SetActive()
{
	if (!myCameraActive)
		myCameraActive = true;
}

/* Camera Shake */
bool Camera::IsShaking() const
{
	return myIsShaking;
}
Camera& Camera::ShakeUpdate(const float& aDeltaTime)
{
	SetPosition(GetPosition() - myCurrentShakeOffset);
	if (myShakeDuration < 0.0f)
	{
		myIsShaking = false;
		myCurrentShakeOffset = { 0.0f, 0.0f };

		return *this;
	}

	myShakeDuration -= aDeltaTime;

	myShakeIntensity *= myShakeDropoff;

	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	std::random_device randomDevice;

	myCurrentShakeOffset = { myLesserThanViewPortX * distribution(randomDevice) * myShakeIntensity.x, myLesserThanViewPortY * distribution(randomDevice) * myShakeIntensity.y};

	SetPosition(GetPosition() + myCurrentShakeOffset);

	return *this;
}

Camera& Camera::Shake(const float& aDuration, const float& anIntensity, const float& aDropOff)
{
	return Shake(aDuration, { anIntensity, anIntensity }, aDropOff);
}
Camera& Camera::Shake(const float& aDuration, const v2f& anIntensity, const float& aDropOff)
{
	myIsShaking = true;
	myShakeDuration = aDuration;
	myShakeIntensity = anIntensity;
	myShakeDropoff = aDropOff;

	return *this;
}

/* Camera Zoom */
const float& Camera::GetZoom() const
{
	return myZoom;
}
v2f Camera::GetViewportSize() const
{
	return { static_cast<float>(Config::width) * myZoom, static_cast<float>(Config::height) * myZoom };
}
Camera& Camera::SetZoom(const float& aZoom)
{
	myZoom = aZoom;
	myInverseZoom = 1.0f / aZoom;

	return *this;
}