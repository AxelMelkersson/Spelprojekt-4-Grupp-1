#pragma once
#include "GameObject.h"
#include "Animation.hpp"
#include "BashAbility.h"
#include "DataManager.h"
#include "BashableObject.hpp"

namespace Utils
{
	class Input;
}

class InputWrapper;
class LevelScene;
class AnimationComponent;
class Ledge;
class BashComponent;
class SpringObject;
class Collectible;

class Player : public GameObject
{
public:
	Player(LevelScene* aLevelScene);
	~Player();

	void InitAnimations();
	void InitCollider();

	void Update(const float& aDeltaTime) override;

	void CheckJump();

	void UpdateCoyoteTime(const float& aDeltaTime);

	void TryLetJumpWhenFalling(const float& aYDistance);

	void CheckMove(const float& aDeltaTime);

	void GoRight(const float& aDeltaTime);
	void GoLeft(const float& aDeltaTime);

	void Jump();
	void DoubleJump();
	void LedgeJump();

	void ReactivateDoubleJump();

	void Landed(const int& aOverlapY) override;
	void SideCollision(const int& aOverlapX) override;

	void ResetVelocity();

	void RedirectVelocities(const v2f& aDirection);

	const v2f GetPlatformVelocity();
	void SetPlatformVelocity(const v2f& aPlatformVelocity);

	void AnimationState();

	void UpdatePlayerVelocity(const float& aDeltaTime);

	void GrabLedge(const v2f& aLedgeLerpPosition, const v2f& aLedgePosition);
	void LeaveLedge();
	const bool GetLedgeIsGrabbed();

	void LerpToPosition(const v2f& aPosition);
	void SetLerpPosition(const v2f& aPosition);
	void EndLerp();

	void ActivateSpringForce(float mySpringVelocity, const float aRetardation, const bool aShouldResetVelocity);

	void BounceOnDestructibleWall();
	const bool& GetIsBashing();

	void Kill();
	void Eaten();

	void KillReset();
	void Respawn();

	void BashCollision(GameObject* aGameObject, BashComponent* aBashComponent) override;

	void DecreaseSpringJump(const float& aDeltaTime);

	const bool& GetHasLanded();

	void SetAnimation(const int& aAnimationIndex);
	void SetNextAnimation(const int& aAnimationIndex);

	void UnlockLandingSounds();

	void SetSpawnPosition(const v2f& aSpawnPosition);

	void StartGliding();
	const bool GetIsGliding();

	const bool GetHasDied();
	void PlayFootSteps(const int& aPlatformIndex);
	void PlayLandingSounds(const int& aPlatformIndex);

	const v2f GetCurrentVelocity();

	void SpawnAnimation();
	void StopSpawn();

private:
#ifdef _DEBUG
	void ImGuiUpdate();
#endif // _DEBUG

	Animation myAnimations[15];
	std::vector<Collectible*> myCollectibles;

	std::shared_ptr<InputWrapper> myInputHandler;
	BashAbility* myBashAbility;
	BashableObject* myBashableObject;
	AnimationComponent* myAnimationComponent;

	Utils::Timer* myTimerInput;

	v2f myCurrentVelocity;
	v2f myPlatformVelocity;
	v2f mySpringVelocity;
	v2f myLerpPosition;
	v2f mySize;
	v2f mySpawnPosition;

	float myAirCoyoteTimer;
	float myTriggerRunningAnimationSpeed;
	float mySpringVelocityRetardation;
	float myPercentageLeftVelocity;
	float mySpringTimer;
	float myGlideFactor;

	int myCurrentAnimationIndex;
	int myDirectionX;

	int myLedgeSoundIndex;

	bool myHasLanded;
	bool myHasLandedVibration;
	bool myHasDoubleJumped;
	bool myHasLandedOnSpring;
	bool myCanJumpWhenFalling;
	bool myWillJumpWhenFalling;
	bool myActiveSpringJump;
	bool myGrabbedLedge;
	bool myIsLerpingToPosition;
	bool myHasDied;
	bool myIsOnPlatform;
	bool myIsGliding;
	bool myIsInRangeOfBash;
	bool myCheckParticleLanding;
	bool myIsSpawning;
	bool mySkipOneUpdate;

	PlayerData* myJsonData = new PlayerData();

	const void CheckParticleLanding();

};

