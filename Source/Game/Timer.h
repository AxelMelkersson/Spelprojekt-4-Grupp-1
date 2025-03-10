#pragma once
#include "GameObject.h"

class Camera;

class Timer : public GameObject
{
public:
	Timer(Scene* aLevelScene);

	void Init(const v2f aPos);
	void Update(const float& aDeltatime) override;

	void Start(float aStartTime = 0.0);
	void Paus();
	void Stop();

	const float GetTime() const;

private:
	Camera* myCamera;
	
	v2f myStartPosition;
	bool myIsActive;
	float myTime;
	float myStartTime;
	float myLastTime;
	float myTotalTime;
};