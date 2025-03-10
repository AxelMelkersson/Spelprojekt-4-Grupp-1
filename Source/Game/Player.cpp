#include "stdafx.h"
#include "Player.hpp"
#include "../External/Headers/CU/Utilities.h"

#include "GameWorld.h"
#include "LevelScene.h"
#include "Controller.h"
#include "InputWrapper.h"

#include "SpriteComponent.h"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"
#include "AnimationComponent.hpp"

#include "BashAbility.h"
#include "BashComponent.hpp"

#include "SpringObject.h"
#include "BashableObject.hpp"
#include "BashableObject.hpp"

#include "Ledge.h"
#include "Collectible.hpp"

#include "DataManager.h"
#include "AudioManager.h"
#include "LevelManager.hpp"
#include "SpeedrunManager.h"

#include "PostMaster.hpp"

#ifdef _DEBUG
#include "imgui.h"
#endif // DEBUG

typedef PlayerData::PlayerFloatEnum PEnum;

Player::Player(LevelScene* aLevelScene) : GameObject(aLevelScene)
{
	myJsonData = new PlayerData();
	myJsonData = dynamic_cast<PlayerData*>(&DataManager::GetInstance().GetDataStruct(DataEnum::player));
	CGameWorld* world = CGameWorld::GetInstance();
	myInputHandler = world->Input();
	myTimerInput = world->GetTimer();

	myBashAbility = new BashAbility(aLevelScene);
	myBashAbility->Init();
	myBashAbility->AddInputWrapper(world->Input());
	myBashAbility->AddPlayerRelation(this);
	myBashAbility->AddTimer(world->GetTimer());

	myBashableObject = nullptr;

	myHasDied = false;

	SetZIndex(101);
	SetPosition({ 20.0f, 10.0f });

	SetPivot(v2f(0.5f, 0.5f));

	mySpawnPosition = v2f(20.0f, 10.0f);
	mySize = v2f(16.0f, 16.0f);

	InitAnimations();
	InitCollider();

	myTriggerRunningAnimationSpeed = 50.0f;
	myAirCoyoteTimer = myJsonData->myFloatValueMap[PEnum::Coyote_Time];

	myDirectionX = 1;
	myCurrentAnimationIndex = 0;

	myHasLanded = true;
	myHasDoubleJumped = false;
	myHasLandedOnSpring = false;
	myCanJumpWhenFalling = false;
	myWillJumpWhenFalling = false;
	myActiveSpringJump = false;
	myGrabbedLedge = false;
	myIsLerpingToPosition = false;
	myIsGliding = false;
	myCheckParticleLanding = true;
	myIsSpawning = true;
	mySkipOneUpdate = true;

	myGlideFactor = 0.14f;

	myResetTimer = 1.0f;

	mySpringVelocity = {};
	myPercentageLeftVelocity = {};
	mySpringVelocityRetardation = {};
	mySpringTimer = {};

	myLedgeSoundIndex = {};

	myIsInRangeOfBash = false;
}

void Player::Update(const float& aDeltaTime)
{
	GameObject::Update(aDeltaTime);

	if (myIsSpawning)
	{
		if (myCurrentAnimationIndex == 13 && myAnimationComponent->GetCurrentIndex() >= 11)
		{
			myIsSpawning = false;
		}

		return;
	}

	if (mySkipOneUpdate)
	{
		mySkipOneUpdate = false;
		return;
	}

	CheckParticleLanding();

	if (myHasDied)
	{
		myResetTimer -= aDeltaTime;
		Kill();
		CGameWorld::GetInstance()->GetTimer()->SetTimeScale(1.0f);
		GetComponent<PhysicsComponent>()->SetVelocity(v2f(0.0f, 0.0f));
		return;
	}

	PhysicsComponent* physics = GetComponent<PhysicsComponent>();

	if (physics)
	{
		if (!myBashAbility->GetIsBashing())
		{
			CheckMove(aDeltaTime);
			CheckJump();
			UpdateCoyoteTime(aDeltaTime);
		}

		UpdatePlayerVelocity(aDeltaTime);

		if (myIsLerpingToPosition)
		{
			LerpToPosition(myLerpPosition);
			LerpToPosition(myLerpPosition);
		}

		if (!myIsInRangeOfBash)
		{
			CGameWorld::GetInstance()->GetTimer()->SetTimeScale(1.0f);

			if (myBashableObject)
			{
				myBashableObject->Dehighlight();
			}
		}

		myIsInRangeOfBash = false;
	}

	if (CGameWorld::GetInstance()->GetTimer()->GetTimeScale() <= 0.05f)
	{
		CGameWorld::GetInstance()->GetTimer()->SetTimeScale(1.0f);
		myAnimationComponent->Update(myTransform, *this);
		CGameWorld::GetInstance()->GetTimer()->SetTimeScale(0.0f);
	}

	AnimationState();

#ifdef _DEBUG
	//ImGuiUpdate();
#endif //DEBUG
}

