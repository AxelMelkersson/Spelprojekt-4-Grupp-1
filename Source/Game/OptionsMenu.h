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
	float myVFXVol;
	float myMusicStep;
	float myVFXStep;

	std::unique_ptr<UIObject> myBackground;
	std::unique_ptr<UIObject> myTitle;
	std::unique_ptr<UIObject> myOptionsTitle;
	std::unique_ptr<UIObject> myFireHighlight;
	std::unique_ptr<UIObject> myBar;
	std::unique_ptr<UIObject> mySoundSettings;
	std::unique_ptr<UIObject> myBGHighlight;
	std::unique_ptr<UIObject> myVFXHighlight;
	std::unique_ptr<UIObject> myBGDot;
	std::unique_ptr<UIObject> myVFXDot;
	std::unique_ptr<UIObject> myResolutions;
	std::unique_ptr<UIObject> my720pHgh;
	std::unique_ptr<UIObject> my1080pHgh;
	std::unique_ptr<UIObject> my4KHgh;
	std::unique_ptr<UIObject> myScreenSizeDot;

	//Credits
	CreditsMenu* myCreditsMenu;

	//Tutorial
	TutorialMenu* myTutorial;

	std::unique_ptr<UIButton> myTutorialsBtn;
	std::unique_ptr<UIButton> myScreenBtn;
	std::unique_ptr<UIButton> mySoundBtn;
	std::unique_ptr<UIButton> myCreditsBtn;
	std::unique_ptr<UIButton> myBackBtn;
	std::unique_ptr<UIButton> myResetBtn;

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
	void UpdateSoundSettings();
	void InactivateHighlight();
	
};