#pragma once
#include "Scene.h"
#include "Animation.hpp"

class InputWrapper;
class UIObject;
class UIButton;
class OptionsMenu;

class MainMenuScene : public Scene
{
public:
    enum class eMainMenuButton
    {
        StartGame,
        LevelSelect,
        SpeedrunMode,
        Options,
        ExitGame
    };

    MainMenuScene();

    void Load() override;

    void Activate() override;
    void Deactivate() override;

    void Update(const float& aDeltaTime) override;


private:
    UIObject* myBackground;
    UIObject* myTitleSprite;
    UIObject* myFireHighlight;

    std::vector<UIButton*> myButtons;

   UIButton* myNewGameBtn;
   UIButton* myLevelSelectBtn;
   UIButton* mySpeedrunModeBtn;
   UIButton* myOptionsBtn;
   UIButton* myExitGameBtn;

    std::shared_ptr<InputWrapper> myInput;

    bool mySubMenuActive = false;
    bool myLevelSelectUnlocked;
    bool mySpeedrunUnlocked;

    int myMovingIndex;

    void InitObjects();
    void UpdateObjects(const float& aDeltaTime);
    void CheckButtonsPress();

    void SetActiveMenu(const bool aStateBool);
    void SetBackgroundActive(const bool aStateBool);
    void CheckActiveAnimations();

    void InitLogoAnimations();
    void LogoAnimation();

    Animation myLogoAnimations[4];

    int myLogoTypeTimes[4];
    int myLogoAnimationIndex;
    int myCurrentLogoTypeTimes;

    OptionsMenu* myOptions;
};