void Player::DoubleJump()
{
	myPlatformVelocity.y = 0;
	AudioManager::GetInstance()->PlayAudio(AudioList::PlayerDoubleJump);
	myCurrentVelocity.y = -myJsonData->myFloatValueMap[PEnum::Double_Jump_Velocity] + myPlatformVelocity.y - mySpringVelocity.y;
	myAnimationComponent->SetAnimation(&myAnimations[3]);

	if (myIsGliding)
	{
		myAnimationComponent->SetNextAnimation(&myAnimations[12]);
	}
	else
	{
		myAnimationComponent->SetNextAnimation(&myAnimations[4]);
	}

	myCurrentAnimationIndex = 3;
	myHasLanded = false;
	myHasDoubleJumped = true;
	myWillJumpWhenFalling = false;
	myBashAbility->ResetVelocity(false, true);
}
void Player::LedgeJump()
{
	myGrabbedLedge = false;
	AudioManager::GetInstance()->UnLockAudio(AudioList::LandOnGrassEasy);
	if (!myInputHandler->GetInput()->GetKeyDown(Keys::SKey) && myInputHandler->GetController()->GetLeftThumbStick().y < 0.3f)
	{
		myCurrentVelocity.y = -myJsonData->myFloatValueMap[PEnum::Ledge_Jump_Velocity];
	}

	myIsLerpingToPosition = false;

	myAnimationComponent->SetAnimation(&myAnimations[2]);
	myCurrentAnimationIndex = 2;
	myHasDoubleJumped = false;
	myWillJumpWhenFalling = false;
	myBashAbility->ResetVelocity(false, true);
}
void Player::ReactivateDoubleJump()
{
	myHasDoubleJumped = false;
}
void Player::TryLetJumpWhenFalling(const float& aYDistance)
{
	const float distancePerJumpWhenLandingTime = GetComponent<PhysicsComponent>()->GetVelocityY() * myJsonData->myFloatValueMap[PEnum::Jump_When_Falling_Time];
	if (aYDistance < distancePerJumpWhenLandingTime && aYDistance > 0)
	{
		myCanJumpWhenFalling = true;
	}
	else
	{
		myCanJumpWhenFalling = false;
	}
}

void Player::GoRight(const float& aDeltaTime)
{
	if (myCurrentVelocity.x < 0)
	{
		myCurrentVelocity.x = 0.0f;
	}

	if (myPlatformVelocity.x < 0)
	{
		myPlatformVelocity.x = 0.0f;
	}

	if (myBashAbility->GetVelocity().x < 0)
	{
		myBashAbility->ResetVelocity(true, false);
	}

	myCurrentVelocity.x = Utils::Lerp(myCurrentVelocity.x, myJsonData->myFloatValueMap[PEnum::Max_Speed], myJsonData->myFloatValueMap[PEnum::Acceleration] * aDeltaTime);
}
void Player::GoLeft(const float& aDeltaTime)
{
	if (myCurrentVelocity.x > 0)
	{
		myCurrentVelocity.x = 0.0f;
	}

	if (myPlatformVelocity.x > 0)
	{
		myPlatformVelocity.x = 0.0f;
	}

	if (myBashAbility->GetVelocity().x > 0)
	{
		myBashAbility->ResetVelocity(true, false);
	}

	myCurrentVelocity.x = Utils::Lerp(myCurrentVelocity.x, -myJsonData->myFloatValueMap[PEnum::Max_Speed], myJsonData->myFloatValueMap[PEnum::Acceleration] * aDeltaTime);
}
void Player::StartGliding() 
{
	myIsGliding = true;

	myAnimationComponent->SetAnimation(&myAnimations[11]);
	myAnimationComponent->SetNextAnimation(&myAnimations[12]);

	AudioManager::GetInstance()->PlayAudio(AudioList::PlayerHover);
	AudioManager::GetInstance()->LockAudio(AudioList::PlayerHover);
}

void Player::Kill()
{
	if (myBashAbility->GetDashIsActive())
	{
		return;
	}

	if (!myHasDied)
	{
		AudioManager::GetInstance()->PlayAudio(AudioList::PlayerDeath);
		AudioManager::GetInstance()->PlayAudio(AudioList::PlayerRespawn);
		KillReset();
	}
	else if (myResetTimer <= 0)
	{
		LevelScene* levelScene = dynamic_cast<LevelScene*>(myScene);
		if (levelScene)
		{
			levelScene->IncreaseBlackScreen(1.0f);
			if (!levelScene->GetReachedFullOpacity())
			{
				return;
			}
		}

		PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::PlayerDeath, 0));
		CGameWorld::GetInstance()->GetLevelManager().ReloadScene(LevelManager::eScenes::LevelScene);
	}
}
void Player::Landed(const int& aOverlapY)
{
	if (!myHasLanded)
	{
		myInputHandler->GetController()->Vibrate(
			myJsonData->myFloatValueMap[PEnum::Land_Vibration_Strength],
			myJsonData->myFloatValueMap[PEnum::Land_Vibration_Strength],
			myJsonData->myFloatValueMap[PEnum::Land_Vibration_Length]);

		myScene->GetCamera().Shake(
			myJsonData->myFloatValueMap[PEnum::Land_Shake_Duration],
			myJsonData->myFloatValueMap[PEnum::Land_Shake_Intensity],
			myJsonData->myFloatValueMap[PEnum::Land_Shake_DropOff]);
	}

	if (aOverlapY < 0)
	{
		myAirCoyoteTimer = myJsonData->myFloatValueMap[PEnum::Coyote_Time];
		if (!myActiveSpringJump)
		{
			myHasLanded = true;
			myIsGliding = false;
			AudioManager::GetInstance()->Stop(AudioList::PlayerHover);
		}

		myHasDoubleJumped = false;

		myBashAbility->ResetVelocity(true, true);

		if (myWillJumpWhenFalling)
		{
			Jump();
		}
	}
	myCurrentVelocity.y = 0.0f;
	myBashAbility->ResetVelocity(false, true);
	if (!myHasLandedOnSpring)
		mySpringVelocity.y = {};
}
void Player::SideCollision(const int& aOverlapX)
{
	aOverlapX;
	myCurrentVelocity.x = 0.0f;
	myBashAbility->ResetVelocity(true, false);
	mySpringVelocity.x = {};
}

