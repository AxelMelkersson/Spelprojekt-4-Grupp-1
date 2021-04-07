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

	std::unique_ptr<UIObject> myTitle;
	std::unique_ptr<UIObject> myBackground2;
	std::unique_ptr<UIObject> myBackground;
	std::unique_ptr<UIObject> myBar;
	std::unique_ptr<UIObject> myBestTime;
	std::unique_ptr<UIObject> myFireHighlight;

	std::unique_ptr<UIButton> myStartGameBtn;
	std::unique_ptr<UIButton> myMainMenuBtn;

	std::array<std::unique_ptr<UIText>, 10> myHighscoreList;

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

