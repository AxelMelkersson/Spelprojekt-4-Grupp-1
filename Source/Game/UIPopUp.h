#pragma once
#include "UIObject.h"
#include "Subscriber.hpp"

class Scene;
class UIText;

class UIPopUp
	: public Subscriber
{
public:
	enum class ePopUpTypes
	{
		Easy,
		Med,
		Hard
	};

	UIPopUp(Scene* aLevelScene);
	~UIPopUp();
	void InitPopUp();
	void Update(const float& aDeltaTime);
	void Activate(ePopUpTypes aType);
	void Notify(const Message& aMessage) override;

private:
	Scene* myScene;

	std::unique_ptr<UIObject> myBackground;
	std::unique_ptr<UIObject> myFireEasy;
	std::unique_ptr<UIObject> myFireMed;
	std::unique_ptr<UIObject> myFireHard;

	std::unique_ptr<UIText> myCollectibleString;
	std::unique_ptr<UIText> myCollectibleString2;
	std::unique_ptr<UIText> myCollectibleString3;


	std::vector<UIText*> myLevelCollectibles;
	std::vector<std::vector<int>> myCollectibleInfo;
	std::vector<std::vector<int>> myCollectibleCollected;

	bool myEasyActive = false;
	bool myMedActive = false;
	bool myHardActive = false;
	bool myIsMaxLeft = false;

	float myBackXPos;
	float myCurrentTime;
	float myCurrentStayTime;
	float myMaxTime;
	float myStayTime;

	void Deactivate();
	void SetNewPositions(const float& aDeltaTime);
	void ResetObjects();
	void UpdateCollectibles();
};

