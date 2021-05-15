#pragma once

namespace Utils
{
	class Input;
	class Timer;
}

class LevelScene;
class InputWrapper;
class Player;

class BashAbility : public GameObject
{
public:
	BashAbility(LevelScene* aLevelScene);

	void Init();

	void Update(const float& aDeltaTime);
	void UpdateBashVelocity(const float& aDeltaTime);

	v2f GetVelocity();
	void SetVelocity(const v2f& aDashVelocity);
	void InvertDashDirectionX();

	void ResetVelocity(const bool aResetX, const bool aResetY);

	void AddPlayerRelation(Player* aPlayer);
	void AddInputWrapper(const std::shared_ptr<InputWrapper> aInputWrapper);
	void AddTimer(Utils::Timer* aTimer);

	const bool GetIsBashing();
	const bool GetDashIsActive();

	void ActivateBash(GameObject* aGameObject);

	void UpdateBashArrow();

	void ImGuiUpdate();

	void StopBashing();

private:
	std::shared_ptr<InputWrapper> myInput;
	
	GameObject* myBashObject;
	Player* myPlayer;
	Utils::Timer* myTimerInput;

	float myTimeScale;

	bool myDashAbilityActive;
	bool myButtonHold;
	bool myRadiusFromDash;
	bool myIsBashing;
	bool myLMBMousePressed;

	float myTimer;
	float myDashDuration;
	float myMaxDashDuration;
	float myMaxDashDurationTimer;
	float myDashSpeed;
	float myAcceleration;
	float myRetardation;

	int myVibrationStrength;
	float myVibrationLength;

	float myDashShakeDuration;
	float myDashShakeIntensity;
	float myDashShakeDropOff;

	float myAspectRatioFactorY;

	v2f myDashDirection;
	v2f myCurrentDashVelocity;
	v2f myUsedDashDirection;

	void FreezeTime();
	void DashUse(const float& aDeltaTime);
	void UseBashAbility(const float& aDeltaTime);
	void CheckButtonPress();

};

