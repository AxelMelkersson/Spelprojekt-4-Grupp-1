#pragma once
#include "GameObject.h"
#include "Animation.hpp"
#include "SpriteComponent.h"

class Scene;

class BashableObject : public GameObject
{
public:
	BashableObject(Scene* aLevelScene);

	void Init(const v2f& aPosition, const float& aRadius);
	void Update(const float& aDeltaTime) override;

	void Highlight();
	void Dehighlight();

private:
	Animation myAnimations[2];
	SpriteComponent* myHighlight;
	SpriteComponent* myIdle;

};