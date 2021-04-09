#include "stdafx.h"
#include "Enemy.h"
#include "EnemyProjectile.h"

#include "Player.hpp"
#include "LevelScene.h"

#include "SpriteComponent.h"
#include "AnimationComponent.hpp"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"
#include "WaypointComponent.hpp"
#include "AudioComponent.h"

#include "AudioManager.h"
#include "PostMaster.hpp"

#ifdef _DEBUG
#include "imgui.h"
#endif // DEBUG

typedef EnemyData::EnemyFloatEnum EEnum;

Enemy::Enemy(Scene* aScene) : GameObject(aScene)
{
	myJsonData = dynamic_cast<EnemyData*>(&DataManager::GetInstance().GetDataStruct(DataEnum::enemy));
	myWayPointComponent = nullptr;
	AudioComponent* audio = AddComponent<AudioComponent>();
	audio->AddAudio(AudioList::EnemyNormalIdle);
	audio->SetRadius(100);
}
NormalEnemy::NormalEnemy(Scene* aScene) : Enemy(aScene)
{
	InitAnimation();
	this->SetZIndex(140);
}
ShootingEnemy::ShootingEnemy(Scene* aScene) : Enemy(aScene)
{
	InitAnimation();
	this->SetZIndex(140);
}

void Enemy::InitEnemy(const std::vector<v2f>& someWayPoints, const float& aSpeed)
{
	if (someWayPoints.size() > 1)
	{
		myWayPointComponent = AddComponent<WaypointComponent>();
		myWayPointComponent->SetOwner(this);
		myWayPointComponent->SetWaypoints(someWayPoints);
		myWayPointComponent->SetSpeed(aSpeed);
		IsMoving = true;
		mySpeed = aSpeed;
	}
	if (someWayPoints.size() == 0)
	{
		this->SetPosition({ 0.0f, 0.0f });
	}
	else
	{
		this->SetPosition(someWayPoints[0]);
	}
	InitCollider();
}
void Enemy::InitCollider()
{
	PhysicsComponent* physics = AddComponent<PhysicsComponent>();
	physics->SetCanCollide(false);
	physics->SetIsStatic(false);
	physics->SetApplyGravity(false);
}
void NormalEnemy::InitCollider()
{
	Enemy::InitCollider();
	ColliderComponent* collider = this->AddComponent<ColliderComponent>();
	collider->SetCollideOnlyWithPlayer(true);

	Transform transform = this->GetTransform();
	collider->SetSize({myJsonData->myFloatValueMap[EEnum::NE_CollisionSizeX], myJsonData->myFloatValueMap[EEnum::NE_CollisionSizeY]});
}
void ShootingEnemy::InitCollider()
{
	Enemy::InitCollider();
	ColliderComponent* collider = this->AddComponent<ColliderComponent>();
	collider->SetCollideOnlyWithPlayer(true);

	Transform transform = this->GetTransform();
	collider->SetSize({ myJsonData->myFloatValueMap[EEnum::SE_CollisionSizeX], myJsonData->myFloatValueMap[EEnum::SE_CollisionSizeY] });
}

void NormalEnemy::Update(const float& aDeltaTime)
{
	if (IsMoving)
	{
		myWayPointComponent->Move(aDeltaTime);
	}
	GameObject::Update(aDeltaTime);
}
void ShootingEnemy::Update(const float& aDeltaTime)
{
	if (IsMoving)
	{
		myWayPointComponent->Move(aDeltaTime);
	}
	GameObject::Update(aDeltaTime);

	if (GetComponent<AnimationComponent>()->GetHasEnded())
	{
		GetComponent<AnimationComponent>()->SetAnimation(&myAnimations[0]);
	}

	if (GetComponent<SpriteComponent>()->GetShouldRender())
	{
		myShotTimer -= aDeltaTime;
		if (myShotTimer <= 0)
		{
			myShotTimer = myJsonData->myFloatValueMap[EEnum::FireRate];
			Shoot();
		}
	}
#ifdef _DEBUG
	ImGuiUpdate();
#endif // _DEBUG
}

void NormalEnemy::InitAnimation()
{
	SpriteComponent* spriteIdle = AddComponent<SpriteComponent>();
	spriteIdle->SetSpritePath("Sprites/Enemies/Enemy1.dds");
	spriteIdle->SetSize({ myJsonData->myFloatValueMap[EEnum::NE_SpriteSizeX], myJsonData->myFloatValueMap[EEnum::NE_SpriteSizeY] });

	myAnimation = Animation(false, false, false, 0, 10, 10, 0.1f, spriteIdle, 16, 16);

	AnimationComponent* anime = AddComponent<AnimationComponent>();
	anime->SetSprite(spriteIdle);
	anime->SetAnimation(&myAnimation);
}

void ShootingEnemy::InitAnimation()
{
	SpriteComponent* spriteIdle = AddComponent<SpriteComponent>();
	spriteIdle->SetSpritePath("Sprites/Enemies/Enemy2.dds");
	spriteIdle->SetSize({ myJsonData->myFloatValueMap[EEnum::SE_SpriteSizeX], myJsonData->myFloatValueMap[EEnum::SE_SpriteSizeY] });

	SpriteComponent* spriteAttack = AddComponent<SpriteComponent>();
	spriteAttack->SetSpritePath("Sprites/Enemies/Enemy2Attack.dds");
	spriteAttack->SetSize({ myJsonData->myFloatValueMap[EEnum::SE_SpriteSizeX], myJsonData->myFloatValueMap[EEnum::SE_SpriteSizeY] });
	spriteAttack->Deactivate();

	myAnimations[0] = Animation(false, false, false, 0, 10, 10, 0.1f, spriteIdle, 16, 16);
	myAnimations[1] = Animation(false, false, false, 0, 10, 10, 0.1f, spriteAttack, 16, 16);

	AnimationComponent* anime = AddComponent<AnimationComponent>();
	anime->SetSprite(spriteIdle);
	anime->SetAnimation(&myAnimations[0]);
}

void ShootingEnemy::Shoot()
{
	GetComponent<AnimationComponent>()->SetAnimation(&myAnimations[1]);
	GetComponent<AnimationComponent>()->SetNextAnimation(&myAnimations[0]);
	AudioManager::GetInstance()->PlayAudio(AudioList::EnemyShooting);
	EnemyProjectile* projectile = new EnemyProjectile(this->myScene, this->GetPosition(), dynamic_cast<LevelScene*>(this->myScene)->GetPlayer()->GetPosition());
}
void Enemy::OnCollision(GameObject* aGameObject)
{
	Player* player = dynamic_cast<Player*>(aGameObject);
	if (player)
	{
		if (!player->GetHasDied())
		{
			player->Kill();
		}
	}
}

#ifdef _DEBUG
void ShootingEnemy::ImGuiUpdate()
{
	ImGui::Begin("Enemies", &myIsActive, ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::Button("Save to JSON"))
	{
		DataManager::GetInstance().SetDataStruct(DataEnum::enemy);
	}

	ImGui::InputFloat("Fire Rate", &myJsonData->myFloatValueMap[EEnum::FireRate], 0.0f, 50.0f);
	ImGui::InputFloat("Fire Radius", &myJsonData->myFloatValueMap[EEnum::FireRadius], 0.0f, 1000.0f);
	ImGui::InputFloat("Speed", &myJsonData->myFloatValueMap[EEnum::Speed], 0.0f, 1000.0f);

	ImGui::End();
}
#endif // _DEBUG
