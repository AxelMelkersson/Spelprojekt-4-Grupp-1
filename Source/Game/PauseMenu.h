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

	std::unique_ptr<UIObject> myBackground;
	std::unique_ptr<UIObject> myBar;
	std::unique_ptr<UIObject> myFire;
	std::unique_ptr<UIObject> myFire2;
	std::unique_ptr<UIObject> myFire3;
	std::unique_ptr<UIObject> myFireHighlight;
	
	std::unique_ptr<UIObject> myContinueHighlight;
	std::unique_ptr<UIObject> myLevelSelectHighlight;
	std::unique_ptr<UIObject> myMainMenuHighlight;

	std::vector<UIObject*> myHighlights;


	std::vector<UIButton*> myButtons;

	std::vector<int> myTotalCollectibleInfo;
	std::vector<int> myTotalCollectibleInfoCollected;

	std::unique_ptr<UIButton> myContinueBtn;
	std::unique_ptr<UIButton> myOptionsBtn;
	std::unique_ptr<UIButton> myMainMenuBtn;

	std::unique_ptr<UIObject> myTitleString;
	std::unique_ptr<UIText> myCollectibleString;
	std::unique_ptr<UIText> myCollectibleString2;
	std::unique_ptr<UIText> myCollectibleString3;

	std::shared_ptr<InputWrapper> myInput;

	int myMovingIndex;

	bool myMenuActive;
	bool myIsSpeedrun;
	bool mySkipOneUpdate;

	void CheckIndexPress();
	void ActivateMenu();
	void DeactivateMenu();
	void InitTexts();
	void UpdateUIElements(const float& aDeltaTime);
	void CheckActiveAnimations();
};

