#pragma once
#include "GameObject.h"
#include "Animation.hpp"
#include <memory>

class Scene;
class Collectible;
class SpeechBubble;

class Bonfire : public GameObject
{
public:
	Bonfire(Scene* aScene, const unsigned int anIndex);

	void Update(const float& aDeltaTime) override;

	void OnCollision(GameObject* aGameObject) override;
	const unsigned int GetBonfireIndex() const;

private:
	Animation myAnimations[2];

	float myTurnInSpeed;
	float myTurnInDistance;
	int myCollectibleIndex;
	const int myBonfireIndex;

	bool myHasBeenActivated;
	std::unique_ptr<SpeechBubble> mySpeechBubble;
	bool myActivateParticle;
};