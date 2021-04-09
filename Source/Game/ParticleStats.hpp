#pragma once
#include "..\tga2dcore\tga2d\math\color.h"
#include "..\External\Headers\CU\Vector4.hpp"
#include <string>

namespace Tga2D
{
	class CColor;
}

class ParticleStats
{
private:
	friend class ParticleEffect;
	friend class ParticleEffectFactory;

	std::string mySpritePath = {};
	int myEffectTypeIndex = {};
	int myZIndex = {};

	bool myLockedPosition = {};
	
	float myPauseTimer = {};
	float myPauseSeconds = {};

	float myStartScale = {};
	float myEndScale = {};

	float myMinBetweenSpawn = {};
	float myMaxBetweenSpawn = {};

	float myMinSpeed = {};
	float myMaxSpeed = {};

	float myMinParticleAngularVel = {};
	float myMaxParticleAngularVel = {};

	float myEmitterAngle = {};
	float myEmitterWidth = {};
	float myEmitterHeigth = {};

	float myMinParticleLifeTime = {};
	float myMaxParticleLifeTime = {};
	float myEmitterLifeTime = {};

	float mySpriteRotation = {};

	v4f myEmitterAngular = {};
	v2f myOffset = {};

	Tga2D::CColor myStartColor = {};
	Tga2D::CColor myEndColor = {};

};