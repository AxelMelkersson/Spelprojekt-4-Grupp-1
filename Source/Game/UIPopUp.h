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

	UIObject* myBackground;

	std::vector<UIObject*> myFires;

	UIText* myCollectibleString;

	std::vector<int> myLevelIndexes;
	std::vector<UIText*> myLevelCollectibles;
	
	std::vector<int> myCollectibleInfo;
	std::vector<int> myCollectibleCollected;

	std::vector<int> myShowQueue;

	bool myIsMaxLeft = false;

	float myCurrentTime;
	float myCurrentStayTime;

	float myMaxTime;
	float myStayTime;

	void Deactivate(ePopUpTypes aType);
	void SetNewPositions(const float& aDeltaTime);
	void UpdateCollectibles();
};
