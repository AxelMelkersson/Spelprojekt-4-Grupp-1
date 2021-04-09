#pragma once
#include "ParticleStats.hpp"
#include "ParticleEffectTypes.hpp"
#include "GameObject.h"
#include "EffectSprite.h"

class SpritebatchComponent;
class SpriteComponent;
class Player;
class LevelScene;

class ParticleEffect : public GameObject
{
public:
	ParticleEffect(Scene* aLevelScene);
	~ParticleEffect();

	void Init(ParticleStats aStats);

	void Render() override;
	void Update(const float& aDeltaTime) override;

	const void SetIsActive(const bool aActiveState);
	const void SetEffect(ParticleStats aEffect);
	const void SetFollowObject(GameObject& aFollowObject);
	
	const bool GetIsActive();
	const eParticleEffects GetType() const;


private:
	std::vector<EffectSprite*> mySprites;

	GameObject* myFollowObject;
	SpritebatchComponent* myBatch;
	ParticleStats myStats;

	float mySpawnInterval;
	float myLifeTime;
	float myTimer;
	float myPauseTimer;

	int myZIndex; 

	bool mySpawningInLocalSpace;
	bool myKilledEffect;
	bool myObjectIsFollowing;
	bool myInitBatching;
	bool myActiveEffect;
	bool myCreatingSprites;
	bool myAddedEmitter;
	bool myAddedPauseTimer;
	bool myEffectIsDestroyed;

	const void UpdateParticle(const float& aDeltaTime);
	const void CheckWhenToSpawnSprites();
	const void SpawnSprite();
	const void CheckIfEffectIsDead();
	const void CheckIfSpritesAreDead(const float& aDeltaTime);
	const void CheckActiveStats();
	const void DeleteSprites();

};

