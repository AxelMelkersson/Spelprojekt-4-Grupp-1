#pragma once
#include "GameObject.h"
#include "Subscriber.hpp"
#include "Animation.hpp"

class Scene;

namespace Utils
{
	class Timer;
}

class Collectible : public GameObject, public Subscriber
{
public:
	enum class eCollectibleType
	{
		Easy,
		Medium,
		Hard
	};

	Collectible(Scene* aLevelScene, const unsigned int anID, const unsigned int aBonfireID);

	void Init(const v2f& aPosition, eCollectibleType aType);
	void Update(const float& aDeltaTime) override;

	void OnCollision(GameObject* aGameObject) override;
	void Reset();
	void TurnIn();

	void Notify(const Message& aMessage) override;

	void ImGuiUpdate();

private:
	Animation myAnimations[2];

	v2f mySpawnPosition;
	v2f myTargetPosition;
	GameObject* myTarget;

	float mySpeed;
	float myMinRadiusFromTarget;
	float myIdleMovementSpeed;
	float myIdleMovementDistance;
	float myTimeOffset;

	eCollectibleType myType;
	bool myWasCollectedBefore;
	bool myWasCollected;
	bool myWasTurnedIn;
	bool myWasTurnedInWhenEnteringDoor;

	const unsigned int myID;
	const unsigned int myBonfireID;

	const void CheckPopUpMessages();
	const void ActivateTrailEffect();
	const void ActivateCollectedEffect();

};