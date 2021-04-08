#include "stdafx.h"
#include <iostream>
#include "Scene.h"
#include <tga2d/sprite/sprite.h>
#include <tga2d/sprite/sprite_batch.h>
#include "ParticleEffect.h"
#include "Scene.h"
#include "Player.hpp"
#include "PhysicsComponent.h"
#include "Random.hpp"
#include "SpriteComponent.h"
#include "SpritebatchComponent.h"

ParticleEffect::ParticleEffect(Scene* aLevelScene)
	:
	GameObject(aLevelScene),
	myBatch(nullptr)
{ 
	myScene = aLevelScene;
	mySpawningInLocalSpace = {};
	myFollowObject = nullptr;
	myEffectIsDestroyed = {};
	myKilledEffect = {};
	myObjectIsFollowing = {};
	mySpawnInterval = {};
	myActiveEffect = {};
	myTimer = {};
	myLifeTime = {};
	myCreatingSprites = {};
	myAddedEmitter = {};
	myPauseTimer = {};
	myAddedPauseTimer = {};
	myZIndex = {};
	myInitBatching = {};
	mySetZIndex = {};
}

ParticleEffect::~ParticleEffect()
{
	if (!myEffectIsDestroyed)
		DeleteSprites();

	GameObject::~GameObject();
}

void ParticleEffect::Init(ParticleStats aStats)
{
	myStats = aStats;
	myCreatingSprites = true;
	myBatch = AddComponent<SpritebatchComponent>();
	myBatch->SetSpritePath(myStats.mySpritePath);
	myBatch->SetSamplerFilter(ESamplerFilter_Point);
	myBatch->Init();

	SetPosition(GetPosition());
	SetPivot({ 0.5f, 0.5f });
	Activate();

	if (myStats.myEffectTypeIndex == static_cast<int>(eParticleEffects::RainEffectBackgroundParticle) || myStats.myEffectTypeIndex == static_cast<int>(eParticleEffects::RainEffectForegroundParticle))
	{

		v2f bounds = myScene->GetCamera().GetBoundSize();

		myStats.myEmitterWidth = myStats.myEmitterWidth + bounds.x * 1.5f;
		myStats.myOffset = { myStats.myOffset.x, myStats.myOffset.y + -bounds.y };
	}
}

void ParticleEffect::Render()
{
	for (auto sprite : mySprites)
		sprite->GetSprite()->Render(myTransform, *this);

	myBatch->Render(myTransform, *this);
}

void ParticleEffect::Update(const float& aDeltaTime)
{
	if (!mySetZIndex)
	{
		SetZIndex(myStats.myZIndex);
		mySetZIndex = true;
	}

	if (myActiveEffect)
	{
		UpdateParticle(aDeltaTime);
	}
}

const void ParticleEffect::SetIsActive(const bool aActiveState)
{
	myActiveEffect = aActiveState;
}

const bool ParticleEffect::GetIsActive()
{
	return myActiveEffect;
}

const eParticleEffects ParticleEffect::GetType() const
{
	return static_cast<eParticleEffects>(myStats.myEffectTypeIndex);
}

const void ParticleEffect::UpdateParticle(const float& aDeltaTime)
{
	myTimer += aDeltaTime;
	myLifeTime += aDeltaTime;
	myPauseTimer += aDeltaTime;

	CheckActiveStats();

	if (!myKilledEffect)
		CheckWhenToSpawnSprites();

	CheckIfSpritesAreDead(aDeltaTime);
	CheckIfEffectIsDead();
}

const void ParticleEffect::CheckWhenToSpawnSprites()
{
	if (myTimer > mySpawnInterval && myCreatingSprites)
	{
		if (myAddedPauseTimer)
		{
			if (myPauseTimer <= myStats.myPauseTimer)
			{
				SpawnSprite();
			}
			else
			{
				if (myPauseTimer >= myStats.myPauseTimer + myStats.myPauseSeconds)
					myPauseTimer = {};
			}
		}
		else
		{
			SpawnSprite();
		}
	}
}

