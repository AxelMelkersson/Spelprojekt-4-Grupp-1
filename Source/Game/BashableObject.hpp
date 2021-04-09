#pragma once
#include "GameObject.h"
#include "Animation.hpp"

class Scene;

class BashableObject : public GameObject
{
public:
	BashableObject(Scene* aLevelScene);
	~BashableObject();

	void Init(const v2f& aPosition, const float& aRadius);

	void Update(const float& aDeltaTime) override;


private:
	Animation myAnimations[1];

};