void Player::ResetVelocity()
{
	myCurrentVelocity.x = 0;
	myCurrentVelocity.y = 0;
}
void Player::RedirectVelocities(const v2f& aDirection)
{
	myPlatformVelocity.x = Utils::Abs(myPlatformVelocity.x) * aDirection.x;
	myPlatformVelocity.y = Utils::Abs(myPlatformVelocity.y) * aDirection.y;

	mySpringVelocity.x = Utils::Abs(mySpringVelocity.x) * aDirection.x;
	mySpringVelocity.y = Utils::Abs(mySpringVelocity.y) * aDirection.y;

	if (mySpringVelocity.y < -170.0f)
	{
		AudioManager::GetInstance()->PlayAudio(AudioList::SuperBash);
	}
}

const v2f Player::GetPlatformVelocity() const
{
	return myPlatformVelocity;
}
const bool Player::GetLedgeIsGrabbed() const
{
	return myGrabbedLedge;
}
const bool Player::GetIsGliding() const
{
	return myIsGliding;
}
const bool& Player::GetHasLanded() const
{
	return myHasLanded;
}
const bool Player::GetHasDied() const
{
	return myHasDied;
}
const bool& Player::GetIsBashing() const
{
	return myBashAbility->GetIsBashing();
}
const v2f Player::GetCurrentVelocity() const
{
	return myCurrentVelocity;
}

void Player::SetPlatformVelocity(const v2f& aPlatformVelocity)
{
	myPlatformVelocity = aPlatformVelocity;
}
void Player::SetLerpPosition(const v2f& aPosition)
{
	myLerpPosition = aPosition;
	myIsLerpingToPosition = true;
}
void Player::SetAnimation(const int& aAnimationIndex)
{
	myAnimationComponent->SetAnimation(&myAnimations[aAnimationIndex]);
}
void Player::SetNextAnimation(const int& aAnimationIndex)
{
	myAnimationComponent->SetNextAnimation(&myAnimations[aAnimationIndex]);
}
void Player::SetSpawnPosition(const v2f& aSpawnPosition)
{
	if (CGameWorld::GetInstance()->GetLevelManager().GetSpeedrunManager()->GetIsSpeedrun())
	{
		int end = 0;
	}
	if (CGameWorld::GetInstance()->GetLevelManager().GetLevelIndex() == 0)
	{
		//mySpawnPosition = { 0.5f, 0.5f };
		int end = 0;
	}


	mySpawnPosition = aSpawnPosition;
}

void Player::GrabLedge(const v2f& aLedgeLerpPosition, const v2f& aLedgePosition)
{
	if (myTransform.myPosition.x > aLedgePosition.x)
	{
		myDirectionX = -1;

		PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::PlayerLedgeLeftGrabbedHandParticle, GetPosition()));
		PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::PlayerLedgeLeftGrabbedLegParticle, GetPosition()));
	}
	else if (myTransform.myPosition.x < aLedgePosition.x)
	{
		myDirectionX = 1;

		PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::PlayerLedgeRightGrabbedHandParticle, GetPosition()));
		PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::PlayerLedgeRightGrabbedLegParticle, GetPosition()));
	}

	SetLerpPosition(aLedgeLerpPosition);

	AnimationComponent* animation = myAnimationComponent;
	animation->SetAnimation(&myAnimations[5]);

	myGrabbedLedge = true;
	myCurrentVelocity.y = 0;
	myBashAbility->ResetVelocity(true, true);
	++myLedgeSoundIndex;
	if (myLedgeSoundIndex > 2) myLedgeSoundIndex = 1;
	switch (myLedgeSoundIndex)
	{
	case 1:
		AudioManager::GetInstance()->PlayAudio(AudioList::GrabLedge);
		break;
	case 2:
		AudioManager::GetInstance()->PlayAudio(AudioList::GrabLedge2);
		break;
	}
}
void Player::LeaveLedge()
{
	myGrabbedLedge = false;
	myIsLerpingToPosition = false;
	AudioManager::GetInstance()->PlayAudio(AudioList::LeaveLedge);

	AnimationComponent* animation = myAnimationComponent;
	if (myIsGliding)
	{
		animation->SetNextAnimation(&myAnimations[12]);
	}
	else
	{
		animation->SetNextAnimation(&myAnimations[4]);
	}
}
void Player::EndLerp()
{
	myIsLerpingToPosition = false;
}

void Player::ActivateSpringForce(float aSpringVelocity, const float aRetardation, const bool aShouldResetVelocity)
{
	PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::VelocityLinesParticle, this));
	ReactivateDoubleJump();
	myHasLanded = false;
	myActiveSpringJump = true;
	myHasLandedOnSpring = true;
	mySpringVelocity.y = aSpringVelocity;
	mySpringVelocityRetardation = aRetardation;

	myCurrentVelocity.y = 0;

	if (aShouldResetVelocity)
	{
		myBashAbility->ResetVelocity(false, true);
	}
}
void Player::BounceOnDestructibleWall()
{
	v2f dashVelocity = myBashAbility->GetVelocity();
	dashVelocity.x *= -0.8f;

	myBashAbility->InvertDashDirectionX();
	myBashAbility->SetVelocity(dashVelocity);
}
void Player::BashCollision(GameObject* aGameObject, BashComponent* aBashComponent)
{
	if (myHasDied)
	{
		return;
	}

	if (aBashComponent->GetRadius() * aBashComponent->GetRadius() >= (aGameObject->GetPosition() - GetPosition()).LengthSqr())
	{
		myBashableObject = dynamic_cast<BashableObject*>(aGameObject);
		if (myBashableObject && !myBashAbility->GetIsBashing())
		{
			myBashableObject->Highlight();
		}

		myIsInRangeOfBash = true;

		if (!myInputHandler->IsDashing() && !myBashAbility->GetIsBashing())
		{
			CGameWorld::GetInstance()->GetTimer()->SetTimeScale(0.8f);
		}

		if (myInputHandler->IsDashing() && !myBashAbility->GetIsBashing())
		{
			aGameObject->OnStartBashed();
			myBashAbility->ActivateBash(aGameObject);
			SetLerpPosition(aGameObject->GetPosition());
		}
	}
}

