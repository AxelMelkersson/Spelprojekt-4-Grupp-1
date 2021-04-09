#pragma once
#include "GameObject.h"

class Timer : public GameObject
{
public:
	Timer(Scene* aLevelScene);
	void Init(const v2f aPos);

	void Start(float aStartTime = 0.0);
	void Paus();
	void Stop();
	float GetTime() const { return myTime; }

	void Update(const float& aDeltatime) override;

private:
	bool myIsActive;
	float myTime;
	float myStartTime;
	float myLastTime;
	float myTotalTime;
};

