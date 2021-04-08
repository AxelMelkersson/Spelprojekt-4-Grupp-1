#include "stdafx.h"
#include "AudioManager.h"
#include "AudioComponent.h"
#include "Random.hpp"
#include "tga2d/audio/audio_out.h"
#include "AudioClip.h"
#include <bass/bass.h>
#include <algorithm>
#include <assert.h>

#pragma comment(lib,"bass.lib")

static std::unique_ptr<AudioManager> ourInstance = std::make_unique<AudioManager>();

bool CompareVolumes(AudioComponent* aComponent1, AudioComponent* aComponent2)
{
	return aComponent2->myVolume < aComponent1->myVolume;
}

const std::unique_ptr<AudioManager>& AudioManager::GetInstance()
{
	return ourInstance;
}

void AudioManager::Init()
{
	myAudioOut = {};
	mySFXVolume = 0.2f;
	myMusicVolume = 0.5f;
	SetMusicVolume(mySFXVolume);
	SetSFXVolume(mySFXVolume);
}

void AudioManager::Update(const float& aDeltaTime)
{
	ComponentUpdate();
	Fade(aDeltaTime);
}

void AudioManager::SetMusicVolume(float aVolume)
{
	myMusicVolume = aVolume;
	for (auto const& [key, val] : myLibrary.myAudioList)
	{
		if (myLibrary.myAudioList[key]->GetLayer() == AudioLayer::Music)
		{
			myLibrary.myAudioList[key]->SetVolume(aVolume);
		}
	}
}

void AudioManager::SetSFXVolume(float aVolume)
{
	mySFXVolume = aVolume;
	for (auto const& [key, val] : myLibrary.myAudioList)
	{
		if (myLibrary.myAudioList[key]->GetLayer() == AudioLayer::SoundEffect)
		{
			myLibrary.myAudioList[key]->SetVolume(aVolume);
		}
	}
}

void AudioManager::AddMusicVolume(float aVolume)
{
	for (auto const& [key, val] : myLibrary.myAudioList)
	{
		if (myLibrary.myAudioList[key]->GetLayer() == AudioLayer::Music)
		{
			myLibrary.myAudioList[key]->AddVolume(aVolume);
		}
	}
}

void AudioManager::AddSFXVolume(float aVolume)
{
	for (auto const& [key, val] : myLibrary.myAudioList)
	{
		if (myLibrary.myAudioList[key]->GetLayer() == AudioLayer::SoundEffect)
		{
			myLibrary.myAudioList[key]->AddVolume(aVolume);
		}
	}
}

void AudioManager::SetSoundVolume(AudioList aSound, const float& aVolume)
{
	myLibrary.myAudioList[aSound]->SetVolume(aVolume);
}

void AudioManager::SetSoundPosition(AudioList aSound, const VECTOR2F& aPosition)
{
	myLibrary.myAudioList[aSound]->SetPosition(aPosition);
}

void AudioManager::Fade(const float& aDeltaTime)
{
	if (myFades.size() > 0)
	{
		for (int i = 0; i < myFades.size(); ++i)
		{
			if (myLibrary.myAudioList[myFades[i]]->Fade(aDeltaTime))
			{
				myFades.erase(myFades.begin() + i);
			}
		}
	}
}

void AudioManager::FadeOut(AudioList aSound)
{
	if (!myLibrary.myAudioList[aSound]->GetIsFading())
	{
		myFades.push_back(aSound);
	}
	myLibrary.myAudioList[aSound]->SetFade(true, true);
}

void AudioManager::FadeIn(AudioList aSound)
{
	if (!myLibrary.myAudioList[aSound]->GetIsFading())
	{
		myFades.push_back(aSound);
	}
	myLibrary.myAudioList[aSound]->SetFade(true, false);
}

float AudioManager::GetMusicVolume() const
{
	return myMusicVolume;
}

float AudioManager::GetSFXVolume() const
{
	return mySFXVolume;
}

//void AudioManager::PlayMusic(const std::string & anAudioPath, float aVolume, bool aShouldLoop)
//{
//	Tga2D::AudioOut::Handle channel = {};
//
//	if (!IsPlaying(anAudioPath))
//	{
//		const float volume = myMusicVolume * aVolume;
//		StopCurrentMusic();
//
//		if (Utils::RandomInt(0, 100) == 100)
//		{
//			myAudioOut->PlayMusic("Sounds/Music/GameMusic.mp3", aShouldLoop, channel);
//		}
//		else
//		{
//			myAudioOut->PlayMusic(anAudioPath, aShouldLoop, channel);
//		}
//		
//		myAudioOut->SetVolume(channel, volume);
//	}
//
//	if (aShouldLoop)
//	{
//		BASS_ChannelFlags(channel, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP);
//	}
//}