void Player::PlayFootSteps(const int& aPlatformIndex)
{
	AnimationComponent* animation = myAnimationComponent;
	if (myCurrentAnimationIndex == 1)
	{
		//If is running
		switch (aPlatformIndex)
		{
		case 0: // Grass
			switch (animation->GetCurrentIndex())
			{
			case 1:
				AudioManager::GetInstance()->PlayAudio(AudioList::WalkGrassRight);
				AudioManager::GetInstance()->LockAudio(AudioList::WalkGrassRight);
				break;
			case 6:
				AudioManager::GetInstance()->PlayAudio(AudioList::WalkGrassLeft);
				AudioManager::GetInstance()->LockAudio(AudioList::WalkGrassLeft);
				break;
			default:
				AudioManager::GetInstance()->UnLockAudio(AudioList::WalkGrassRight);
				AudioManager::GetInstance()->UnLockAudio(AudioList::WalkGrassLeft);
				break;
			}
			break;
		case 1: // Stone
			switch (animation->GetCurrentIndex())
			{
			case 1:
				AudioManager::GetInstance()->PlayAudio(AudioList::WalkStoneRight);
				AudioManager::GetInstance()->LockAudio(AudioList::WalkStoneRight);
				break;
			case 6:
				AudioManager::GetInstance()->PlayAudio(AudioList::WalkStoneLeft);
				AudioManager::GetInstance()->LockAudio(AudioList::WalkStoneLeft);
				break;
			default:
				AudioManager::GetInstance()->UnLockAudio(AudioList::WalkStoneRight);
				AudioManager::GetInstance()->UnLockAudio(AudioList::WalkStoneLeft);
				break;
			}
			break;
		case 2: //Brick
			switch (animation->GetCurrentIndex())
			{
			case 1:
				AudioManager::GetInstance()->PlayAudio(AudioList::WalkBrickRight);
				AudioManager::GetInstance()->LockAudio(AudioList::WalkBrickRight);
				break;
			case 6:
				AudioManager::GetInstance()->PlayAudio(AudioList::WalkBrickLeft);
				AudioManager::GetInstance()->LockAudio(AudioList::WalkBrickLeft);
				break;
			default:
				AudioManager::GetInstance()->UnLockAudio(AudioList::WalkBrickRight);
				AudioManager::GetInstance()->UnLockAudio(AudioList::WalkBrickLeft);
				break;
			}
			break;
		case 3: // Wood
			switch (animation->GetCurrentIndex())
			{
			case 1:
				AudioManager::GetInstance()->PlayAudio(AudioList::WalkWoodRight);
				AudioManager::GetInstance()->LockAudio(AudioList::WalkWoodRight);
				break;
			case 6:
				AudioManager::GetInstance()->PlayAudio(AudioList::WalkWoodLeft);
				AudioManager::GetInstance()->LockAudio(AudioList::WalkWoodLeft);
				break;
			default:
				AudioManager::GetInstance()->UnLockAudio(AudioList::WalkWoodRight);
				AudioManager::GetInstance()->UnLockAudio(AudioList::WalkWoodLeft);
				break;
			}
			break;
		case 4: // Gravel
			switch (animation->GetCurrentIndex())
			{
			case 1:
				AudioManager::GetInstance()->PlayAudio(AudioList::WalkGravelRight);
				AudioManager::GetInstance()->LockAudio(AudioList::WalkGravelRight);
				break;
			case 6:
				AudioManager::GetInstance()->PlayAudio(AudioList::WalkGravelLeft);
				AudioManager::GetInstance()->LockAudio(AudioList::WalkGravelLeft);
				break;
			default:
				AudioManager::GetInstance()->UnLockAudio(AudioList::WalkGravelRight);
				AudioManager::GetInstance()->UnLockAudio(AudioList::WalkGravelLeft);
				break;
			}
			break;
		default:
			break;
		}
	}
}
void Player::PlayLandingSounds(const int& aPlatformIndex)
{
	if (myCurrentVelocity.y > 240.0f)
	{
		switch (aPlatformIndex)
		{
		case 0:
			AudioManager::GetInstance()->PlayAudio(AudioList::LandOnGrassHeavy);
			break;
		case 1:
			AudioManager::GetInstance()->PlayAudio(AudioList::LandOnStoneHeavy);
			break;
		case 2:
			AudioManager::GetInstance()->PlayAudio(AudioList::LandBricksHeavy);
			break;
		case 3:
			AudioManager::GetInstance()->PlayAudio(AudioList::LandWoodHeavy);
			break;
		case 4:
			AudioManager::GetInstance()->PlayAudio(AudioList::LandGravelHeavy);
			break;
		}
	}
	else if (myCurrentVelocity.y > 10.0f)
	{
		switch (aPlatformIndex)
		{
		case 0:
			AudioManager::GetInstance()->PlayAudio(AudioList::LandOnGrassEasy);
			break;
		case 1:
			AudioManager::GetInstance()->PlayAudio(AudioList::LandOnStoneLight);
			break;
		case 2:
			AudioManager::GetInstance()->PlayAudio(AudioList::LandBricksLight);
			break;
		case 3:
			AudioManager::GetInstance()->PlayAudio(AudioList::LandWoodLight);
			break;
		case 4:
			AudioManager::GetInstance()->PlayAudio(AudioList::LandGravelLight);
			break;
		}
	}
}

