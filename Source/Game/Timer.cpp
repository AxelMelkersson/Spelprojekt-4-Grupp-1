#include "stdafx.h"
#include "Timer.h"
#include "../External/Headers/CU/Utilities.h"
#include "GameWorld.h"
#include "TextComponent.h"
#include <assimp\StringUtils.h>

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
	TextComponent* textComponent = AddComponent<TextComponent>();
	textComponent->CreateText("Text/alagard.ttf", EFontSize::EFontSize_36, 0);
	textComponent->SetRelativePosition(aPos.x, aPos.y);
	int time = myTime;
	textComponent->SetText(to_string(time));
	textComponent->Activate();
	SetZIndex(200);
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

void Timer::Update(const float& aDeltatime)
{
	float time = floorf(myTime * 100) / 100;

	GetComponent<TextComponent>()->SetText(to_string(time));

	myTime += CGameWorld::GetInstance()->GetTimer()->GetTotalTime() - myLastTime - myStartTime + myTotalTime;
	myLastTime = myTime;

	GameObject::Update(aDeltatime);
}
