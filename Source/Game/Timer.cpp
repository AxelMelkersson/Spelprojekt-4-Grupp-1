#include "stdafx.h"
#include "Timer.h"
#include "../External/Headers/CU/Utilities.h"
#include "GameWorld.h"
#include "TextComponent.h"
#include "SpeedrunManager.h"
#include "SpriteComponent.h"

Timer::Timer(Scene* aLevelScene)
	:
	GameObject(aLevelScene),
	myIsActive(false),
	myTime(0.0f),
	myLastTime(0.0f),
	myStartTime(0.0f),
	myTotalTime(0.0f)
{}

void Timer::Init(const v2f aPos)
{
	SpriteComponent* spriteComponent = AddComponent<SpriteComponent>();
	spriteComponent->SetSpritePath("Sprites/UI/popUp/UI_PopUp_84x32px.dds");
	spriteComponent->SetRelativePosition({ 34, 10 });
	spriteComponent->SetSize(v2f(54.0f, 12.0f));

	TextComponent* textComponent = AddComponent<TextComponent>();
	textComponent->CreateText("Text/alagard.ttf", EFontSize::EFontSize_36, 0);
	textComponent->SetRelativePosition(aPos.x, aPos.y);
	textComponent->Activate();
	SetZIndex(200);

}
void Timer::Update(const float& aDeltatime)
{
	GetComponent<TextComponent>()->SetText(CGameWorld::GetInstance()->GetLevelManager().GetSpeedrunManager()->GetTimeOutput(myTime));

	myTime += CGameWorld::GetInstance()->GetTimer()->GetTotalTime() - myLastTime - myStartTime + myTotalTime;
	myLastTime = myTime;

	GameObject::Update(aDeltatime);
}

void Timer::Start(float aStartTime)
{
	myIsActive = true;
	myTime = aStartTime;
	myStartTime = CGameWorld::GetInstance()->GetTimer()->GetTotalTime();
	myLastTime = myStartTime;
	myTotalTime = aStartTime;
}
void Timer::Paus()
{
	myIsActive = false;
}
void Timer::Stop()
{
	myIsActive = false;
	myTime = 0.0f;
}

const float Timer::GetTime() const
{
	return myTime;
}

