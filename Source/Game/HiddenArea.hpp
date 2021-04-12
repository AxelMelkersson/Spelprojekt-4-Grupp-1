#pragma once
#include "GameObject.h"

class SpritebatchComponent;
class Scene;

class HiddenArea : public GameObject
{
public:
	HiddenArea(Scene* aLevelScene, const v2f& aPosition, const v2f& aSize);
	~HiddenArea();

	void Update(const float& aDeltaTime) override;

	void SetBatch(SpritebatchComponent* aHiddenSpriteBatch);

	void OnCollision(GameObject* aGameObject) override;

private:
	SpritebatchComponent* myHiddenSpriteBatch;
	
	float myOpacity;
	float myOpacityChangeSpeed;
	float myTimer;

	bool myPlayerCollided;
	bool myHasBeenFound;

};

