#include "stdafx.h"
#include "AudioClip.h"
#include "tga2d/audio/audio.h"
#include "tga2d/audio/audio_out.h"
#include "AudioLibrary.h"
#include "AudioManager.h"
#include <iostream>

AudioClip::AudioClip(const char* anAudioPath, const bool aIsLooping, const float& aVolume, const float& aMaxVol, AudioLayer aLayer) :
	myMaxVolume(aMaxVol), myVolProcent(aMaxVol / 100.0f)
{
	myAudio = new Tga2D::CAudio();
	myVolume = aVolume;
	myAudio->Init(anAudioPath, aIsLooping);
	myLayer = aLayer;
	myAudio->SetVolume(myVolume);
	myIsPlaying = false;
}

AudioClip::~AudioClip()
{
	myAudio = nullptr;
	delete myAudio;
}

void AudioClip::SetVolume(const float& aVolChange)
{
	myVolume = GetVolPercentage(aVolChange);
	if (myVolume < 0)
	{
		myVolume = 0;
	}
	if (myVolume > myMaxVolume)
	{
		myVolume = myMaxVolume;
	}
	myAudio->SetVolume(myVolume);
}

void AudioClip::SetComponentVolume(const float& aVolChange)
{
	myVolume = GetVolPercentage(aVolChange) * AudioManager::GetInstance()->GetSFXVolume();
	if (myVolume < 0)
	{
		myVolume = 0;
	}
	if (myVolume > myMaxVolume)
	{
		myVolume = myMaxVolume;
	}
	myAudio->SetVolume(myVolume);
}

void AudioClip::AddVolume(const float& aVolChange)
{
	myVolume += aVolChange;
	if (myVolume < 0)
	{
		myVolume = 0;
	}
	if (myVolume > 1)
	{
		myVolume = 1;
	}
	myAudio->SetVolume(myVolume);
}

void AudioClip::SetPosition(const VECTOR2F aPosition)
{
	//myAudio->SetPosition(aPosition / 10);
}

void AudioClip::Play()
{
	if (myCanPlay)
	{
		myAudio->Play();
		myIsPlaying = true;
	}
}

void AudioClip::PlayIfAvailable()
{
	if (!myIsPlaying && myCanPlay)
	{
		myAudio->Play();
		myIsPlaying = true;
	}
}

void AudioClip::Mute()
{
	//myAudio->Stop(true);
	myAudio->SetVolume(0);
}

void AudioClip::UnMute()
{
	myAudio->SetVolume(myVolume);
}

void AudioClip::Stop()
{
	if (myIsFading) return;
	myAudio->Stop();
	myIsPlaying = false;
	UnLock();
}

void AudioClip::Lock()
{
	myCanPlay = false;
}

void AudioClip::UnLock()
{
	myCanPlay = true;
}

bool AudioClip::Fade(const float& aDeltaTime)
{
	if (myIsFadingOut)
	{
		if (myVolume > 0)
		{
			myVolume -= aDeltaTime / 10.0f;
			myAudio->SetVolume(myVolume);
		}
		else
		{
			myIsFading = false;
			Stop();
			return true;
		}
	}
	else
	{
		PlayIfAvailable();
		if (myLayer == AudioLayer::SoundEffect)
		{
			if (myVolume < GetVolPercentage(AudioManager::GetInstance()->GetSFXVolume()))
			{
				myVolume += aDeltaTime / 5.0f;
				myAudio->SetVolume(myVolume);
			}
			else
			{
				myIsFading = false;
				return true;
			}
		}
		else
		{
			if (myVolume < GetVolPercentage(AudioManager::GetInstance()->GetMusicVolume()))
			{
				myVolume += aDeltaTime / 5.0f;
				myAudio->SetVolume(myVolume);
			}
			else
			{
				myIsFading = false;
				return true;
			}
		}
	}
	return false;
}

void AudioClip::SetFade(const bool& aFade, const bool& aOut)
{
	myIsFading = aFade;
	myIsFadingOut = aOut;
}

const bool AudioClip::GetIsFading()
{
	return myIsFading;
}

AudioLayer AudioClip::GetLayer()
{
	return myLayer;
}

float AudioClip::GetVolPercentage(const float& aVolume)
{
	return myVolProcent * (aVolume * 100);
}
