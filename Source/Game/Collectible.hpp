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

	Collectible(Scene* aLevelScene);
	~Collectible();

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
	bool myWasCollected;
	bool myWasTurnedIn;
};

