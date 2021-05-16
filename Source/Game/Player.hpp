#pragma once
#include "GameObject.h"
#include "Animation.hpp"

namespace Utils
{
	class Input;
	class Timer;
}

class Ledge;
class LevelScene;
class BashAbility;
class Collectible;
class InputWrapper;
class SpringObject;
class BashComponent;
class BashableObject;
class AnimationComponent;
struct PlayerData;

class Player : public GameObject
{
public:
	Player(LevelScene* aLevelScene);

	void Update(const float& aDeltaTime) override;

	void DoubleJump();
	void LedgeJump();
	void ReactivateDoubleJump();
	void TryLetJumpWhenFalling(const float& aYDistance);

	void GoRight(const float& aDeltaTime);
	void GoLeft(const float& aDeltaTime);
	void StartGliding();

	void Kill();
	void Landed(const int& aOverlapY) override;
	void SideCollision(const int& aOverlapX) override;

	void ResetVelocity();
	void RedirectVelocities(const v2f& aDirection);

	const v2f GetPlatformVelocity() const;
	const bool GetLedgeIsGrabbed() const;
	const bool GetIsGliding() const;
	const bool& GetHasLanded() const;
	const bool GetHasDied() const;
	const bool& GetIsBashing() const;
	const v2f GetCurrentVelocity() const;

	void SetPlatformVelocity(const v2f& aPlatformVelocity);
	void SetLerpPosition(const v2f& aPosition);
	void SetAnimation(const int& aAnimationIndex);
	void SetNextAnimation(const int& aAnimationIndex);
	void SetSpawnPosition(const v2f& aSpawnPosition);

	void GrabLedge(const v2f& aLedgeLerpPosition, const v2f& aLedgePosition);
	void LeaveLedge();
	void EndLerp();

	void ActivateSpringForce(float mySpringVelocity, const float aRetardation, const bool aShouldResetVelocity);
	void BounceOnDestructibleWall();
	void BashCollision(GameObject* aGameObject, BashComponent* aBashComponent) override;

	void PlayFootSteps(const int& aPlatformIndex);
	void PlayLandingSounds(const int& aPlatformIndex);

	void SpawnAnimation();
	void StopSpawn();

private:
	void InitCollider();
	void InitAnimations();
	void UpdateCoyoteTime(const float& aDeltaTime);
	void UpdatePlayerVelocity(const float& aDeltaTime);
	void Jump();
	void Respawn();
	void KillReset();
	void UnlockLandingSounds();
	const void CheckJump();
	const void CheckParticleLanding();
	const void CheckMove(const float& aDeltaTime);
	const void AnimationState();
	const void LerpToPosition(const v2f& aPosition);
	const void DecreaseSpringJump(const float& aDeltaTime);

	Animation myAnimations[15];
	std::vector<Collectible*> myCollectibles;

	BashAbility* myBashAbility;
	BashableObject* myBashableObject;
	AnimationComponent* myAnimationComponent;
	std::shared_ptr<InputWrapper> myInputHandler;

	Utils::Timer* myTimerInput;

	v2f mySize;
	v2f myLerpPosition;
	v2f mySpawnPosition;
	v2f mySpringVelocity;
	v2f myCurrentVelocity;
	v2f myPlatformVelocity;

	float myResetTimer;
	float mySpringTimer;
	float myGlideFactor;
	float myAirCoyoteTimer;
	float myPercentageLeftVelocity;
	float mySpringVelocityRetardation;
	float myTriggerRunningAnimationSpeed;

	int myDirectionX;
	int myLedgeSoundIndex;
	int myCurrentAnimationIndex;

	bool myHasDied;
	bool myHasLanded;
	bool myIsGliding;
	bool myIsSpawning;
	bool myGrabbedLedge;
	bool mySkipOneUpdate;
	bool myIsInRangeOfBash;
	bool myHasDoubleJumped;
	bool myActiveSpringJump;
	bool myHasLandedOnSpring;
	bool myHasLandedVibration;
	bool myCanJumpWhenFalling;
	bool myWillJumpWhenFalling;
	bool myIsLerpingToPosition;
	bool myCheckParticleLanding;

	PlayerData* myJsonData;

#ifdef _DEBUG
	void ImGuiUpdate();
#endif // _DEBUG
};