#pragma once
#include "GameObject.h"
#include "ParticleEffect.h"
#include "Subscriber.hpp"
#include "GameObject.h"

class Scene;

class ParticleEffectFactory : public GameObject, public Subscriber
{
public:
	ParticleEffectFactory(Scene* aLevelScene);
	~ParticleEffectFactory();

	void ReadEffects();

	void Init() override;

	void Update(const float& aDeltaTime) override;

	void Notify(const Message& aMessage) override;

	void TestEffect(const v2f aPosition);
	void TestEffectFollowObject();
	void SpawnEffect(GameObject* aGameObject, const eParticleEffects aEffectType);
	ParticleEffect* SpawnEffect(const v2f aPosition, const eParticleEffects aEffectType);
	void SpawnEffectFollowObject(GameObject* aObject, const eParticleEffects aEffectType);

	void StartEffect(const int aIndex);

private:
	struct SpawnEffects
	{
		GameObject* myGameObject = {};
		eParticleEffects myEffectType = {};
		float myTimer = {};
		float myTotalTimer = {};
		float myTotalSpawnTimer = {};
		float mySpawnEverySecond = {};
		int mySpawnAmount = {};
		bool mySpawningAllTime = false;
	};

	Scene* myScene;
	Player* myPlayer;

	std::vector<ParticleStats> myEffects;
	std::vector<SpawnEffects> mySpawningEffects;

	bool myActiveFireFlies;
	bool myActiveRain;
	bool myActiveRainBackground;
	bool myActiveRainBackgroundDust;
	bool myStartup;

	int myTestIndex;

	const void AddSubscribers();
	void SetEffect(ParticleEffect& aEffect, const eParticleEffects aEffectType);
	const void StartEffects();
	const void CheckPlayerSpriteDirection(ParticleEffect* aEffect);
	const void CheckPlayerSpritePath(ParticleEffect* aEffect, const int aIndex);

};

