#include "stdafx.h"
#include "ParticleEffectFactory.h"
#include "ParticleStats.hpp"
#include "Scene.h"
#include "LevelScene.h"
#include "Player.hpp"
#include "Subscriber.hpp"
#include "PostMaster.hpp"
#include "Random.hpp"
#include "ColliderComponent.h"
#include "PhysicsComponent.h"
#include "ParticleStartUpTypes.hpp"

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
	myActiveFireFlies = {};
	myActiveRain = {};
	myActiveRainBackground = {};
	myActiveRainBackgroundDust = {};
	myStartup = false;
	myPlayer = {};

}

ParticleEffectFactory::~ParticleEffectFactory()
{
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
	if (!myStartup)
	{
		AddSubscribers();
		StartEffects();
		myStartup = true;
	}


	for (int i = mySpawningEffects.size() - 1; i >= 0; i--)
	{
		mySpawningEffects[i].myTimer += aDeltaTime;
		mySpawningEffects[i].myTotalTimer += aDeltaTime;

		if (mySpawningEffects[i].mySpawningAllTime && mySpawningEffects[i].myTimer >= mySpawningEffects[i].mySpawnEverySecond)
		{
			mySpawningEffects[i].myTimer = {};

			SpawnEffect(mySpawningEffects[i].myGameObject->GetPosition(), mySpawningEffects[i].myEffectType);
		}
		else if (mySpawningEffects[i].myTimer >= mySpawningEffects[i].mySpawnEverySecond && mySpawningEffects[i].myTotalTimer <= mySpawningEffects[i].myTotalSpawnTimer && !mySpawningEffects[i].mySpawningAllTime)
		{
			mySpawningEffects[i].myTimer = {};

			if (mySpawningEffects[i].myEffectType == eParticleEffects::PlayerBashedPlayerParticle)
			{
				ParticleEffect* effect = SpawnEffect(mySpawningEffects[i].myGameObject->GetPosition(), mySpawningEffects[i].myEffectType);

				CheckPlayerSpritePath(effect, i);
				CheckPlayerSpriteDirection(effect);

				mySpawningEffects[i].mySpawnAmount++;
			}
			else
			{
				SpawnEffect(mySpawningEffects[i].myGameObject->GetPosition(), mySpawningEffects[i].myEffectType);
			}
		}

		if (!mySpawningEffects[i].myGameObject->IsActive() || mySpawningEffects[i].myGameObject->GetShouldBeDestroyed())
		{
			mySpawningEffects.erase(mySpawningEffects.begin() + i);
			break;
		}

		if (mySpawningEffects[i].myTotalTimer >= mySpawningEffects[i].myTotalSpawnTimer && !mySpawningEffects[i].mySpawningAllTime)
		{
			mySpawningEffects.erase(mySpawningEffects.begin() + i);
			break;
		}
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
	case eMessageType::EnemyPurpleTrailParticle:
	{
		GameObject* gameobjectToFollow = aMessage.myEffectObject;

		SpawnEffect(gameobjectToFollow, eParticleEffects::EnemyPurpleTrailParticle);
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
	case eMessageType::CollectibleCollectedParticleEasy:
	{
		const v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::CollectibleCollectedParticleEasy);
		break;
	}
	case eMessageType::CollectibleCollectedParticleMedium:
	{
		const v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::CollectibleCollectedParticleMedium);
		break;
	}
	case eMessageType::CollectibleCollectedParticleHard:
	{
		const v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::CollectibleCollectedParticleHard);
		break;
	}
	case eMessageType::CollectibleTrailEffectEasy:
	{
		GameObject* gameobjectToFollow = aMessage.myEffectObject;

		SpawnEffect(gameobjectToFollow, eParticleEffects::CollectibleTrailEffectEasy);
		break;
	}
	case eMessageType::CollectibleTrailEffectMedium:
	{
		GameObject* gameobjectToFollow = aMessage.myEffectObject;

		SpawnEffect(gameobjectToFollow, eParticleEffects::CollectibleTrailEffectMedium);
		break;
	}
	case eMessageType::CollectibleTrailEffectHard:
	{
		GameObject* gameobjectToFollow = aMessage.myEffectObject;

		SpawnEffect(gameobjectToFollow, eParticleEffects::CollectibleTrailEffectHard);
		break;
	}
	case eMessageType::CollectibleTrailEffect:
	{
		GameObject* gameobjectToFollow = aMessage.myEffectObject;

		SpawnEffect(gameobjectToFollow, eParticleEffects::CollectibleTrailEffect);
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
	case eMessageType::BonfireWakeupTopParticleEasy:
	{
		v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::BonfireWakeupTopParticleEasy);
		break;
	}
	case eMessageType::BonfireWakeupExplosionParticleEasy:
	{
		v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::BonfireWakeupExplosionParticleEasy);
		break;
	}
	case eMessageType::BonfireIdleParticleEasy:
	{
		v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::BonfireIdleParticleEasy);
		break;
	}
	case eMessageType::BonfireWakeupTopParticleMedium:
	{
		v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::BonfireWakeupTopParticleEasy);
		break;
	}
	case eMessageType::BonfireWakeupExplosionParticleMedium:
	{
		v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::BonfireWakeupExplosionParticleEasy);
		break;
	}
	case eMessageType::BonfireIdleParticleMedium:
	{
		v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::BonfireIdleParticleMedium);
		break;
	}
	case eMessageType::BonfireWakeupTopParticleHard:
	{
		v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::BonfireWakeupTopParticleHard);
		break;
	}
	case eMessageType::BonfireWakeupExplosionParticleHard:
	{
		v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::BonfireWakeupExplosionParticleHard);
		break;
	}
	case eMessageType::BonfireIdleParticleHard:
	{
		v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::BonfireIdleParticleHard);
		break;
	}
	case eMessageType::DustParticleOne:
	{
		v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::DustParticleOne);
		break;
	}
	case eMessageType::DustParticleTwo:
	{
		v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::DustParticleTwo);
		break;
	}
	case eMessageType::DustParticleThree:
	{
		v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::DustParticleThree);
		break;
	}
	case eMessageType::DustParticleStartupOne:
	{
		v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::DustParticleStartupOne);
		break;
	}
	case eMessageType::DustParticleStartupTwo:
	{
		v2f position = std::get<v2f>(aMessage.myData);

		SpawnEffect(position, eParticleEffects::DustParticleStartupTwo);
		break;
	}
	case eMessageType::UnstablePlatformParticleOne:
	{
		GameObject* gameobjectToFollow = aMessage.myEffectObject;
		v2f position = gameobjectToFollow->GetPosition();

		ParticleEffect* effect = SpawnEffect(position, eParticleEffects::UnstablePlatformParticleOne);
		effect->SetWidth(gameobjectToFollow->GetComponent<ColliderComponent>()->GetWidth() * 0.5f);
		effect->SetOffset(gameobjectToFollow->GetComponent<ColliderComponent>()->GetWidth() * 0.5f);
		break;
	}
	case eMessageType::UnstablePlatformParticleTwo:
	{
		GameObject* gameobjectToFollow = aMessage.myEffectObject;
		v2f position = gameobjectToFollow->GetPosition();

		ParticleEffect* effect = SpawnEffect(position, eParticleEffects::UnstableGroundParticleTwo);
		effect->SetWidth(gameobjectToFollow->GetComponent<ColliderComponent>()->GetWidth() * 0.5f);
		effect->SetOffset(gameobjectToFollow->GetComponent<ColliderComponent>()->GetWidth() * 0.5f);
		break;
	}
	case eMessageType::UnstableGroundIdleParticle:
	{
		GameObject* gameobjectToFollow = aMessage.myEffectObject;
		v2f position = gameobjectToFollow->GetPosition();

		ParticleEffect* effect = SpawnEffect(position, eParticleEffects::UnstableGroundIdleParticle);
		effect->SetWidth(gameobjectToFollow->GetComponent<ColliderComponent>()->GetWidth() * 0.5f);
		effect->SetOffset(gameobjectToFollow->GetComponent<ColliderComponent>()->GetWidth() * 0.5f);
		effect->SetGameObject(gameobjectToFollow);

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

	if (timerEffect.myTotalSpawnTimer <= 0.f)
		timerEffect.mySpawningAllTime = true;

	mySpawningEffects.push_back(timerEffect);
}

