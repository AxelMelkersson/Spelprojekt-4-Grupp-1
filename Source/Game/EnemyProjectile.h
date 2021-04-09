#pragma once
#include "DataManager.h"

class ParticleEffectFactory;
class GameObject;

class EnemyProjectile : public GameObject
{
public:
	EnemyProjectile(Scene* aScene, const v2f& aPosition, const v2f& aTarget);
	~EnemyProjectile() = default;

	void Update(const float& aDeltaTime) override;

	void SetDirection(const v2f& aPosition, const v2f& aTarget);
	void OnCollision(GameObject* aGameObject) override;

	void InitCollider();
	void InitVisuals();

private:
	Animation myAnimation;

	v2f mySpriteSize;
	v2f myColliderSize = {3.0f, 3.0f};
	v2f myDirection;

	EnemyData* myJsonData = new EnemyData();

};