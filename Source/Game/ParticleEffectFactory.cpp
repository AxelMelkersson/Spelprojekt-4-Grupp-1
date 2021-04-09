#include "stdafx.h"
#include "ParticleEffectFactory.h"
#include "ParticleStats.hpp"
#include "Scene.h"
#include "LevelScene.h"
#include "Player.hpp"
#include "Subscriber.hpp"
#include "PostMaster.hpp"
#include "Random.hpp"

#include "../External/Headers/rapidjson/document.h"
#include "../External/Headers/rapidjson/istreamwrapper.h"
#include <fstream>

ParticleEffectFactory::ParticleEffectFactory(Scene* aLevelScene)
	:
	GameObject(aLevelScene)
{
	myScene = aLevelScene;
	myEffects = {};
	myTestIndex = {};
	myHasAddedSubscribers = false;
}

ParticleEffectFactory::~ParticleEffectFactory()
{
	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::BonfireWakeupExplosionParticle);
	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::BonfireWakeupTopParticle);
	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::RainEffectNextScreenParticle);
	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::RainEffectForegroundParticle);
	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::RainEffectBackgroundParticle);
	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::CollectibleCollectedParticle);
	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::VelocityLinesParticle);
	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::PlayerLedgeLeftGrabbedLegParticle);
	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::PlayerLedgeLeftGrabbedHandParticle);
	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::PlayerLedgeRightGrabbedLegParticle);
	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::PlayerLedgeRightGrabbedHandParticle);
	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::PlayerLandedParticle);
	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::PlayerBashedPlayerParticle);
	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::PlayerBashedSmallParticle);
	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::EnemyBulletTrailEmitter);
	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::EnemyShootingTrailParticle);
	PostMaster::GetInstance().RemoveSubcriber(this, eMessageType::EnemyShootingBulletHitParticle);
	GameObject::~GameObject();
}

