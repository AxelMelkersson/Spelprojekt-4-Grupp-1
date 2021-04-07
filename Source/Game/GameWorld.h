#pragma once
#include "MainMenuScene.h"
#include "LevelSelect.hpp"
#include "LevelScene.h"
#include "IntroLogosScene.h"
#include "WinScene.hpp"
#include "LevelManager.hpp"
#include "SpeedrunScene.h"

namespace Utils
{
	class Timer;
	class Input;
}

class CGame;
class InputWrapper;

class CGameWorld
{
public:
	friend CGame;

	inline static CGameWorld* GetInstance()
	{
		return ourInstance;
	}

	CGameWorld(CGame* aGame); 
	~CGameWorld();

	void Init();
	void Update(); 
	void Render();

	inline const float& DeltaTime() const
	{
		return myDeltaTime;
	}
	inline const float& TotalTime() const
	{
		return myTotalTime;
	}
	inline std::shared_ptr<InputWrapper> Input()
	{
		return myInput;
	}
	inline Utils::Timer* GetTimer()
	{
		return myTimer.get();
	}
	inline CGame* Game()
	{
		return myGame;
	}

	inline LevelManager& GetLevelManager()
	{
		return myLevelManager;
	}

	void LoadDebugger();

private:
	/*** System ***/
	std::unique_ptr<Utils::Timer> myTimer;
	std::shared_ptr<InputWrapper> myInput;
	CGame* myGame;
	float myDeltaTime;
	float myTotalTime;
	static CGameWorld* ourInstance;

	/*** Scenes ***/
	IntroLogosScene myIntroLogosScene;
	MainMenuScene myMainMenuScene;
	LevelSelect myLevelSelect;
	LevelScene myLevelScene;
	WinScene myWinScene;
	SpeedrunScene mySpeedrunScene;

	LevelManager myLevelManager;

};
