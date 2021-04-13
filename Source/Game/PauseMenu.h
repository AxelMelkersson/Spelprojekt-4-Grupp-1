#pragma once
#include "GameObject.h"
#include "UIButton.h"

#include "UIObject.h"
#include "Animation.hpp"

#include "UIText.h"

#include "OptionsMenu.h"

#include "Subscriber.hpp"

class SpriteComponent;
class InputWrapper;
class Scene;
class Animation;

class PauseMenu : Subscriber
{
public:
	PauseMenu(Scene* aLevelScene);
	~PauseMenu();

	void InitMenu();

	void Update(const float& aDeltaTime);

	void SetActiveMenu(const bool aStatement);
	bool IsPauseActive();
	bool GetOptionsIsActive();
	void SelectButton();
	void SkipOneUpdate();

	void Notify(const Message& aMessage) override;

private:
	Scene* myScene;
	Camera& myCamera;
	Animation myAnimation[1];

	OptionsMenu* myOptionsMenu;

	UIObject* myBackground;
	UIObject* myBar;
	UIObject* myFire;
	UIObject* myFire2;
	UIObject* myFire3;
	UIObject* myFireHighlight;
	
	UIObject* myContinueHighlight;
	UIObject* myLevelSelectHighlight;
	UIObject* myMainMenuHighlight;

	std::vector<UIObject*> myHighlights;


	std::vector<UIButton*> myButtons;

	std::vector<int> myTotalCollectibleInfo;
	std::vector<int> myTotalCollectibleInfoCollected;

	UIButton* myContinueBtn;
	UIButton* myOptionsBtn;
	UIButton* myMainMenuBtn;

	UIObject* myTitleString;
	UIText* myCollectibleString;
	UIText* myCollectibleString2;
	UIText* myCollectibleString3;

	std::shared_ptr<InputWrapper> myInput;

	int myMovingIndex;

	bool myMenuActive;
	bool myIsSpeedrun;
	bool mySkipOneUpdate;

	void CheckIndexPress();
	void ActivateMenu();
	void DeactivateMenu();
	void InitTexts();
	void UpdateCollectibleInfo(const bool aIniting);
	void UpdateUIElements(const float& aDeltaTime);
	void CheckActiveAnimations();
};

