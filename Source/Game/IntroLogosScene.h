#pragma once
#include "Scene.h"

class GameObject;
class SpriteComponent;
enum class eIntroLogo;

class IntroLogosScene : public Scene
{
public:
	IntroLogosScene();

	void Load() override;
	void Update(const float &aDeltaTime) override;

	void DisplayLogo(const float aTime);

private:
	GameObject* myLogo;
	std::vector<SpriteComponent*> mySpriteVector;
	float myTimeSinceInited;
	float myTotalLogoTime;

};