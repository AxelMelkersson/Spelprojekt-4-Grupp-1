#pragma once
#include "Scene.h"
#include <vector>
#include "Platform.h"
#include "BackGround.h"
#include "SpringObject.h"
#include "PauseMenu.h"
#include "Timer.h" 
#include "ParticleEffectFactory.h"
#include "UIPopUp.h"

#include "Subscriber.hpp"

class GameObject;

class LevelScene :
    public Scene, public Subscriber
{
public:
    LevelScene();

    void Load() override;
    void Unload() override;

    void Activate() override;
    void Deactivate() override;

    void Update(const float& aDeltaTime) override;

    void AddBlackScreen();
    void DecreaseBlackScreen();
    void IncreaseBlackScreen(const float& aOpacitySpeedFactor);
    const bool GetReachedFullOpacity();
    void Transitioning();

    GameObject* GetPlayer();
    ParticleEffectFactory& GetEffectFactory();
    Background& GetBackground();

    void Notify(const Message& aMessage) override;

private:
    GameObject* myBlackScreen;
    GameObject* myPlayer;
    Background* myBackground;
    PauseMenu* myPauseMenu;
    Timer* myTimer;
    ParticleEffectFactory* myEffectFactory;
    UIPopUp* myPopUp;

    float myBlackScreenOpacity;
    float myBlackScreenOpacitySpeed;
    float myStayBlackTime;

    bool myReachedFullOpacity;
    bool myIsTransitioning;
    bool myIsSpeedrun;

    int myShowPopUp = -1;
};

