#pragma once

#include <string>
#include <memory>
#include <assert.h>
#include "AudioLibrary.h"

namespace Tga2D
{
	class AudioOut;
}

class AudioComponent;

class AudioManager
{
public:
	AudioManager() = default;
	~AudioManager() = default;

	const static std::unique_ptr<AudioManager>& GetInstance();

	void Init();
	void Update(const float& aDeltaTime);

	void SetMusicVolume(float aVolume);
	void SetSFXVolume(float aVolume);
	void AddMusicVolume(float aVolume);
	void AddSFXVolume(float aVolume);

	void SetSoundVolume(AudioList aSound, const float& aVolume);
	void SetSoundPosition(AudioList aSound, const VECTOR2F& aPosition);

	void Fade(const float& aDeltaTime);

	void FadeOut(AudioList aSound);
	void FadeIn(AudioList aSound);

	float GetMusicVolume() const;
	float GetSFXVolume() const;

	void PlayAudio(AudioList aSound);
	void PlayIfAvailable(AudioList aSound);

	void Stop(AudioList aSound);

	//bool IsPlaying(const std::string& anAudioPath);

	void StopAllSounds(bool anAndMusic = false);

	void LockAudio(AudioList anAudio);
	void UnLockAudio(AudioList anAudio);

	void AddComponentToListen(AudioComponent* aComponent);
	void RemoveomponentToListen(AudioComponent* aComponent);

private:

	void ComponentUpdate();
	std::unique_ptr<Tga2D::AudioOut> myAudioOut;
	std::vector<AudioComponent*> myPlatformComponents;
	std::vector<AudioComponent*> myEnemyComponents;
	std::vector<AudioList> myFades;
	float myMusicVolume = 1;
	float mySFXVolume = 1;

	void StopCurrentMusic();

	AudioLibrary myLibrary = AudioLibrary();
};