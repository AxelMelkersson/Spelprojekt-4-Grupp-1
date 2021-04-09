#pragma once
#include "GameObject.h"
#include "Animation.hpp"

class Scene;
class Collectible;

class Bonfire : public GameObject
{
public:
	Bonfire(Scene* aScene, const unsigned int anIndex);
	~Bonfire();

	void OnCollision(GameObject* aGameObject) override;

	const unsigned int GetBonfireIndex() const;

private:
	Animation myAnimations[2];

	float myTurnInSpeed;
	float myTurnInDistance;
	int myCollectibleIndex;
	const int myBonfireIndex;

	bool myHasBeenActivated;

};

