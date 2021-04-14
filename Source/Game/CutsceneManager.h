#pragma once
#include "GameObject.h"

class InputWrapper;

namespace Tga2D
{
	class CVideo;
}

enum class CutsceneType
{
	Intro, Outro
};

class CutsceneManager
{
public:
	~CutsceneManager() = default;

	static CutsceneManager& GetInstance()
	{
		static CutsceneManager cutsceneManager;
		return cutsceneManager;
	}

	void PlayVideo(CutsceneType aCutsceneType);
	void Update(const float& aDeltaTime);
	void Render();
	void StopVideo();
	bool IsPlaying();
private:
	CutsceneManager();
	float myCurrentVideoLength;
	Tga2D::CVideo* myVideo;
	std::shared_ptr<InputWrapper> myInput;
	bool myIsPlaying;
	CutsceneType myType;
};

