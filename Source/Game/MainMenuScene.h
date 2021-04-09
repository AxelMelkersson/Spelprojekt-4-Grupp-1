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
    std::unique_ptr<UIObject> myBackground;
    std::unique_ptr<UIObject> myTitleSprite;
    std::unique_ptr<UIObject> myFireHighlight;

    std::vector<UIButton*> myButtons;

    std::unique_ptr<UIButton> myNewGameBtn;
    std::unique_ptr<UIButton> myLevelSelectBtn;
    std::unique_ptr<UIButton> mySpeedrunModeBtn;
    std::unique_ptr<UIButton> myOptionsBtn;
    std::unique_ptr<UIButton> myExitGameBtn;

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

