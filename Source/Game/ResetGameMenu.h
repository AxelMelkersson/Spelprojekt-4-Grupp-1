#pragma once
#include "GameObject.h"
#include "UIButton.h"
#include "UIObject.h"
#include "UIText.h"
#include <vector>

class Scene;

class ResetGameMenu :
    public GameObject
{
public:
    ResetGameMenu(Scene* aScene);
    ~ResetGameMenu() = default;



private:
    UIButton* myYesBtn;
    UIButton* myNoBtn;
    std::vector<UIButton*> myResetButtons;
};

