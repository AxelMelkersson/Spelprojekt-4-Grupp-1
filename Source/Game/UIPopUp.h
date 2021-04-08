#pragma once
#include "UIObject.h"


class Scene;

class UIPopUp
{
public:
	enum class ePopUpTypes
	{
		Easy,
		Med,
		Hard
	};

	UIPopUp(Scene* aLevelScene);
	void InitPopUp();
	void Update(const float& aDeltaTime);


private:
	Scene* myScene;

	std::unique_ptr<UIObject> myBackground;
	std::unique_ptr<UIObject> myFireEasy;
	std::unique_ptr<UIObject> myFireMed;
	std::unique_ptr<UIObject> myFireHard;

	bool myEasyActive = false;
	bool myMedActive = false;
	bool myHardActive = false;

	float myCurrentTime;
	float myMaxTime;

	void Activate(ePopUpTypes aType);
	void Deactivate();
	
};

