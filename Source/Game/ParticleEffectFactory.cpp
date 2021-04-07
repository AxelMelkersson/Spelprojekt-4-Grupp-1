#include "stdafx.h"
#include "ParticleEffectFactory.h"
#include "ParticleStats.hpp"
#include "Scene.h"
#include "LevelScene.h"
#include "Player.hpp"
#include "Subscriber.hpp"
#include "PostMaster.hpp"

#include "../External/Headers/rapidjson/document.h"
#include "../External/Headers/rapidjson/istreamwrapper.h"
#include <fstream>

ParticleEffectFactory::ParticleEffectFactory()
{
	myScene = {};
	myPlayer = {};
	myEffects = {};
	myTestIndex = {};
}

void ParticleEffectFactory::ReadEffects(Scene* aLevelScene)
{
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::PlayerLandedParticle);
	PostMaster::GetInstance().AddSubcriber(this, eMessageType::EnemyShootingTrailParticle);

	myScene = aLevelScene;
	LevelScene* levelScene = dynamic_cast<LevelScene*>(aLevelScene);
	Player* player = dynamic_cast<Player*>(levelScene->GetPlayer());
	myPlayer = player;

	assert(player != NULL);

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
	//SpawnCharacterEffects();
}

void ParticleEffectFactory::Notify(const Message& aMessage)
{
	const v2f position = std::get<v2f>(aMessage.myData);

	switch (aMessage.myMessageType)
	{
	case eMessageType::EnemyShootingTrailParticle:
	{
		SpawnEffect(position, eParticleEffects::TrailEffect2);
		break;
	}
	case eMessageType::PlayerLandedParticle:
	{
		SpawnEffect(position, eParticleEffects::FallEffect);
		break;
	}
	default:
		break;
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
	ParticleEffect* effect = new ParticleEffect(myScene);

	SetEffect(*effect, static_cast<eParticleEffects>(myTestIndex));

	effect->SetFollowObject(*myPlayer);
	effect->SetIsActive(true);
}

void ParticleEffectFactory::SpawnEffect(v2f aPosition, const eParticleEffects aEffectType)
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

void ParticleEffectFactory::SetEffect(ParticleEffect& aEffect, const eParticleEffects aEffectType)
{
	switch (aEffectType)
	{
	case eParticleEffects::RunEffect:
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
	case eParticleEffects::RainEffectBackground:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::RainEffectBackground)]);
		break;
	}case eParticleEffects::RainEffectForeground:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::RainEffectForeground)]);
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
	case eParticleEffects::FallEffect:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::FallEffect)]);
		break;
	}
	case eParticleEffects::TrailEffect2:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::TrailEffect2)]);
		break;
	}
	case eParticleEffects::TestEffect2:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::TestEffect2)]);
		break;
	}
	case eParticleEffects::TestEffect3:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::TestEffect3)]);
		break;
	}
	case eParticleEffects::TestEffect4:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::TestEffect4)]);
		break;
	}
	case eParticleEffects::TestEffect5:
	{
		aEffect.Init(myEffects[static_cast<int>(eParticleEffects::TestEffect5)]);
		break;
	}
	}
}


void ParticleEffectFactory::SpawnCharacterEffects()
{
	ParticleEffect* effect = new ParticleEffect(myScene);
	effect->Init(myEffects[static_cast<int>(eParticleEffects::RunEffect)]);
	effect->SetIsActive(true);
}
