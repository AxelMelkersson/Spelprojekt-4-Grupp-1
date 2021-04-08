#pragma once
#include "GameObject.h"
#include "Animation.hpp"

class AnimationComponent;
class SpriteComponent;
class LevelScene;

class Button : GameObject
{
public:
	Button(Scene* aLevelScene);

	void Init(const v2f myStartingPosition, const v2f myPositionFromStart);
	void Update(const float& aDeltaTime) override;

	virtual void OnCollision(GameObject* aGameObject) override;
	bool GetActiveButton();

private:
	Animation myAnimation[2];

	v2f myPosition;
	v2f mySize;

	bool myButtonActive;
	bool myHasCollided;

	void InitButton(const v2f myStartingPosition, const v2f myPositionFromStart);

};