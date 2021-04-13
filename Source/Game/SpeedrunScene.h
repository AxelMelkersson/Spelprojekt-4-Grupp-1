#pragma once
#include "Scene.h"

#include <array>

class UIObject;
class UIButton;
class UIText;
class InputWrapper;

class SpeedrunScene : public Scene
{
public:
	enum class eSpeedRunButton
	{
		StartGame,
		MainMenu
	};
	SpeedrunScene();

	
	void Load() override;
	void Update(const float& aDeltaTime) override;

	void Activate() override;
	void Deactivate() override;


private:
	std::shared_ptr<InputWrapper> myInput;

	UIObject* myTitle;
	UIObject* myBackground2;
	UIObject* myBackground;
	UIObject* myBar;
	UIObject* myBestTime;
	UIObject* myFireHighlight;

	UIButton* myStartGameBtn;
	UIButton* myMainMenuBtn;

	std::array<UIText*, 10> myHighscoreList;

	std::vector<UIButton*> myButtons;

	bool myIsInMenu;

	int myMovingIndex;

	void InitObjects();
	void InitHighscores();
	void CheckButtonPress();
	void UpdateObjects(const float& aDeltaTime);
	void SetActiveMenu(const bool aStateBool);
	void CheckActiveanimations();

};

