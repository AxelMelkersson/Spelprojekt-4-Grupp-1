#include "stdafx.h"
#include "EffectSprite.h"
#include "SpriteComponent.h"
#include "Random.hpp"
#include "../External/Headers/CU/Utilities.h"

EffectSprite::EffectSprite()
{
	mySprite = {};
}

EffectSprite::~EffectSprite()
{
	mySprite->Deactivate();
	mySprite = nullptr;
}

void EffectSprite::Update(const float& aDeltatime)
{
	myTotalTimer += aDeltatime;

	LerpSpeed(aDeltatime);
	LerpScale(aDeltatime);
	LerpColor(aDeltatime);

	MoveSprite(aDeltatime);

	CheckIfSpriteAlive();
}

void EffectSprite::AddSprite(SpriteComponent* aSprite)
{
	mySprite = aSprite;
	mySprite->SetSpritePath(myPathString);
	mySprite->SetCeilPosition(true);
	myCurrentColor = myStartColor;

	SetNewColor();
	
	mySize = mySprite->GetSize() * myMinScale;
	myMaxVectorScale = mySprite->GetSize() * myMaxScale;

	v2f position = {};

	float width = myEmitterWidth;
	position.x = Utils::RandomFloat(-width, width);

	float height = myEmitterHeigth;
	position.y = Utils::RandomFloat(0, -height);

	if (myIsLockedPos)
	{
		myDirection.x = Utils::RandomFloat(myEmitterAngular.x, myEmitterAngular.z);
		myDirection.y = Utils::RandomFloat(myEmitterAngular.y, myEmitterAngular.w);
	}

	position = mySprite->GetRelativePosition() + position + myOffset;
	
	mySprite->SetSize({0.f,0.f});
	mySprite->SetRelativePosition(position);
	mySprite->SetRelativeRotation(mySpriteRotation);
}

SpriteComponent* EffectSprite::GetSprite()
{
	return mySprite;
}

bool EffectSprite::IsAlive()
{
	return myIsAlive;
}

void EffectSprite::SetInactive()
{
	myIsAlive = false;
}

const void EffectSprite::MoveSprite(const float& aDeltaTime)
{
	v2f direction = { std::cosf(myEmitterAngle * myPI / 180.f), -std::sinf(myEmitterAngle * myPI / 180.f) };
	v2f position = {};

	if (!myIsLockedPos)
	{
		position.x = mySprite->GetRelativePositionX() + direction.x * mySpeedInterval * aDeltaTime;
		position.y = mySprite->GetRelativePositionY() + direction.y * mySpeedInterval * aDeltaTime;
	}
	else
	{
		float speedX = myDirection.x * mySpeedInterval * aDeltaTime;
		float speedY = myDirection.y * mySpeedInterval * aDeltaTime;

		position.x = mySprite->GetRelativePositionX() + speedX;
		position.y = mySprite->GetRelativePositionY() + speedY;
	}

	mySprite->SetRelativePosition(position);
	mySprite->SetRelativeRotation(mySprite->GetRelativeRotation() + myRotation * aDeltaTime);

	if (!myHasActivated)
	{
		myHasActivated = true;
		mySprite->SetSize(mySize);
		mySprite->Activate();
	}
}

const void EffectSprite::LerpSpeed(const float& aDeltatime)
{
	const float timer = myLifeTime - myTotalTimer;

	myMinSpeed = Utils::Lerp(myMinSpeed, myMaxSpeed, aDeltatime / timer);
	mySpeedInterval = myMinSpeed;
}

const void EffectSprite::LerpScale(const float& aDeltatime)
{
	const float timer = myLifeTime - myTotalTimer;

	myScale.x = Utils::Lerp(mySprite->GetSize().x, myMaxVectorScale.x, aDeltatime / timer);
	myScale.y = Utils::Lerp(mySprite->GetSize().y, myMaxVectorScale.y, aDeltatime / timer);

	mySprite->SetSize(myScale);
}

const void EffectSprite::LerpColor(const float& aDeltaTime)
{
	const float timer = myLifeTime - myTotalTimer;

	myCurrentColor.myR = Utils::Lerp(myCurrentColor.myR, myEndColor.myR, aDeltaTime / timer);
	myCurrentColor.myG = Utils::Lerp(myCurrentColor.myG, myEndColor.myG, aDeltaTime / timer);
	myCurrentColor.myB = Utils::Lerp(myCurrentColor.myB, myEndColor.myB, aDeltaTime / timer);
	myCurrentColor.myA = Utils::Lerp(myCurrentColor.myA, myEndColor.myA, aDeltaTime / timer);

	SetNewColor();
}

const void EffectSprite::SetNewColor()
{
	mySprite->SetColor({ myCurrentColor.myR, myCurrentColor.myG, myCurrentColor.myB, myCurrentColor.myA});
}

const void EffectSprite::CheckIfSpriteAlive()
{
	if (myTotalTimer > myLifeTime)
	{
		myIsAlive = false;
	}
}
