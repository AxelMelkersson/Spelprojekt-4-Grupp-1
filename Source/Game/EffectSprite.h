#pragma once
#include "../External/Headers/CU/Vector2.hpp"
#include "../External/Headers/CU/Vector4.hpp"

class SpriteComponent;

class EffectSprite
{
public:
	EffectSprite();
	~EffectSprite();

	void Update(const float& aDeltatime);

	void AddSprite(SpriteComponent* aSprite);
	SpriteComponent* GetSprite();

	bool IsAlive();

	void SetInactive();


	SpriteComponent* mySprite;

	std::string myPathString = {};
	v2f myDirection = {};
	v2f myScale = {};
	v2f myMaxVectorScale = {};
	v2f myOffset = {};
	v2f mySize = {};

	v4f myEmitterAngular = {};

	const float myPI = 3.14159265359f;

	float myTotalTimer = {};

	float myPauseTime = {};
	float myMinScale = {};
	float myMaxScale = {};
	float myMinSpeed = {};
	float myMaxSpeed = {};
	float mySpeedInterval = {};
	float myLifeTime = {};
	float myRotation = {};
	float mySpriteRotation = {};
	float myEmitterWidth = {};
	float myEmitterHeigth = {};
	float mySpawnAngle = {};
	float myEmitterAngle = {};

	float myEmitterStartAngle = {};
	float myEmitterEndAngle = {};

	bool myHasActivated = {};
	bool myIsAlive = true;
	bool myIsLockedPos = false;

	Tga2D::CColor myCurrentColor = {};

	Tga2D::CColor myStartColor = {};
	Tga2D::CColor myEndColor = {};

private:

	const void MoveSprite(const float& aDeltaTime);
	const void LerpSpeed(const float& aDeltatime);
	const void LerpScale(const float& aDeltatime);
	const void LerpColor(const float& aDeltatime);

	const void SetNewColor();
	const void CheckIfSpriteAlive();

};