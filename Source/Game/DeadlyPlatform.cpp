#include "stdafx.h"
#include "DeadlyPlatform.hpp"
#include "AudioManager.h"
#include "Player.hpp"

#include "PhysicsComponent.h"

DeadlyPlatform::DeadlyPlatform(Scene* aLevelScene)
	:
	Platform(aLevelScene)
{

}

void DeadlyPlatform::Init(const v2f& aSize, const v2f& aSpriteSize, const v2f& aPosition, const bool& aIsOneway)
{
	Platform::Init(aSize, aSpriteSize, aPosition, aIsOneway);

	GetComponent<PhysicsComponent>()->SetCanCollide(false);
}

void DeadlyPlatform::OnCollision(GameObject* aGameObject)
{
	Player* player = dynamic_cast<Player*>(aGameObject);
	if (player)
	{
		if (!player->GetHasDied())
		{
			switch (myMaterial)
			{
			case 5: AudioManager::GetInstance()->PlayAudio(AudioList::FallDeath); break;
			case 6: AudioManager::GetInstance()->PlayAudio(AudioList::SpikeHit); AudioManager::GetInstance()->PlayAudio(AudioList::SpikeDeath); break;
			case 7: AudioManager::GetInstance()->PlayAudio(AudioList::ThornDeath); AudioManager::GetInstance()->PlayAudio(AudioList::SpikeDeath); break;
			}
			player->Kill();
		}
	}
}