void ParticleEffectFactory::ReadEffects()
{
	int index = {};
	std::ifstream effectObjectFile("JSON/Particles/MasterParticles.json");
	rapidjson::IStreamWrapper effectObjectStream(effectObjectFile);

	rapidjson::Document effectDocuments;
	effectDocuments.ParseStream(effectObjectStream);

	myTestIndex = effectDocuments["TestEffect"]["File"].GetInt64();

	for (rapidjson::Value::ConstValueIterator effectPaths = effectDocuments["particlesPath"].Begin(); effectPaths != effectDocuments["particlesPath"].End(); ++effectPaths)
	{
		std::string path = "JSON/Particles/infile";
		path.append(std::to_string(index) + ".json");

		std::ifstream particleFile(path);
		rapidjson::IStreamWrapper particleStream(particleFile);

		rapidjson::Document particleDocuments;
		particleDocuments.ParseStream(particleStream);

		for (rapidjson::Value::ConstValueIterator particleStat = particleDocuments["particle"].Begin(); particleStat != particleDocuments["particle"].End(); ++particleStat)
		{
			ParticleStats stats;
			stats.myZIndex = (*particleStat)["ZIndex"].GetInt();
			stats.myEffectTypeIndex = (*particleStat)["EffectTypeIndex"].GetInt();
			stats.mySpritePath = (*particleStat)["SpritePath"].GetString();
			stats.myLockedPosition = (*particleStat)["LockPosition"].GetBool();
			stats.myPauseTimer = (*particleStat)["PauseTimer"].GetFloat();
			stats.myPauseSeconds = (*particleStat)["PauseSeconds"].GetFloat();
			stats.myStartScale = (*particleStat)["StartScale"].GetFloat();
			stats.myEndScale = (*particleStat)["EndScale"].GetFloat();
			stats.myMinBetweenSpawn = (*particleStat)["MinTimeBetweenSpawns"].GetFloat();
			stats.myMaxBetweenSpawn = (*particleStat)["MaxTimeBetweenSpawns"].GetFloat();
			stats.myMinSpeed = (*particleStat)["MinSpeed"].GetFloat();
			stats.myMaxSpeed = (*particleStat)["MaxSpeed"].GetFloat();
			stats.myMinParticleAngularVel = (*particleStat)["MinParticleAngularVel"].GetFloat();
			stats.myMaxParticleAngularVel = (*particleStat)["MaxParticleAngularVel"].GetFloat();
			stats.myEmitterAngle = (*particleStat)["EmitterAngle"].GetFloat();
			stats.myEmitterWidth = (*particleStat)["EmitterWidth"].GetFloat();
			stats.myEmitterHeigth = (*particleStat)["EmitterHeigth"].GetFloat();
			stats.myMinParticleLifeTime = (*particleStat)["MinParticleLifeTime"].GetFloat();
			stats.myMaxParticleLifeTime = (*particleStat)["MaxParticleLifeTime"].GetFloat();
			stats.myEmitterLifeTime = (*particleStat)["EmitterLifeTime"].GetFloat();
			stats.mySpriteRotation = (*particleStat)["RotateSprite"].GetFloat();
			stats.mySpawnEverySecond = (*particleStat)["SpawnEverySecond"].GetFloat();
			stats.mySpawnLifeTime = (*particleStat)["SpawnLifeTime"].GetFloat();
			stats.myOffset = { (*particleStat)["Offset"][0]["X"].GetFloat(), (*particleStat)["Offset"][1]["Y"].GetFloat() };

			stats.myEmitterAngular = { (*particleStat)["LockedAngular"][0].GetFloat(), (*particleStat)["LockedAngular"][1].GetFloat(), (*particleStat)["LockedAngular"][2].GetFloat(), (*particleStat)["LockedAngular"][3].GetFloat() };
			stats.myStartColor = { (*particleStat)["StartColor"][0].GetFloat() / 255.f, (*particleStat)["StartColor"][1].GetFloat() / 255.f, (*particleStat)["StartColor"][2].GetFloat() / 255.f, (*particleStat)["StartColor"][3].GetFloat()};
			stats.myEndColor = { (*particleStat)["EndColor"][0].GetFloat() / 255.f, (*particleStat)["EndColor"][1].GetFloat() / 255.f, (*particleStat)["EndColor"][2].GetFloat() / 255.f, (*particleStat)["EndColor"][3].GetFloat()};


			myEffects.push_back(stats);
		}

		particleFile.close();
		index++;
		
	}

	effectObjectFile.close();


}

void ParticleEffectFactory::Init()
{
	ReadEffects();
	GameObject::Init();
}

void ParticleEffectFactory::Update(const float& aDeltaTime)
{
	if (!myHasAddedSubscribers)
	{
		AddSubscribers();
	}


	for (int i = mySpawningEffects.size() - 1; i >= 0; i--)
	{
		mySpawningEffects[i].myTimer += aDeltaTime;
		mySpawningEffects[i].myTotalTimer += aDeltaTime;

		if (mySpawningEffects[i].myTimer >= mySpawningEffects[i].mySpawnEverySecond && mySpawningEffects[i].myTotalTimer <= mySpawningEffects[i].myTotalSpawnTimer)
		{
			mySpawningEffects[i].myTimer = {};

			SpawnEffect(mySpawningEffects[i].myGameObject->GetPosition(), mySpawningEffects[i].myEffectType);
		}

		if (mySpawningEffects[i].myTotalTimer >= mySpawningEffects[i].myTotalSpawnTimer)
			mySpawningEffects.erase(mySpawningEffects.begin() + i);
	}
}