ParticleEffect* ParticleEffectFactory::SpawnEffect(const v2f aPosition, const eParticleEffects aEffectType)
{
	ParticleEffect* effect = new ParticleEffect(myScene);

	SetEffect(*effect, aEffectType);

	effect->SetPosition(aPosition);
	effect->SetIsActive(true);

	return effect;
}

void ParticleEffectFactory::SpawnEffectFollowObject(GameObject* aObject, const eParticleEffects aEffectType)
{
	ParticleEffect* effect = new ParticleEffect(myScene);

	SetEffect(*effect, aEffectType);

	effect->SetFollowObject(*aObject);
	effect->SetIsActive(true);
}

void ParticleEffectFactory::StartEffect(const int aIndex)
{
	eEffectStartUpTypes startupType = static_cast<eEffectStartUpTypes>(aIndex);

	switch (startupType)
	{
	case eEffectStartUpTypes::FireFlies:
	{

		break;
	}
	case eEffectStartUpTypes::RainForeground_Background:
	{

		break;
	}
	case eEffectStartUpTypes::RainBackground:
	{

		break;
	}
	case eEffectStartUpTypes::RainBackground_Dust:
	{

		break;
	}
	default:
		break;
	}

}

void ParticleEffectFactory::StartAllRainEffects()
{
	myActiveRain = true;
}

