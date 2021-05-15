#pragma once
#include <string>
#include <memory>
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
	void SetComponentVolume(AudioList aSound, const float& aVolume);

	void MuteEverything();
	void UnMuteEverything();

	void Fade(const float& aDeltaTime);

	void FadeOut(AudioList aSound);
	void FadeIn(AudioList aSound);

	float GetMusicVolume() const;
	float GetSFXVolume() const;

	void PlayAudio(AudioList aSound);
	void PlayIfAvailable(AudioList aSound);

	void Stop(AudioList aSound);
	void ForceStop(AudioList aSound);

	void StopAllSounds(bool anAndMusic = false);

	void LockAudio(AudioList anAudio);
	void UnLockAudio(AudioList anAudio);

	void AddComponentToListen(AudioComponent* aComponent);
	void RemoveomponentToListen(AudioComponent* aComponent);

private:
	void ComponentUpdate();
	void StopCurrentMusic();

	std::unique_ptr<Tga2D::AudioOut> myAudioOut;
	std::vector<AudioComponent*> myPlatformComponents;
	std::vector<AudioComponent*> myEnemyComponents;
	std::vector<AudioList> myFades;
	float myMusicVolume = 0.2;
	float mySFXVolume = 0.5;

	AudioLibrary myLibrary = AudioLibrary();

};