void ParticleEffectFactory::Notify(const Message& aMessage)
{
	switch (aMessage.myMessageType)
	{
	case eMessageType::EnemyShootingTrailParticle:
	{
		const v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::BulletEffectTrail2);
		break;
	}
	case eMessageType::PlayerLandedParticle:
	{
		const v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::PlayerFallLandEffect);
		break;
	}
	case eMessageType::PlayerLedgeRightGrabbedHandParticle:
	{
		const v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::PlayerLedgeRightGrabbedHandParticle);
		break;
	}
	case eMessageType::PlayerLedgeRightGrabbedLegParticle:
	{
		const v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::PlayerLedgeRightGrabbedLegParticle);
		break;
	}
	case eMessageType::PlayerLedgeLeftGrabbedHandParticle:
	{
		const v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::PlayerLedgeLeftGrabbedHandParticle);
		break;
	}
	case eMessageType::PlayerLedgeLeftGrabbedLegParticle:
	{
		const v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::PlayerLedgeLeftGrabbedLegParticle);
		break;
	}
	case eMessageType::PlayerBashedPlayerParticle:
	{
		GameObject* gameobjectToFollow = aMessage.myEffectObject;

		SpawnEffect(gameobjectToFollow, eParticleEffects::PlayerBashedPlayerParticle);
		break;
	}
	case eMessageType::PlayerBashedSmallParticle:
	{
		GameObject* gameobjectToFollow = aMessage.myEffectObject;

		SpawnEffect(gameobjectToFollow, eParticleEffects::PlayerBashedSmallParticle);
		break;
	}
	case eMessageType::EnemyShootingBulletHitParticle:
	{
		const v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::BulletEffectHit);
		break;
	}
	case eMessageType::EnemyBulletTrailEmitter:
	{
		GameObject* gameobjectToFollow = aMessage.myEffectObject;

		SpawnEffectFollowObject(gameobjectToFollow, eParticleEffects::BulletEffectTrail);
		break;
	}
	{
		const v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::BulletEffectHit);
		break;
	}
	case eMessageType::VelocityLinesParticle:
	{
		GameObject* gameobjectToFollow = aMessage.myEffectObject;

		SpawnEffect(gameobjectToFollow, eParticleEffects::VelocityLinesParticles);
		break;
	}
	case eMessageType::CollectibleCollectedParticle:
	{
		const v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::CollectibleCollectedParticle);
		break;
	}
	case eMessageType::RainEffectBackgroundParticle:
	{
		const v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::RainEffectBackgroundParticle);
		break;
	}
	case eMessageType::RainEffectForegroundParticle:
	{
		const v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::RainEffectForegroundParticle);
		break;
	}
	case eMessageType::RainEffectNextScreenParticle:
	{
		v2f position = std::get<v2f>(aMessage.myData);
		position.y = 90.f;

		SpawnEffect(position, eParticleEffects::RainEffectNextScreenParticle);
		break;
	}
	case eMessageType::BonfireWakeupTopParticle:
	{
		v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::BonfireWakeupTopParticle);
		break;
	}
	case eMessageType::BonfireWakeupExplosionParticle:
	{
		v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::BonfireWakeupExplosionParticle);
		break;
	}
	default:
	{
		break;
	}
	}

}


void ParticleEffectFactory::TestEffect(const v2f aPosition)
{
	ParticleEffect* effect = new ParticleEffect(myScene);

	SetEffect(*effect, static_cast<eParticleEffects>(myTestIndex));

	effect->SetPosition(aPosition);
	effect->SetIsActive(true);
}

void ParticleEffectFactory::TestEffectFollowObject()
{
	LevelScene* levelScene = dynamic_cast<LevelScene*>(myScene);

	ParticleEffect* effect = new ParticleEffect(myScene);

	SetEffect(*effect, static_cast<eParticleEffects>(myTestIndex));

	effect->SetFollowObject(*levelScene->GetPlayer());
	effect->SetIsActive(true);
}

void ParticleEffectFactory::SpawnEffect(GameObject* aGameObject, const eParticleEffects aEffectType)
{
	SpawnEffects timerEffect;
	timerEffect.myGameObject = aGameObject;
	timerEffect.myEffectType = aEffectType;
	timerEffect.myTotalSpawnTimer = myEffects[static_cast<int>(aEffectType)].mySpawnLifeTime;
	timerEffect.mySpawnEverySecond = myEffects[static_cast<int>(aEffectType)].mySpawnEverySecond;

	mySpawningEffects.push_back(timerEffect);
}