void ParticleEffectFactory::StartFirefliesEffects()
{
	myActiveFireFlies = true;
}

const void ParticleEffectFactory::AddSubscribers()
{
	Subscribe(eMessageType::DustParticleStartupOne);
	Subscribe(eMessageType::DustParticleStartupTwo);
	Subscribe(eMessageType::DustParticleOne);
	Subscribe(eMessageType::DustParticleTwo);
	Subscribe(eMessageType::DustParticleThree);
	Subscribe(eMessageType::CollectibleCollectedParticleEasy);
	Subscribe(eMessageType::CollectibleCollectedParticleMedium);
	Subscribe(eMessageType::CollectibleCollectedParticleHard);
	Subscribe(eMessageType::CollectibleTrailEffectEasy);
	Subscribe(eMessageType::CollectibleTrailEffectMedium);
	Subscribe(eMessageType::CollectibleTrailEffectHard);
	Subscribe(eMessageType::CollectibleTrailEffect);
	Subscribe(eMessageType::UnstablePlatformParticleOne);
	Subscribe(eMessageType::UnstablePlatformParticleTwo);
	Subscribe(eMessageType::UnstableGroundIdleParticle);
	Subscribe(eMessageType::BonfireIdleParticleEasy);
	Subscribe(eMessageType::BonfireWakeupExplosionParticleEasy);
	Subscribe(eMessageType::BonfireWakeupTopParticleEasy);
	Subscribe(eMessageType::BonfireIdleParticleMedium);
	Subscribe(eMessageType::BonfireWakeupExplosionParticleMedium);
	Subscribe(eMessageType::BonfireWakeupTopParticleMedium);
	Subscribe(eMessageType::BonfireIdleParticleHard);
	Subscribe(eMessageType::BonfireWakeupExplosionParticleHard);
	Subscribe(eMessageType::BonfireWakeupTopParticleHard);
	Subscribe(eMessageType::RainEffectNextScreenParticle);
	Subscribe(eMessageType::RainEffectForegroundParticle);
	Subscribe(eMessageType::RainEffectBackgroundParticle);
	Subscribe(eMessageType::VelocityLinesParticle);
	Subscribe(eMessageType::PlayerLedgeLeftGrabbedLegParticle);
	Subscribe(eMessageType::PlayerLedgeLeftGrabbedHandParticle);
	Subscribe(eMessageType::PlayerLedgeRightGrabbedLegParticle);
	Subscribe(eMessageType::PlayerLedgeRightGrabbedHandParticle);
	Subscribe(eMessageType::PlayerLandedParticle);
	Subscribe(eMessageType::PlayerBashedPlayerParticle);
	Subscribe(eMessageType::PlayerBashedSmallParticle);
	Subscribe(eMessageType::EnemyShootingTrailParticle);
	Subscribe(eMessageType::EnemyShootingBulletHitParticle);
	Subscribe(eMessageType::EnemyBulletTrailEmitter);
	Subscribe(eMessageType::EnemyPurpleTrailParticle);
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
	case eParticleEffects::CollectibleTrailEffect:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::CollectibleCollectedParticleEasy:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::CollectibleCollectedParticleMedium:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::CollectibleCollectedParticleHard:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::CollectibleTrailEffectEasy:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::CollectibleTrailEffectMedium:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::CollectibleTrailEffectHard:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::DeathEffect:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::RainEffectBackgroundParticle:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}case eParticleEffects::RainEffectForegroundParticle:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::BulletEffectTrail:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::BulletEffectHit:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::FogEffect:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::PlayerFallLandEffect:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::BulletEffectTrail2:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::EnemyPurpleTrailParticle:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::PlayerBashedPlayerParticle:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::PlayerBashedSmallParticle:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::PlayerLedgeRightGrabbedHandParticle:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::PlayerLedgeRightGrabbedLegParticle:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::PlayerLedgeLeftGrabbedHandParticle:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::PlayerLedgeLeftGrabbedLegParticle:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::VelocityLinesParticles:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::BonfireWakeupTopParticleEasy:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::BonfireWakeupExplosionParticleEasy:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::BonfireIdleParticleEasy:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::BonfireWakeupTopParticleMedium:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::BonfireWakeupExplosionParticleMedium:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::BonfireIdleParticleMedium:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::BonfireWakeupTopParticleHard:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::BonfireWakeupExplosionParticleHard:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::BonfireIdleParticleHard:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::UnstablePlatformParticleOne:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::UnstableGroundParticleTwo:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::DustParticleOne:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::DustParticleTwo:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::DustParticleThree:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::DustParticleStartupOne:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::DustParticleStartupTwo:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	case eParticleEffects::UnstableGroundIdleParticle:
	{
		aEffect.Init(myEffects[static_cast<int>(aEffectType)]);
		break;
	}
	}
}

