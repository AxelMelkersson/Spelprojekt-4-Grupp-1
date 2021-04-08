#pragma once

namespace Tga2D
{
	class AudioOut;
	class CAudio;
}

enum class AudioLayer;

class AudioClip
{
public:
	AudioClip() = default;
	AudioClip(const char* anAudioPath, const bool aIsLooping, const float& aVolume, const float& aMaxVol, AudioLayer aLayer);
	~AudioClip();

	void SetVolume(const float& aVolChange);
	void AddVolume(const float& aVolChange);
	void SetPosition(const VECTOR2F aPosition);

	void Play();
	void PlayIfAvailable();
	void Stop();

	void Mute();
	void UnMute();

	void Lock();
	void UnLock();

	bool Fade(const float& aDeltaTime);
	void SetFade(const bool& aFade, const bool& aOut);
	const bool GetIsFading();

	AudioLayer GetLayer();

private:
	float GetVolPercentage(const float& aVolume);

	float myVolume = 0.0f;
	const float myMaxVolume;
	const float myVolProcent;
	bool myCanPlay = true;
	bool myIsPlaying = false;
	bool myIsFadingOut = false;
	bool myIsFading = false;
	AudioLayer myLayer;
	Tga2D::CAudio* myAudio = nullptr;
};