void ParticleEffectFactory::SpawnEffect(const v2f aPosition, const eParticleEffects aEffectType)
{
	ParticleEffect* effect = new ParticleEffect(myScene);

	SetEffect(*effect, aEffectType);

	effect->SetPosition(aPosition);
	effect->SetIsActive(true);
}

void ParticleEffectFactory::SpawnEffectFollowObject(GameObject* aObject, const eParticleEffects aEffectType)
{
	ParticleEffect* effect = new ParticleEffect(myScene);

	SetEffect(*effect, aEffectType);

	effect->SetFollowObject(*aObject);
	effect->SetIsActive(true);
}

const void ParticleEffectFactory::AddSubscribers()
{
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::BonfireWakeupExplosionParticle);
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::BonfireWakeupTopParticle);
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::RainEffectNextScreenParticle);
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::RainEffectForegroundParticle);
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::RainEffectBackgroundParticle);
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::CollectibleCollectedParticle);
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::VelocityLinesParticle);
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::PlayerLedgeLeftGrabbedLegParticle);
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::PlayerLedgeLeftGrabbedHandParticle);
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::PlayerLedgeRightGrabbedLegParticle);
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::PlayerLedgeRightGrabbedHandParticle);
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::PlayerLandedParticle);
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::PlayerBashedPlayerParticle);
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::PlayerBashedSmallParticle);
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::EnemyShootingTrailParticle);
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::EnemyShootingBulletHitParticle);
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::EnemyBulletTrailEmitter);
	myHasAddedSubscribers = true;
}

void ParticleEffectFactory::SetEffect(ParticleEffect& aEffect, const eParticleEffects aEffectType)
{
	switch (aEffectType)
	{
	case eParticleEffects::RainEffectNextScreenParticle:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::CollectibleEffect:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::CollectibleEffect)]);
		break;
	}
	case eParticleEffects::DeathEffect:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::DeathEffect)]);
		break;
	}
	case eParticleEffects::RainEffectBackgroundParticle:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::RainEffectBackgroundParticle)]);
		break;
	}case eParticleEffects::RainEffectForegroundParticle:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::RainEffectForegroundParticle)]);
		break;
	}
	case eParticleEffects::BulletEffectTrail:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::BulletEffectTrail)]);
		break;
	}
	case eParticleEffects::BulletEffectHit:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::BulletEffectHit)]);
		break;
	}
	case eParticleEffects::FogEffect:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::FogEffect)]);
		break;
	}
	case eParticleEffects::PlayerFallLandEffect:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::PlayerFallLandEffect)]);
		break;
	}
	case eParticleEffects::BulletEffectTrail2:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::BulletEffectTrail2)]);
		break;
	}
	case eParticleEffects::PlayerBashedPlayerParticle:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::PlayerBashedPlayerParticle)]);
		break;
	}
	case eParticleEffects::PlayerBashedSmallParticle:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::PlayerBashedSmallParticle)]);
		break;
	}
	case eParticleEffects::PlayerLedgeRightGrabbedHandParticle:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::PlayerLedgeRightGrabbedHandParticle)]);
		break;
	}
	case eParticleEffects::PlayerLedgeRightGrabbedLegParticle:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::PlayerLedgeRightGrabbedLegParticle)]);
		break;
	}
	case eParticleEffects::PlayerLedgeLeftGrabbedHandParticle:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::PlayerLedgeLeftGrabbedHandParticle)]);
		break;
	}
	case eParticleEffects::PlayerLedgeLeftGrabbedLegParticle:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::PlayerLedgeLeftGrabbedLegParticle)]);
		break;
	}
	case eParticleEffects::VelocityLinesParticles:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::VelocityLinesParticles)]);
		break;
	}
	case eParticleEffects::CollectibleCollectedParticle:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::CollectibleCollectedParticle)]);
		break;
	}
	case eParticleEffects::BonfireWakeupTopParticle:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::BonfireWakeupTopParticle)]);
		break;
	}
	case eParticleEffects::BonfireWakeupExplosionParticle:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::BonfireWakeupExplosionParticle)]);
		break;
	}
	}
}
