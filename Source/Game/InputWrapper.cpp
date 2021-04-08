#include <memory>
#include "stdafx.h"
#include "InputWrapper.h"
#include <iostream>

#include "GameWorld.h"

#include "../External/Headers/CU/Utilities.h"

InputWrapper::InputWrapper()
{
	myController = {};
	myInput = {};
	myHoldDash = {};
	myMouseDirection = {};
}

void InputWrapper::Init()
{
	//myMouseSensitivityX = 4.f;
	//myMouseSensitivityY = 4.f;
	myController = std::make_shared<Controller>();
	myInput = std::make_shared<Utils::Input>();
	myController->Init();
}

void InputWrapper::Update(const float& aDeltaTime)
{
	GetCursorPos(&myCursor);

	myInput->Update();
	myController->Update();
	CheckMousePosition(aDeltaTime);
	CalculateMouseAxis();
}

float InputWrapper::GetLeftPullForce()
{
	return std::abs(myController->LeftThumbStickPull());
}

float InputWrapper::GetRightPullForce()
{
	return std::abs(myController->RightThumbStickPull());
}

v2f InputWrapper::GetLeftStickMovement()
{
	return myController->GetLeftThumbStick();
}

v2f InputWrapper::GetRightStickMovement()
{
	return myController->GetRightThumbStick();
}

v2f InputWrapper::GetMouseAxisMovement()
{
	//return myMouseDirection;
	return myNormalizedDirection;
}

v2f InputWrapper::GetAxisMovement()
{
	if (GetLeftStickMovement().x != 0.0f || GetLeftStickMovement().y != 0.0f)
	{
		return GetLeftStickMovement();
	}
	else
		return GetMouseAxisMovement();
}

bool InputWrapper::IsMovingUp()
{
	if (myInput->GetKeyDown(Keys::WKey) || GetController()->GetLeftThumbStick().y < 0.0f || GetController()->IsButtonHoldDown(Controller::Button::DPadUp))
		return true;
	else
		return false;
}

bool InputWrapper::IsMovingLeft()
{
	if (myInput->GetKeyDown(Keys::AKey) || GetController()->GetLeftThumbStick().x < -0.35f || GetController()->IsButtonHoldDown(Controller::Button::DPadLeft))
		return true;
	else
		return false;
}

bool InputWrapper::IsMovingDown()
{
	if (myInput->GetKeyDown(Keys::SKey) || GetController()->GetLeftThumbStick().y > 0.9f || GetController()->IsButtonHoldDown(Controller::Button::DPadDown))
		return true;
	else
		return false;
}

bool InputWrapper::IsMovingRight()
{
	if (myInput->GetKeyDown(Keys::DKey) || GetController()->GetLeftThumbStick().x > 0.35f || GetController()->IsButtonHoldDown(Controller::Button::DPadRight))
		return true;
	else
		return false;
}


bool InputWrapper::IsJumping()
{
	if (GetInput()->GetKeyJustDown(Keys::SPACEBAR) || GetInput()->GetKeyJustDown(Keys::WKey) ||GetController()->IsButtonPressed(Controller::Button::Cross))
		return true;
	else
		return false;
}

bool InputWrapper::IsDashing()
{
	if (GetInput()->GetKeyDown(Keys::LeftMouseButton) || GetController()->IsButtonHoldDown(Controller::Button::Square))
	{
		if (!myHoldDash)
		{
			SetCursorMiddle();
			myHoldDash = true;
			SetMousePosition();
		}

		return true;
	}
	else
	{
		myNormalizedDirection = {};
		myHoldDash = false;
		return false;
	}
}

bool InputWrapper::IsDashingReleased()
{
	if (GetInput()->GetKeyJustUp(Keys::LeftMouseButton) || !GetController()->IsButtonHoldDown(Controller::Button::Square))
	{
		return true;
	}
	else
		return false;
}

std::shared_ptr<Utils::Input> InputWrapper::GetInput()
{
	return myInput;
}

std::shared_ptr<Controller> InputWrapper::GetController()
{
	return myController;
}

void InputWrapper::SetCursor()
{
	HMONITOR monitor = MonitorFromWindow(*Tga2D::CEngine::GetInstance()->GetHWND(), MONITOR_DEFAULTTONEAREST);
	MONITORINFO info;
	info.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(monitor, &info);

	int pixels = 30;
	int width = static_cast<int>(info.rcMonitor.right);
	int height = static_cast<int>(info.rcMonitor.bottom);

	myScreenSize.x = static_cast<float>(width);
	myScreenSize.y = static_cast<float>(height);

	if (myCursor.x >= width - pixels || myCursor.y >= height - pixels || myCursor.x <= pixels || myCursor.y <= pixels)
	{
		SetCursorMiddle();
		SetMousePosition();
	}

}

void InputWrapper::CheckMousePosition(const float& aDeltaTime)
{
	if (myHoldDash)
	{
		SetCursor();
	}
}

void InputWrapper::SetMousePosition()
{
	myPreviousMousePosition.x = static_cast<float>(myCursor.x);
	myPreviousMousePosition.y = static_cast<float>(myCursor.y);
}


void InputWrapper::CalculateMouseAxis()
{
	myNewMousePosition.x = static_cast<float>(myCursor.x);
	myNewMousePosition.y = static_cast<float>(myCursor.y);

	v2f mouseDistance = myNewMousePosition - myPreviousMousePosition;

	const float timeScale = CGameWorld::GetInstance()->GetTimer()->GetTimeScale();

	CGameWorld::GetInstance()->GetTimer()->SetTimeScale(1.0f);
	const float deadZone = 0.001f * CGameWorld::GetInstance()->GetTimer()->GetDeltaTime();
	CGameWorld::GetInstance()->GetTimer()->SetTimeScale(timeScale);

	if (mouseDistance.LengthSqr() >= deadZone * deadZone)
	{
		myNormalizedDirection = mouseDistance.GetNormalized();
	}

	myNewMousePosition = {};
}

void InputWrapper::SetCursorMiddle()
{
	HMONITOR monitor = MonitorFromWindow(*Tga2D::CEngine::GetInstance()->GetHWND(), MONITOR_DEFAULTTONEAREST);
	MONITORINFO info;
	info.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(monitor, &info);

	int pixels = 30;
	int width = static_cast<int>(info.rcMonitor.right);
	int height = static_cast<int>(info.rcMonitor.bottom);

	SetCursorPos(width / 2.f, height / 2.f);
	myCursor.x = width / 2.f;
	myCursor.y = height / 2.f;
}