void AudioManager::PlayAudio(AudioList aSound)
{
	myLibrary.myAudioList[aSound]->Play();
	//Tga2D::AudioOut::Handle channel;

	//const float volume = mySFXVolume * aVolume;
	//myAudioOut->Play(anAudioPath, false, channel);

	//myAudioOut->SetVolume(channel, volume);

	//if (aShouldLoop)
	//{
	//	BASS_ChannelFlags(channel, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP);
	//}
}

void AudioManager::PlayIfAvailable(AudioList aSound)
{
	myLibrary.myAudioList[aSound]->PlayIfAvailable();
}

void AudioManager::Stop(AudioList aSound)
{
	myLibrary.myAudioList[aSound]->Stop();
	//myAudioOut->Stop(anAudioPath, true);
}

//bool AudioManager::IsPlaying(const std::string & anAudioPath)
//{
//	return Tga2D::audio_helpers::IsNowPlaying(*myAudioOut, anAudioPath);
//}
//
//void AudioManager::StopAll(bool anOnlyRepeating)
//{
//	Tga2D::audio_helpers::StopAllNowPlaying(*myAudioOut, anOnlyRepeating);
//}

void AudioManager::StopCurrentMusic()
{
	myAudioOut->StopMusic(true);
}

void AudioManager::StopAllSounds(bool anAndMusic)
{
	//if (anAndMusic)
	//{
	//	StopCurrentMusic();
	//}

	for (auto const& [key, val] : myLibrary.myAudioList)
	{
		if (myLibrary.myAudioList[key]->GetLayer() == AudioLayer::SoundEffect)
		{
			val->Stop();
		}
	}
}

void AudioManager::LockAudio(AudioList anAudio)
{
	myLibrary.myAudioList[anAudio]->Lock();
}

void AudioManager::UnLockAudio(AudioList anAudio)
{
	myLibrary.myAudioList[anAudio]->UnLock();
}

void AudioManager::AddComponentToListen(AudioComponent* aComponent)
{
	switch (aComponent->myAudio)
	{
	case AudioList::MovingPlatform:
		PlayIfAvailable(AudioList::MovingPlatform);
		myPlatformComponents.push_back(aComponent);
		break;
	case AudioList::EnemyNormalIdle:
		PlayIfAvailable(AudioList::EnemyNormalIdle);
		myEnemyComponents.push_back(aComponent);
		break;
	}
	//myComponentVolumes.Enqueue(aComponent->myVolume);
}

void AudioManager::ComponentUpdate()
{
	if (myPlatformComponents.size() > 1)
	{
		std::sort(myPlatformComponents.begin(), myPlatformComponents.end(), CompareVolumes);
	}
	if (myPlatformComponents.size() > 0)
	{
		SetSoundVolume(myPlatformComponents[0]->myAudio, myPlatformComponents[0]->myVolume);
	}
	if (myEnemyComponents.size() > 1)
	{
		std::sort(myEnemyComponents.begin(), myEnemyComponents.end(), CompareVolumes);
	}
	if (myEnemyComponents.size() > 0)
	{
		SetSoundVolume(myEnemyComponents[0]->myAudio, myEnemyComponents[0]->myVolume);
	}
}

void AudioManager::RemoveomponentToListen(AudioComponent* aComponent)
{
	switch (aComponent->myAudio)
	{
	case AudioList::MovingPlatform:
		for (int i = 0; i < myPlatformComponents.size(); ++i)
		{
			if (myPlatformComponents[i] == aComponent)
			{
				myPlatformComponents.erase(myPlatformComponents.begin() + i);
				break;
			}
		}
		if (myPlatformComponents.size() == 0)
		{
			Stop(AudioList::MovingPlatform);
		}
		break;
	case AudioList::EnemyNormalIdle:
		for (int i = 0; i < myEnemyComponents.size(); ++i)
		{
			if (myEnemyComponents[i] == aComponent)
			{
				myEnemyComponents.erase(myEnemyComponents.begin() + i);
				break;
			}
		}
		if (myEnemyComponents.size() == 0)
		{
			Stop(AudioList::EnemyNormalIdle);
		}
		break;
	}
	//CommonUtilities::MinHeap<AudioComponent*> tempHeap;
	//while (myComponents.GetSize() > 0)
	//{
	//	if (myComponents.GetTop() != aComponent)
	//	{
	//		tempHeap.Enqueue(myComponents.GetTop());
	//	}
	//	myComponents.Dequeue();
	//}
	//while (tempHeap.GetSize() > 0)
	//{
	//	myComponents.Enqueue(tempHeap.Dequeue());
	//}
}