void Player::SpawnAnimation() 
{
	myAnimationComponent->SetAnimation(&myAnimations[13]);
	myAnimationComponent->SetNextAnimation(&myAnimations[14]);

	myCurrentAnimationIndex = 13;
}
void Player::StopSpawn()
{
	if (myCurrentAnimationIndex != 13 && myIsSpawning)
	{
		SetAnimation(0);
		myCurrentAnimationIndex = 0;
		myIsSpawning = false;
	}
}

// Private Methods
void Player::InitCollider()
{
	PhysicsComponent* physics = AddComponent<PhysicsComponent>();
	physics->SetCanCollide(true);
	physics->SetIsStatic(false);
	physics->SetApplyGravity(false);

	ColliderComponent* collider = AddComponent<ColliderComponent>();
	collider->SetSize(7.0f, 12.0f);
	collider->SetPosition(v2f(0.0f, 2.0f));
}
void Player::InitAnimations()
{
	SpriteComponent* spriteIdle = AddComponent<SpriteComponent>();
	spriteIdle->SetSpritePath("Sprites/Characters/PlayerIdle.dds");
	spriteIdle->SetSize(mySize);

	SpriteComponent* spriteRun = AddComponent<SpriteComponent>();
	spriteRun->SetSpritePath("Sprites/Characters/SpritePlayerRun.dds");
	spriteRun->SetSize(mySize);
	spriteRun->Deactivate();

	SpriteComponent* spriteJump = AddComponent<SpriteComponent>();
	spriteJump->SetSpritePath("Sprites/Characters/PlayerJump.dds");
	spriteJump->SetSize(mySize);
	spriteJump->Deactivate();

	SpriteComponent* spriteDoubleJump = AddComponent<SpriteComponent>();
	spriteDoubleJump->SetSpritePath("Sprites/Characters/PlayerDoubleJump.dds");
	spriteDoubleJump->SetSize(mySize);
	spriteDoubleJump->Deactivate();

	SpriteComponent* spriteFall = AddComponent<SpriteComponent>();
	spriteFall->SetSpritePath("Sprites/Characters/PlayerFall.dds");
	spriteFall->SetSize(mySize);
	spriteFall->Deactivate();

	SpriteComponent* spriteLedgeGrab = AddComponent<SpriteComponent>();
	spriteLedgeGrab->SetSpritePath("Sprites/Characters/playerLedgeGrab.dds");
	spriteLedgeGrab->SetSize(mySize);
	spriteLedgeGrab->Deactivate();

	SpriteComponent* spriteBashStart = AddComponent<SpriteComponent>();
	spriteBashStart->SetSpritePath("Sprites/Characters/PlayerBashStart.dds");
	spriteBashStart->SetSize(mySize);
	spriteBashStart->Deactivate();

	SpriteComponent* spriteBashLoop = AddComponent<SpriteComponent>();
	spriteBashLoop->SetSpritePath("Sprites/Characters/PlayerBashLoop.dds");
	spriteBashLoop->SetSize(mySize);
	spriteBashLoop->Deactivate();

	SpriteComponent* spriteBashFlying = AddComponent<SpriteComponent>();
	spriteBashFlying->SetSpritePath("Sprites/Characters/PlayerBashFlyingLoop.dds");
	spriteBashFlying->SetSize(mySize);
	spriteBashFlying->Deactivate();

	SpriteComponent* spriteBashFlyingTransition = AddComponent<SpriteComponent>();
	spriteBashFlyingTransition->SetSpritePath("Sprites/Characters/PlayerBashFlyingTransition.dds");
	spriteBashFlyingTransition->SetSize(mySize);
	spriteBashFlyingTransition->Deactivate();

	SpriteComponent* spriteDeath = AddComponent<SpriteComponent>();
	spriteDeath->SetSpritePath("Sprites/Characters/PlayerDeath.dds");
	spriteDeath->SetSize(mySize);
	spriteDeath->Deactivate();

	SpriteComponent* spriteGlideTransition = AddComponent<SpriteComponent>();
	spriteGlideTransition->SetSpritePath("Sprites/Characters/PlayerGlidingTransition.dds");
	spriteGlideTransition->SetSize(mySize);
	spriteGlideTransition->Deactivate();

	SpriteComponent* spriteGlide = AddComponent<SpriteComponent>();
	spriteGlide->SetSpritePath("Sprites/Characters/PlayerGlidingLoop.dds");
	spriteGlide->SetSize(mySize);
	spriteGlide->Deactivate();

	SpriteComponent* spriteSpawn = AddComponent<SpriteComponent>();
	spriteSpawn->SetSpritePath("Sprites/Characters/PlayerSpawn.dds");
	spriteSpawn->SetSize(mySize);
	spriteSpawn->Deactivate();

	myAnimations[0] = Animation(false, false, false, 0, 74, 74, 0.10f, spriteIdle, 16, 16);
	myAnimations[1] = Animation(false, false, false, 0, 9, 9, 0.085f, spriteRun, 16, 16);
	myAnimations[2] = Animation(false, true, false, 0, 6, 6, 0.07f, spriteJump, 16, 16);
	myAnimations[3] = Animation(false, true, false, 0, 5, 5, 0.09f, spriteDoubleJump, 16, 16);
	myAnimations[4] = Animation(false, false, false, 0, 4, 4, 0.09f, spriteFall, 16, 16);
	myAnimations[5] = Animation(false, false, false, 0, 22, 22, 0.125f, spriteLedgeGrab, 16, 16);
	myAnimations[6] = Animation(false, true, false, 0, 13, 13, 0.07f, spriteBashStart, 16, 16);
	myAnimations[7] = Animation(false, false, false, 0, 9, 9, 0.10f, spriteBashLoop, 16, 16);
	myAnimations[8] = Animation(false, false, false, 0, 4, 4, 0.10f, spriteBashFlying, 16, 16);
	myAnimations[9] = Animation(false, true, false, 0, 4, 4, 0.10f, spriteBashFlyingTransition, 16, 16);
	myAnimations[10] = Animation(false, true, false, 0, 20, 20, 0.09f, spriteDeath, 16, 16);
	myAnimations[11] = Animation(false, true, false, 0, 4, 4, 0.07f, spriteGlideTransition, 16, 16);
	myAnimations[12] = Animation(false, false, false, 0, 4, 4, 0.085f, spriteGlide, 16, 16);
	myAnimations[13] = Animation(false, true, false, 0, 7, 7, 0.095f, spriteSpawn, 16, 16);
	myAnimations[14] = Animation(false, true, false, 7, 12, 12, 0.14f, spriteSpawn, 16, 16);

	myAnimationComponent = AddComponent<AnimationComponent>();
	myAnimationComponent->SetSprite(spriteIdle);
	myAnimationComponent->SetAnimation(&myAnimations[0]);
	spriteIdle->SetSize(mySize);
}
void Player::Jump()
{
	if (myPlatformVelocity.y > 0.0f)
	{
		myPlatformVelocity.y = 0;
	}

	UnlockLandingSounds();
	PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::PlayerLandedParticle, GetPosition()));
	AudioManager::GetInstance()->PlayAudio(AudioList::PlayerJump);
	v2f calculatedSpring = mySpringVelocity;
	calculatedSpring.y = calculatedSpring.y;
	myCurrentVelocity.y = -myJsonData->myFloatValueMap[PEnum::Jump_Velocity] + myPlatformVelocity.y - calculatedSpring.y;
	myAnimationComponent->SetAnimation(&myAnimations[2]);

	if (myIsGliding)
	{
		myAnimationComponent->SetNextAnimation(&myAnimations[12]);
	}
	else
	{
		myAnimationComponent->SetNextAnimation(&myAnimations[4]);
	}

	myCurrentAnimationIndex = 2;
	myHasLanded = false;
	myWillJumpWhenFalling = false;
	myBashAbility->ResetVelocity(false, true);
}
void Player::Respawn()
{
	myHasDied = false;
	SetAnimation(0);
	SetPosition(mySpawnPosition);
}
void Player::KillReset()
{
	myScene->GetCamera().Shake(
		myJsonData->myFloatValueMap[PEnum::Die_Shake_Duration],
		myJsonData->myFloatValueMap[PEnum::Die_Shake_Intensity],
		myJsonData->myFloatValueMap[PEnum::Die_Shake_DropOff]);
	myInputHandler->GetController()->Vibrate(
		myJsonData->myFloatValueMap[PEnum::Die_Vibration_Strength],
		myJsonData->myFloatValueMap[PEnum::Die_Vibration_Strength],
		myJsonData->myFloatValueMap[PEnum::Die_Vibration_Length]);

	ResetVelocity();
	myBashAbility->ResetVelocity(true, true);
	myBashAbility->StopBashing();
	myPlatformVelocity = v2f();
	myHasDied = true;
	SetAnimation(10);
	myAnimationComponent->SetNextAnimation(nullptr);
}
void Player::UnlockLandingSounds()
{
	AudioManager::GetInstance()->UnLockAudio(AudioList::LandOnGrassEasy);
	AudioManager::GetInstance()->UnLockAudio(AudioList::LandOnStoneLight);
	AudioManager::GetInstance()->UnLockAudio(AudioList::LandBricksLight);
	AudioManager::GetInstance()->UnLockAudio(AudioList::LandWoodLight);
	AudioManager::GetInstance()->UnLockAudio(AudioList::LandGravelLight);
}
void Player::UpdateCoyoteTime(const float& aDeltaTime)
{
	if (GetComponent<PhysicsComponent>()->GetVelocityY() > 0)
	{
		myAirCoyoteTimer -= aDeltaTime;
	}
}
void Player::UpdatePlayerVelocity(const float& aDeltaTime)
{
	PhysicsComponent* physics = GetComponent<PhysicsComponent>();

	if (!myGrabbedLedge)
	{
		float fallDecreaseFactor = 1.0f;
		if (myIsGliding && physics->GetVelocity().y > 0.0f)
		{
			fallDecreaseFactor = myGlideFactor;
		}

		myCurrentVelocity.y = Utils::Min(myCurrentVelocity.y + PhysicsManager::ourGravity * aDeltaTime, myJsonData->myFloatValueMap[PEnum::Max_Fall_Speed] * fallDecreaseFactor);
	}

	physics->SetVelocity(myCurrentVelocity + myBashAbility->GetVelocity() + myPlatformVelocity + mySpringVelocity);

	if (physics->GetVelocityY() > 0.0f)
	{
		if (myBashAbility->GetVelocity().y < 0.0f)
		{
			myCurrentVelocity.y = 0.0f;
			myBashAbility->ResetVelocity(false, true);
		}

		if (myPlatformVelocity.y < 0.0f)
		{
			myCurrentVelocity.y = 0.0f;
			myPlatformVelocity.y = 0.0f;
		}

		if (mySpringVelocity.y < 0.0f)
		{
			myCurrentVelocity.y = 0.0f;
			mySpringVelocity.y = 0.0f;
		}
	}

	if (myCurrentVelocity.x + myBashAbility->GetVelocity().x > 0)
	{
		myDirectionX = 1;
	}
	else if (myCurrentVelocity.x + myBashAbility->GetVelocity().x < 0)
	{
		myDirectionX = -1;
	}

	if ((myCurrentVelocity.y != 0) && (myAirCoyoteTimer <= 0))
	{
		myHasLanded = false;
	}

	myPlatformVelocity.x = Utils::Lerp(myPlatformVelocity.x, 0.0f, myJsonData->myFloatValueMap[PEnum::Platform_Velocity_Retardation] * aDeltaTime);
	myPlatformVelocity.y = Utils::Lerp(myPlatformVelocity.y, 0.0f, myJsonData->myFloatValueMap[PEnum::Platform_Velocity_Retardation] * aDeltaTime);

	if (myActiveSpringJump)
	{
		DecreaseSpringJump(aDeltaTime);
	}
}
const void Player::CheckJump()
{
	if (myInputHandler->IsJumping() || myInputHandler->GetInput()->GetKeyDown(Keys::SKey))
	{
		if (myGrabbedLedge)
		{
			LedgeJump();
		}
		else if (!myInputHandler->GetInput()->GetKeyDown(Keys::SKey))
		{
			if (myCanJumpWhenFalling && !myHasLanded && GetComponent<PhysicsComponent>()->GetVelocityY() >= 0.0f)
			{
				myWillJumpWhenFalling = true;
			}

			if (myHasLanded && (GetComponent<PhysicsComponent>()->GetVelocityY() == 0.0f || myAirCoyoteTimer > 0))
			{
				Jump();
			}
			else if (!myHasDoubleJumped)
			{
				DoubleJump();
			}
		}
	}
}
const void Player::CheckMove(const float& aDeltaTime)
{
	if (myGrabbedLedge)
	{
		return;
	}

	PhysicsComponent* physics = GetComponent<PhysicsComponent>();

	if (myInputHandler->IsMovingRight())
	{
		GoRight(aDeltaTime);
	}
	else if (myInputHandler->IsMovingLeft())
	{
		GoLeft(aDeltaTime);
	}
	else
	{
		myCurrentVelocity.x = Utils::Lerp(myCurrentVelocity.x, 0.0f, myJsonData->myFloatValueMap[PEnum::Retardation] * aDeltaTime);
	}
}
const void Player::CheckParticleLanding()
{
	if (myHasLanded && !myCheckParticleLanding && !myWillJumpWhenFalling)
	{
		myCheckParticleLanding = true;
		PostMaster::GetInstance().ReceiveMessage(Message(eMessageType::PlayerLandedParticle, GetPosition()));
	}
	if (!myHasLanded)
	{
		myCheckParticleLanding = false;
	}
}
const void Player::AnimationState()
{
	AnimationComponent* animation = myAnimationComponent;
	if (Utils::Abs(myCurrentVelocity.x) <= myTriggerRunningAnimationSpeed && myHasLanded && myCurrentAnimationIndex != 0)
	{
		animation->SetAnimation(&myAnimations[0]);
		myCurrentAnimationIndex = 0;
	}
	else if (Utils::Abs(myCurrentVelocity.x) > myTriggerRunningAnimationSpeed && myHasLanded && myCurrentAnimationIndex != 1)
	{
		animation->SetAnimation(&myAnimations[1]);
		myCurrentAnimationIndex = 1;
	}

	if (myCurrentAnimationIndex != 2 && myCurrentAnimationIndex != 3 && myCurrentAnimationIndex != 4 && !myHasLanded)
	{
		if (myIsGliding)
		{
			animation->SetAnimation(&myAnimations[12]);
		}
		else
		{
			animation->SetAnimation(&myAnimations[4]);
		}

		myCurrentAnimationIndex = 4;
	}

	for (Animation& animation : myAnimations)
	{
		animation.mySpriteComponent->SetSizeX(mySize.x * myDirectionX);
	}
}
const void Player::LerpToPosition(const v2f& aPosition)
{
	const float timeScale = myTimerInput->GetTimeScale();;

	myTimerInput->SetTimeScale(1.0f);

	myTransform.myPosition.x = Utils::Lerp(myTransform.myPosition.x, aPosition.x, myJsonData->myFloatValueMap[PEnum::Lerp_Acceleration] * myTimerInput->GetDeltaTime());
	myTransform.myPosition.y = Utils::Lerp(myTransform.myPosition.y, aPosition.y, myJsonData->myFloatValueMap[PEnum::Lerp_Acceleration] * myTimerInput->GetDeltaTime());

	if (aPosition.x > myTransform.myPosition.x)
	{
		myDirectionX = 1;
	}
	else if(aPosition.x < myTransform.myPosition.x)
	{
		myDirectionX = -1;
	}

	if (Utils::Abs(aPosition.x - myTransform.myPosition.x) <= 0.5f)
	{
		myTransform.myPosition.x = aPosition.x;
	}

	if (Utils::Abs(aPosition.y - myTransform.myPosition.y) <= 0.5f)
	{
		myTransform.myPosition.y = aPosition.y;
	}

	myTimerInput->SetTimeScale(timeScale);
}
const void Player::DecreaseSpringJump(const float& aDeltaTime)
{
	if (myCurrentVelocity.y == 0)
	{
		myActiveSpringJump = false;
		mySpringVelocity = { 0.0f, 0.0f };
		myCurrentVelocity.y = { 0.0f };
	}

	if (GetComponent<PhysicsComponent>()->GetVelocityY() > 0)
	{
		myActiveSpringJump = false;
		mySpringVelocity = { 0.0f, 0.0f };
		myCurrentVelocity.y = { 0.0f };
	}
	else
	{
		myHasLandedOnSpring = false;
		mySpringVelocity.x = { 0.0f };
		mySpringVelocity.y = Utils::Lerp(mySpringVelocity.y, 0.0f, mySpringVelocityRetardation * aDeltaTime);
	}
}

