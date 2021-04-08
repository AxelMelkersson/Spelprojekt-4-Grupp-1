#pragma once
#include "GameObject.h"
#include "Animation.hpp"
#include "DataManager.h"
#include <vector>

class LevelScene;
class EnemyProjectile;
class WaypointComponent;

class Enemy : public GameObject
{
public:
	Enemy(Scene* aScene);
	virtual ~Enemy() {}

	void InitEnemy(const std::vector<v2f>& someCoordinates, const float& aSpeed);
	virtual void Update(const float& aDeltaTime) = 0;
	void OnCollision(GameObject* aGameObject) override;

protected:
	virtual void InitCollider();

	EnemyData* myJsonData = new EnemyData();
	bool IsMoving = false;
	WaypointComponent* myWayPointComponent;

private:
	float mySpeed = 200;

};

class NormalEnemy : public Enemy
{
public:
	NormalEnemy(Scene* aScene);
	~NormalEnemy() = default;
	void Update(const float& aDeltaTime);

private:
	void InitAnimation();
	void InitCollider() override;
	Animation myAnimation;
};

class ShootingEnemy : public Enemy
{
public:
	ShootingEnemy(Scene* aScene);
	~ShootingEnemy() = default;

	void Update(const float& aDeltaTime);

private:
	float myShotTimer = 0.0f;
	Animation myAnimations[2];

	void InitAnimation();
	void Shoot();
	void InitCollider() override;

#ifdef _DEBUG
	void ImGuiUpdate();
#endif // _DEBUG

};