const void ParticleEffectFactory::StartEffects()
{
	Camera& cam = myScene->GetCamera();
	v2f boundaries = cam.GetBoundSize();

	if (myActiveRain)
	{
		ParticleEffect* rainBackground = SpawnEffect(boundaries * 0.5f, eParticleEffects::RainEffectBackgroundParticle);
		ParticleEffect* rainForeground = SpawnEffect(boundaries * 0.5f, eParticleEffects::RainEffectForegroundParticle);
		ParticleEffect* rainNextScreen = SpawnEffect({ boundaries.x * 0.5f, boundaries.y }, eParticleEffects::RainEffectNextScreenParticle);

		rainBackground->SetWidth(boundaries.x);
		rainForeground->SetWidth(boundaries.x);
		rainNextScreen->SetWidth(boundaries.x);
	}

	if (myActiveRainBackground)
	{
		ParticleEffect* rainBackground = SpawnEffect(boundaries * 0.5f, eParticleEffects::RainEffectBackgroundParticle);
		rainBackground->SetWidth(boundaries.x);
	}

	if (myActiveRainBackgroundDust)
	{
		ParticleEffect* dustParticle1 = SpawnEffect(boundaries, eParticleEffects::DustParticleOne);
		ParticleEffect* dustParticle2 = SpawnEffect(boundaries, eParticleEffects::DustParticleTwo);
		ParticleEffect* dustParticle3 = SpawnEffect({ 0.f, boundaries.y }, eParticleEffects::DustParticleThree);
		ParticleEffect* dustparticleStartupOne = SpawnEffect({ boundaries.x * 0.5f, boundaries.y }, eParticleEffects::DustParticleStartupOne);
		ParticleEffect* dustparticleStartupTwo = SpawnEffect({ boundaries.x * 0.5f, boundaries.y }, eParticleEffects::DustParticleStartupTwo);

		dustParticle1->SetHeight(boundaries.y);
		dustParticle2->SetHeight(boundaries.y);
		dustParticle3->SetHeight(boundaries.y);
		dustparticleStartupOne->SetHeight(boundaries.y);
		dustparticleStartupTwo->SetHeight(boundaries.y);

		ParticleEffect* rainBackground = SpawnEffect(boundaries * 0.5f, eParticleEffects::RainEffectBackgroundParticle);
		rainBackground->SetWidth(boundaries.x);
	}

	if (myActiveFireFlies)
	{
		ParticleEffect* dustParticle1 = SpawnEffect(boundaries, eParticleEffects::DustParticleOne);
		ParticleEffect* dustParticle2 = SpawnEffect(boundaries, eParticleEffects::DustParticleTwo);
		ParticleEffect* dustParticle3 = SpawnEffect({0.f, boundaries.y}, eParticleEffects::DustParticleThree);
		ParticleEffect* dustparticleStartupOne = SpawnEffect({boundaries.x * 0.5f, boundaries.y}, eParticleEffects::DustParticleStartupOne);
		ParticleEffect* dustparticleStartupTwo = SpawnEffect({ boundaries.x * 0.5f, boundaries.y }, eParticleEffects::DustParticleStartupTwo);

		dustParticle1->SetHeight(boundaries.y);
		dustParticle2->SetHeight(boundaries.y);
		dustParticle3->SetHeight(boundaries.y);
		dustparticleStartupOne->SetHeight(boundaries.y);
		dustparticleStartupTwo->SetHeight(boundaries.y);
	}
}

const void ParticleEffectFactory::CheckPlayerSpriteDirection(ParticleEffect* aEffect)
{
	LevelScene* levelScene = dynamic_cast<LevelScene*>(myScene);

	if (levelScene != NULL)
		myPlayer = dynamic_cast<Player*>(levelScene->GetPlayer());

	if (myPlayer->GetComponent<PhysicsComponent>()->GetVelocityX() < 0)
		aEffect->SetReverseImage();
}

const void ParticleEffectFactory::CheckPlayerSpritePath(ParticleEffect* aEffect, const int aIndex)
{
	if (mySpawningEffects[aIndex].mySpawnAmount > 2)
		mySpawningEffects[aIndex].mySpawnAmount = {};

	aEffect->SetNewPlayerSprite(mySpawningEffects[aIndex].mySpawnAmount);
}
