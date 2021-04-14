#pragma once
#include "Controller.h"
#include "../External/Headers/CU/Utilities.h"



class InputWrapper
{
public:

	InputWrapper();
	~InputWrapper() = default;

	void Init();

	void Update(const float& aDeltaTime);

	float GetLeftPullForce();
	float GetRightPullForce();

	v2f GetLeftStickMovement();
	v2f GetRightStickMovement();

	v2f GetMouseAxisMovement();
	v2f GetAxisMovement();

	bool IsMovingUp();
	bool IsMovingLeft();
	bool IsMovingDown();
	bool IsMovingRight();
	bool IsJumping();
	bool IsDashing();
	bool IsDashingReleased();

	void ReleaseCursor();

	//void CalculateAxis();
	

	std::shared_ptr<Utils::Input> GetInput();
	std::shared_ptr<Controller> GetController();

private:
	std::shared_ptr<Utils::Input> myInput;
	std::shared_ptr<Controller> myController;

	bool myHoldDash;

	POINT myCursor;

	v2f myScreenSize;

	v2f myMouseDirection;
	v2f myPreviousMousePosition;
	v2f myNewMousePosition;
	v2f myNormalizedDirection;

	void SetCursor();
	void CheckMousePosition(const float& aDeltaTime);
	void SetMousePosition();
	void CalculateMouseAxis();
	void SetCursorMiddle();

};