const void ParticleEffect::SpawnSprite()
{
	myTimer = {};

	EffectSprite* sprite = new EffectSprite();

	sprite->myPathString = myStats.mySpritePath;
	sprite->myMinSpeed = myStats.myMinSpeed;
	sprite->myMaxSpeed = myStats.myMaxSpeed;
	sprite->myPauseTime = myStats.myPauseTimer;
	sprite->myLifeTime = Utils::RandomFloat(myStats.myMinParticleLifeTime, myStats.myMaxParticleLifeTime);
	sprite->myRotation = Utils::RandomFloat(myStats.myMinParticleAngularVel, myStats.myMaxParticleAngularVel);
	sprite->myMinScale = myStats.myStartScale;
	sprite->myMaxScale = myStats.myEndScale;
	sprite->myEmitterAngle = myStats.myEmitterAngle;
	sprite->myEmitterWidth = myStats.myEmitterWidth;
	sprite->myEmitterHeigth = myStats.myEmitterHeigth;
	sprite->myStartColor = myStats.myStartColor;
	sprite->myEndColor = myStats.myEndColor;
	sprite->myIsLockedPos = myStats.myLockedPosition;
	sprite->myEmitterAngular = myStats.myEmitterAngular;
	sprite->myOffset = myStats.myOffset;
	sprite->mySpriteRotation = myStats.mySpriteRotation;

	sprite->AddSprite(AddComponent<SpriteComponent>());
	myBatch->AddSprite(sprite->GetSprite());

	mySprites.push_back(sprite);

	mySpawnInterval = Utils::RandomFloat(myStats.myMinBetweenSpawn, myStats.myMaxBetweenSpawn);
}

const void ParticleEffect::CheckIfEffectIsDead()
{
	if (myAddedEmitter)
	{
		if (myLifeTime > myStats.myEmitterLifeTime)
			myCreatingSprites = false;
	}

	if (myCreatingSprites == false || myKilledEffect)
	{
		bool spritesAreMoving = false;

		for (int x = 0; x < mySprites.size(); x++)
		{
			if (mySprites[x]->IsAlive())
			{
				spritesAreMoving = true;
				break;
			}
		}

		if (!spritesAreMoving)
		{
			delete myBatch;
			myBatch = nullptr;
			DeleteComponents();
			mySprites.clear();
			myEffectIsDestroyed = true;
			delete this;
		}
	}
}

const void ParticleEffect::CheckIfSpritesAreDead(const float& aDeltaTime)
{
	for (int x = mySprites.size() - 1; x >= 0; x--)
	{
		mySprites[x]->Update(aDeltaTime);

		if (mySprites[x]->IsAlive() == false)
		{
			myBatch->RemoveObject(mySprites[x]->GetSprite(), mySprites[x]->GetSprite()->GetCSprite(), false);
			delete mySprites[x];
			mySprites.erase(mySprites.begin() + x);
			DeleteInactiveSpriteComponents();
		}
	}
}

const void ParticleEffect::CheckActiveStats()
{
	if (myActiveEffect && !myAddedEmitter && myStats.myEmitterLifeTime > 0)
	{
		myAddedEmitter = true;
	}

	if (myStats.myPauseTimer > 0)
		myAddedPauseTimer = true;

	if (myObjectIsFollowing)
	{
		if (myFollowObject->myTransform.myShouldBeDestroyed || !myFollowObject->IsActive())
		{
			for (auto sprite : mySprites)
				sprite->SetInactive();

			myKilledEffect = true;
		}
		else
			SetPosition(myFollowObject->GetPosition());
	}
}

const void ParticleEffect::DeleteSprites()
{
	for (int x = mySprites.size() - 1; x >= 0; x--)
	{
		myBatch->RemoveObject(mySprites[x]->GetSprite(), mySprites[x]->GetSprite()->GetCSprite(), false);
		delete mySprites[x];
		mySprites.erase(mySprites.begin() + x);
	}

	delete myBatch;
	myBatch = nullptr;
	DeleteComponents();
	mySprites.clear();
}

const void ParticleEffect::SetEffect(ParticleStats aEffect)
{
	myStats = aEffect;
}

const void ParticleEffect::SetFollowObject(GameObject& aFollowObject)
{
	myFollowObject = &aFollowObject;
	myObjectIsFollowing = true;
}
