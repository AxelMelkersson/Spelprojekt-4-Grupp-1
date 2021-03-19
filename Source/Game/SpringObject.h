#pragma once
#include "GameObject.h"
#include "Animation.hpp"

class Player;

class SpringObject : public GameObject
{
public:
	SpringObject(Scene* aLevelScene);

	void Init(const v2f aPosition);
	void Update(const float& aDeltaTime) override;

	virtual void OnCollision(GameObject* aGameObject) override;

private:
#ifdef _DEBUG
	void ImGuiUpdate();
#endif // _DEBUG

	v2f myPosition;
	v2f mySize;

	bool myActiveSpring;

	float myVelocityForce;
	float myRetardation;
	float myTimer;
	float mySpringTimerCooldown;

	void InitSprings(const v2f aPosition);
	void CreateGroundSpring();

	Animation myAnimations[3];
};
