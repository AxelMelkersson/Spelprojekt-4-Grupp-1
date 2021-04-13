#pragma once
#include "UIButton.h"
#include "UIObject.h"
#include "UIText.h"

#include <vector>

class InputWrapper;
class Scene;
class SpriteComponent;
class AudioManager;
class TutorialMenu;
class PauseMenu;
class CreditsMenu;

class OptionsMenu
{
public:
	enum class eOptionsMenu
	{
		ScreenSize,
		Sound,
		Credits,
		Tutorial,
		Back,
		ResetGame,
	};

	OptionsMenu(Scene* aLevelScene);
		
	void Init();
	void Update(const float& aDeltaTime);

	void SetActive(const bool aStatement);
	const bool IsOptionsActive() const;

	void DeactivateTutorial();
	void DeactivateCredits();

	void SetOpenedFromPauseMenu(PauseMenu* aPauseMenu);

private:
	Scene* myScene;
	Camera& myCamera;
	AudioManager* myAudioManager;
	PauseMenu* myPauseMenu;

	float myMusicVol;
	float mySFXVol;
	float myMusicStep;
	float myVFXStep;

	UIObject* myBackground;
	UIObject* myTitle;
	UIObject* myOptionsTitle;
	UIObject* myFireHighlight;
	UIObject* myBar;
	UIObject* mySoundSettings;
	UIObject* myBGHighlight;
	UIObject* myVFXHighlight;
	UIObject* myBGDot;
	UIObject* myVFXDot;
	UIObject* myResolutions;
	UIObject* my720pHgh;
	UIObject* my1080pHgh;
	UIObject* my4KHgh;
	UIObject* myScreenSizeDot;

	//Credits
	CreditsMenu* myCreditsMenu;

	//Tutorial
	TutorialMenu* myTutorial;

	UIButton* myTutorialsBtn;
	UIButton* myScreenBtn;
	UIButton* mySoundBtn;
	UIButton* myCreditsBtn;
	UIButton* myBackBtn;
	UIButton* myResetBtn;

	std::vector<UIObject*> myResolutionObj;
	std::vector<UIObject*> mySoundObjects;
	std::vector<UIButton*> myButtons;

	std::shared_ptr<InputWrapper> myInput;

	int myMovingIndex;
	int mySoundMovingIndex;
	int myScreenMovingIndex;

	bool myMenuAcitve;
	bool mySoundSettingsActive = false;

	bool myScreenSettingsActive = false;
	bool myCreditsActive = false;
	bool myTutorialActtive = false;
	bool mySubMenuActive = false;
	
	bool myIsOpenedFromPause;

	void CheckIndexPress(const float& aDeltaTime);
	void ActivateMenu();
	void DeactivateMenu();
	void UpdateUIElements(const float& aDeltaTime);
	void CheckActiveAnimations();
	void InactivateHighlight();
	
};