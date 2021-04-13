#pragma once
#include "UIObject.h"
#include "Subscriber.hpp"

class Scene;
class UIText;

class UIPopUp
	: public Subscriber, public GameObject
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
	void Update(const float& aDeltaTime) override;
	void Activate(ePopUpTypes aType);
	void Notify(const Message& aMessage) override;

private:
	Scene* myScene;

	std::unique_ptr<UIObject> myBackgroundE;
	std::unique_ptr<UIObject> myBackgroundM;
	std::unique_ptr<UIObject> myBackgroundH;

	std::unique_ptr<UIObject> myFireEasy;
	std::unique_ptr<UIObject> myFireMed;
	std::unique_ptr<UIObject> myFireHard;

	std::unique_ptr<UIText> myCollectibleString;
	std::unique_ptr<UIText> myCollectibleString2;
	std::unique_ptr<UIText> myCollectibleString3;

	std::vector<int> myLevelIndexes;
	std::vector<UIText*> myLevelCollectibles;
	std::vector<int> myCollectibleInfo;
	std::vector<int> myCollectibleCollected;

	bool myEasyActive = false;
	bool myMedActive = false;
	bool myHardActive = false;
	bool myIsMaxLeft = false;
	bool myIsMMaxLeft = false;
	bool myIsHMaxLeft = false;

	float myBackXPos;
	float myCurrentTime;
	float myCurrentMTime;
	float myCurrentHTime;
	float myCurrentStayTime;
	float myCurrentMStayTime;
	float myCurrentHStayTime;

	float myMaxTime;
	float myStayTime;


	int myLevelIndex;

	void Deactivate(ePopUpTypes aType);
	void SetNewPositions(const float& aDeltaTime);
	void SetNewMedPositions(const float& aDeltaTime);
	void SetNewHardPositions(const float& aDeltaTime);
	void UpdateCollectibles();
};
