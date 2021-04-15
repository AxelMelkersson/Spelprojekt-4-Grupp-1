#pragma once
#include "GameObject.h"
#include "Animation.hpp"
#include "Subscriber.hpp"

class Jesus : public GameObject, public Subscriber
{
public:
	Jesus(Scene* aLevelScene);
	~Jesus();

	void Init(const v2f& aPosition);
	void InitAnimations();

	void AnimationStates();

	void Update(const float& aDeltaTime) override;

	void Move(const float& aDeltaTime);

	void SetTarget(GameObject* aTarget);

	void OnCollision(GameObject* aGameObject) override;

	void Reset();

	void Notify(const Message& aMessage) override;

	void UpdateImGui();

private:
	Animation myAnimations[3];

	GameObject* myTarget;

	v2f myDirection;
	v2f mySpawnPosition;

	float myTurnSpeed;

	float myAcceleration;
	float myRetardation;

	float myCurrentSpeed;
	float myMaxSpeed;
	float myMinSpeed;

	float myMaxTurnBeforeRetardation;

	float myCloseDistance;

	int myCurrentAnimationIndex;

	bool myEatTarget;

};