#ifdef _DEBUG
void Player::ImGuiUpdate()
{
	ImGui::Begin("Player", &myIsActive, ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::Button("Save to JSON"))
	{
		DataManager::GetInstance().SetDataStruct(DataEnum::player);
	}

	ImGui::InputFloat("Max Speed", &myJsonData->myFloatValueMap[PEnum::Max_Speed], 0.0f, 2000.0f);
	ImGui::InputFloat("Acceleration", &myJsonData->myFloatValueMap[PEnum::Acceleration], 0.0f, 100.0f);
	ImGui::InputFloat("Retardation", &myJsonData->myFloatValueMap[PEnum::Retardation], 0.0f, 100.0f);
	ImGui::InputFloat("Lerp Acceleration", &myJsonData->myFloatValueMap[PEnum::Lerp_Acceleration], 0.0f, 100.0f);
	ImGui::InputFloat("Platform Velocity Retardation", &myJsonData->myFloatValueMap[PEnum::Platform_Velocity_Retardation], 0.0f, 100.0f);
	ImGui::InputFloat("Coyote Time", &myJsonData->myFloatValueMap[PEnum::Coyote_Time], 0.0f, 1.0f);
	ImGui::InputFloat("Jump Velocity", &myJsonData->myFloatValueMap[PEnum::Jump_Velocity], 0.0f, 2000.0f);
	ImGui::InputFloat("Double Jump Velocity", &myJsonData->myFloatValueMap[PEnum::Double_Jump_Velocity], 0.0f, 2000.0f);
	ImGui::InputFloat("Max Fall Speed", &myJsonData->myFloatValueMap[PEnum::Max_Fall_Speed], 0.0f, 2000.0f);
	ImGui::InputFloat("Ledge Jump Velocity", &myJsonData->myFloatValueMap[PEnum::Ledge_Jump_Velocity], 0.0f, 2000.0f);
	ImGui::InputFloat("Jump When Falling Time", &myJsonData->myFloatValueMap[PEnum::Jump_When_Falling_Time], 0.0f, 1.0f);
	ImGui::InputFloat("Trigger Falling Speed", &myJsonData->myFloatValueMap[PEnum::Trigger_Falling_Speed], 0.0f, 50.0f);

	ImGui::Text("Vibrations");
	ImGui::InputFloat("Die Vibration Strength", &myJsonData->myFloatValueMap[PEnum::Die_Vibration_Strength], 0, 65000);
	ImGui::InputFloat("Land Vibration Strength", &myJsonData->myFloatValueMap[PEnum::Land_Vibration_Strength], 0, 65000);
	ImGui::InputFloat("Springs Vibration Strength", &myJsonData->myFloatValueMap[PEnum::Springs_Vibration_Strength], 0, 65000);

	ImGui::InputFloat("Die Vibration Length", &myJsonData->myFloatValueMap[PEnum::Die_Vibration_Length], 0.0f, 10.0f);
	ImGui::InputFloat("Land Vibration Length", &myJsonData->myFloatValueMap[PEnum::Land_Vibration_Length], 0.0f, 10.0f);
	ImGui::InputFloat("Springs Vibration Length", &myJsonData->myFloatValueMap[PEnum::Springs_Vibration_Length], 0.0f, 10.0f);

	ImGui::Text("Camera Shake");
	ImGui::InputFloat("Die Shake Duration", &myJsonData->myFloatValueMap[PEnum::Die_Shake_Duration], 0.0f, 10.0f);
	ImGui::InputFloat("Die Shake Intensity", &myJsonData->myFloatValueMap[PEnum::Die_Shake_Intensity], 0.0f, 10.0f);
	ImGui::InputFloat("Die Shake DropOff", &myJsonData->myFloatValueMap[PEnum::Die_Shake_DropOff], 0.0f, 10.0f);

	ImGui::InputFloat("Land Shake Duration", &myJsonData->myFloatValueMap[PEnum::Land_Shake_Duration], 0.0f, 10.0f);
	ImGui::InputFloat("Land Shake Intensity", &myJsonData->myFloatValueMap[PEnum::Land_Shake_Intensity], 0.0f, 10.0f);
	ImGui::InputFloat("Land Shake DropOff", &myJsonData->myFloatValueMap[PEnum::Land_Shake_DropOff], 0.0f, 10.0f);

	ImGui::InputFloat("Spring Shake Duration", &myJsonData->myFloatValueMap[PEnum::Spring_Shake_Duration], 0.0f, 10.0f);
	ImGui::InputFloat("Spring Shake Intensity", &myJsonData->myFloatValueMap[PEnum::Spring_Shake_Intensity], 0.0f, 10.0f);
	ImGui::InputFloat("Spring Shake DropOff", &myJsonData->myFloatValueMap[PEnum::Spring_Shake_DropOff], 0.0f, 10.0f);

	ImGui::End();
}
#endif // DEBUG