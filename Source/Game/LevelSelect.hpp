#pragma once
#include "Scene.h"

class InputWrapper;
class UIButton;
class UIObject;
class UIText;

class LevelSelect : public Scene
{
public:
	LevelSelect();
	~LevelSelect();

	void Load() override;

	void Update(const float& aDeltaTime) override;

private:
	void CheckButtonPress();
	void InitiateButtons();

	void ShowArea(const int& aIndex);

	void InitCollectibles();
	void InitTexts();

	void UpdateLevelCollectibles();

	std::vector<UIObject*> myAreaTexts;
	std::vector<UIText*> myLevelCollectibles;
	std::vector<UIButton*> myLevelButtons;
	std::vector<int> myAreaIndexes;
	std::vector<int> myLevelIndexes;

	std::vector<int> myTotalCollectibleInfo;
	std::vector<int> myTotalCollectibleInfoCollected;

	std::vector<std::vector<int>> myCollectibleInfo;
	std::vector<std::vector<int>> myCollectibleInfoCollected;

	UIButton* myBackButton;
	UIObject* myBackButtonFire;

	std::shared_ptr<InputWrapper> myInput;

	int myLevelIndex;
	
	bool myIsSelectingLevel;
	bool myLevelWasStarted;

};

