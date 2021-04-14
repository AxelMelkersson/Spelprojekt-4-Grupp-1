#pragma once
#include "Scene.h"

class WinScene : public Scene
{
public:
	WinScene();
	~WinScene();

	void Load() override;

	void Update(const float& aDeltaTime) override;

private:
	float myFadeTime;

};

