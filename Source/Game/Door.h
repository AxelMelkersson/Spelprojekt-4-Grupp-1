#pragma once
#include "GameObject.h"
#include "Animation.hpp"

class AnimationComponent;
class SpriteComponent;
class Scene;
class Button;

class Door : public GameObject
{
public:
	Door(Scene* aLevelScene);

	void Init(const v2f aPosition);
	void Update(const float& aDeltaTime) override;

	void AddButton(const v2f aPosition);
	
private:
	Animation myAnimation[2];

	SpriteComponent* myClosedDoorSprite;
	SpriteComponent* myOpenDoorSprite;
	Button* myButton;

	v2f mySize;

	bool myDoorIsOpen;

};