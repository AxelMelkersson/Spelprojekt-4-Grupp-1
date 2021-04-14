/*
*	Author: Elia Rönning
*/

#include "stdafx.h"
#include "../External/Headers/CU/Utilities.h"
#include <Windows.h>
#include <winuser.h>
#include <iostream>

using namespace Utils;

Timer::Timer()
    : myClock(std::chrono::high_resolution_clock())
    , myStartTime(myClock.now().time_since_epoch().count())
    , myLastUpdate(myStartTime)
    , myCurrentTime(myStartTime)
{
    myTimeScale = 1.0f;
}

Timer& Timer::Update()
{
    myLastUpdate = myCurrentTime;
    myCurrentTime = myClock.now().time_since_epoch().count();

    return *this;
}

const float Timer::GetDeltaTime() const
{
    constexpr float maxDeltaTime = 0.20f;
    return Utils::Min((float)(myCurrentTime - myLastUpdate) * (float)ourInverseNanoSecondsToSeconds, maxDeltaTime) * myTimeScale;
}

const double Timer::GetTotalTime() const
{
    return (double)(myCurrentTime - myStartTime) * ourInverseNanoSecondsToSeconds;
}

const float Timer::GetTimeScale()
{
    return myTimeScale;
}

void Timer::SetTimeScale(const float& aTimeScale)
{
    myTimeScale = aTimeScale;
}

const std::map<Keys, std::string> Input::ourKeyToString =
{
    {Keys::LeftMouseButton, "LeftMouseButton"},
    {Keys::RightMouseButton, "RightMouseButton"},
    {Keys::ControlBreakProcessing, "ControlBreakProcessing"},
    {Keys::MiddleMouseButton, "MiddleMouseButton"},
    {Keys::X1MouseButton, "X1MouseButton"},
    {Keys::X2MouseButton, "X2MouseButton"},
    {Keys::BACKSPACEKey, "BACKSPACEKey"},
    {Keys::TABKey, "TABKey"},
    {Keys::CLEARKey, "CLEARKey"},
    {Keys::ENTERKey, "ENTERKey"},
    {Keys::SHIFTKey, "SHIFTKey"},
    {Keys::CTRLKey, "CTRLKey"},
    {Keys::ALTKey, "ALTKey"},
    {Keys::PAUSEKey, "PAUSEKey"},
    {Keys::CAPSLOCKKey, "CAPSLOCKKey"},
    {Keys::IMEKanaMode, "IMEKanaMode"},
    {Keys::IMEHanguelMode, "IMEHanguelMode"},
    {Keys::IMEHangulMode, "IMEHangulMode"},
    {Keys::IMEOn, "IMEOn"},
    {Keys::IMEJunjaMode, "IMEJunjaMode"},
    {Keys::IMEFinalMode, "IMEFinalMode"},
    {Keys::IMEHanjaMode, "IMEHanjaMode"},
    {Keys::IMEKanjiMode, "IMEKanjiMode"},
    {Keys::IMEOff, "IMEOff"},
    {Keys::ESCKey, "ESCKey"},
    {Keys::IMEConvert, "IMEConvert"},
    {Keys::IMENonconvert, "IMENonconvert"},
    {Keys::IMEAccept, "IMEAccept"},
    {Keys::IMEModeChangeRequest, "IMEModeChangeRequest"},
    {Keys::SPACEBAR, "SPACEBAR"},
    {Keys::PAGEUPKey, "PAGEUPKey"},
    {Keys::PAGEDOWNKey, "PAGEDOWNKey"},
    {Keys::ENDKey, "ENDKey"},
    {Keys::HOMEKey, "HOMEKey"},
    {Keys::LEFTARROWKey, "LEFTARROWKey"},
    {Keys::UPARROWKey, "UPARROWKey"},
    {Keys::RIGHTARROWKey, "RIGHTARROWKey"},
    {Keys::DOWNARROWKey, "DOWNARROWKey"},
    {Keys::SELECTKey, "SELECTKey"},
    {Keys::PRINTKey, "PRINTKey"},
    {Keys::EXECUTEKey, "EXECUTEKey"},
    {Keys::PRINTSCREENKey, "PRINTSCREENKey"},
    {Keys::INSKey, "INSKey"},
    {Keys::DELKey, "DELKey"},
    {Keys::HELPKey, "HELPKey"},
    {Keys::Key0, "Key0"},
    {Keys::Key1, "Key1"},
    {Keys::Key2, "Key2"},
    {Keys::Key3, "Key3"},
    {Keys::Key4, "Key4"},
    {Keys::Key5, "Key5"},
    {Keys::Key6, "Key6"},
    {Keys::Key7, "Key7"},
    {Keys::Key8, "Key8"},
    {Keys::Key9, "Key9"},
    {Keys::AKey, "AKey"},
    {Keys::BKey, "BKey"},
    {Keys::CKey, "CKey"},
    {Keys::DKey, "DKey"},
    {Keys::EKey, "EKey"},
    {Keys::FKey, "FKey"},
    {Keys::GKey, "GKey"},
    {Keys::HKey, "HKey"},
    {Keys::IKey, "IKey"},
    {Keys::JKey, "JKey"},
    {Keys::KKey, "KKey"},
    {Keys::LKey, "LKey"},
    {Keys::MKey, "MKey"},
    {Keys::NKey, "NKey"},
    {Keys::OKey, "OKey"},
    {Keys::PKey, "PKey"},
    {Keys::QKey, "QKey"},
    {Keys::RKey, "RKey"},
    {Keys::SKey, "SKey"},
    {Keys::TKey, "TKey"},
    {Keys::UKey, "UKey"},
    {Keys::VKey, "VKey"},
    {Keys::WKey, "WKey"},
    {Keys::XKey, "XKey"},
    {Keys::YKey, "YKey"},
    {Keys::ZKey, "ZKey"},
    {Keys::LeftWindowsKey, "LeftWindowsKey"},
    {Keys::RightWindowsKey, "RightWindowsKey"},
    {Keys::ApplicationsKey, "ApplicationsKey"},
    {Keys::ComputerSleepKey, "ComputerSleepKey"},
    {Keys::NumericKeypad0Key, "NumericKeypad0Key"},
    {Keys::NumericKeypad1Key, "NumericKeypad1Key"},
    {Keys::NumericKeypad2Key, "NumericKeypad2Key"},
    {Keys::NumericKeypad3Key, "NumericKeypad3Key"},
    {Keys::NumericKeypad4Key, "NumericKeypad4Key"},
    {Keys::NumericKeypad5Key, "NumericKeypad5Key"},
    {Keys::NumericKeypad6Key, "NumericKeypad6Key"},
    {Keys::NumericKeypad7Key, "NumericKeypad7Key"},
    {Keys::NumericKeypad8Key, "NumericKeypad8Key"},
    {Keys::NumericKeypad9Key, "NumericKeypad9Key"},
    {Keys::MultiplyKey, "MultiplyKey"},
    {Keys::AddKey, "AddKey"},
    {Keys::SeparatorKey, "SeparatorKey"},
    {Keys::SubtractKey, "SubtractKey"},
    {Keys::DecimalKey, "DecimalKey"},
    {Keys::DivideKey, "DivideKey"},
    {Keys::F1Key, "F1Key"},
    {Keys::F2Key, "F2Key"},
    {Keys::F3Key, "F3Key"},
    {Keys::F4Key, "F4Key"},
    {Keys::F5Key, "F5Key"},
    {Keys::F6Key, "F6Key"},
    {Keys::F7Key, "F7Key"},
    {Keys::F8Key, "F8Key"},
    {Keys::F9Key, "F9Key"},
    {Keys::F10Key, "F10Key"},
    {Keys::F11Key, "F11Key"},
    {Keys::F12Key, "F12Key"},
    {Keys::F13Key, "F13Key"},
    {Keys::F14Key, "F14Key"},
    {Keys::F15Key, "F15Key"},
    {Keys::F16Key, "F16Key"},
    {Keys::F17Key, "F17Key"},
    {Keys::F18Key, "F18Key"},
    {Keys::F19Key, "F19Key"},
    {Keys::F20Key, "F20Key"},
    {Keys::F21Key, "F21Key"},
    {Keys::F22Key, "F22Key"},
    {Keys::F23Key, "F23Key"},
    {Keys::F24Key, "F24Key"},
    {Keys::NUMLOCKKey, "NUMLOCKKey"},
    {Keys::SCROLLLOCKKey, "SCROLLLOCKKey"},
    {Keys::LeftSHIFTKey, "LeftSHIFTKey"},
    {Keys::RightSHIFTKey, "RightSHIFTKey"},
    {Keys::LeftCONTROLKey, "LeftCONTROLKey"},
    {Keys::RightCONTROLKey, "RightCONTROLKey"},
    {Keys::LeftMENUKey, "LeftMENUKey"},
    {Keys::RightMENUKey, "RightMENUKey"},
    {Keys::BrowserBackKey, "BrowserBackKey"},
    {Keys::BrowserForwardKey, "BrowserForwardKey"},
    {Keys::BrowserRefreshKey, "BrowserRefreshKey"},
    {Keys::BrowserStopKey, "BrowserStopKey"},
    {Keys::BrowserSearchKey, "BrowserSearchKey"},
    {Keys::BrowserFavoritesKey, "BrowserFavoritesKey"},
    {Keys::BrowserStartAndHomeKey, "BrowserStartAndHomeKey"},
    {Keys::VolumeMuteKey, "VolumeMuteKey"},
    {Keys::VolumeDownKey, "VolumeDownKey"},
    {Keys::VolumeUpKey, "VolumeUpKey"},
    {Keys::NextTrackKey, "NextTrackKey"},
    {Keys::PreviousTrackKey, "PreviousTrackKey"},
    {Keys::StopMediaKey, "StopMediaKey"},
    {Keys::PlayPauseMediaKey, "PlayPauseMediaKey"},
    {Keys::StartMailKey, "StartMailKey"},
    {Keys::SelectMediaKey, "SelectMediaKey"},
    {Keys::StartApplication1Key, "StartApplication1Key"},
    {Keys::StartApplication2Key, "StartApplication2Key"},
    {Keys::ForAnyCountryRegionThePlusKey, "ForAnyCountryRegionThePlusKey"},
    {Keys::ForAnyCountryRegionTheCommaKey, "ForAnyCountryRegionTheCommaKey"},
    {Keys::ForAnyCountryRegionTheMinusKey, "ForAnyCountryRegionTheMinusKey"},
    {Keys::ForAnyCountryRegionThePeriodKey, "ForAnyCountryRegionThePeriodKey"},
    {Keys::EitherTheAngleBracketKeyOrTheBackslashKeyOnTheRT102KeyKeyboard, "EitherTheAngleBracketKeyOrTheBackslashKeyOnTheRT102KeyKeyboard"},
    {Keys::IMEPROCESSKey, "IMEPROCESSKey"},
    {Keys::AttnKey, "AttnKey"},
    {Keys::CrSelKey, "CrSelKey"},
    {Keys::ExSelKey, "ExSelKey"},
    {Keys::EraseEOFKey, "EraseEOFKey"},
    {Keys::PlayKey, "PlayKey"},
    {Keys::ZoomKey, "ZoomKey"},
    {Keys::PA1Key, "PA1Key"},
    {Keys::ClearKey, "ClearKey"},
};
const std::string Input::ourUndefined = "Undefined";

Input::Input()
    : myMouseIsCaptured(false)
{
    myLastMousePosition = { 0, 0 };
    myMousePosition = { 0, 0 };

    for (int index = 0; index != static_cast<int>(Keys::ClearKey); ++index)
    {
        myKeyMap[static_cast<Keys>(index)] = false;
        myLastKeyMap[static_cast<Keys>(index)] = false;
        myMessageMap[static_cast<Keys>(index)] = false;
    }
}

bool Input::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam)
{
    lParam;

    switch (message)
    {
        case WM_KEYDOWN:
        {
            myMessageMap[static_cast<Keys>(wParam)] = true;

            return true;
        }
        case WM_KEYUP:
        {
            myMessageMap[static_cast<Keys>(wParam)] = false;

            return true;
        }
        case WM_LBUTTONDOWN:
        {
            myMessageMap[Keys::LeftMouseButton] = true;

            return true;
        }
        case WM_LBUTTONUP:
        {
            myMessageMap[Keys::LeftMouseButton] = false;

            return true;
        }
        case WM_RBUTTONDOWN:
        {
            myMessageMap[Keys::RightMouseButton] = true;

            return true;
        }
        case WM_RBUTTONUP:
        {
            myMessageMap[Keys::RightMouseButton] = false;

            return true;
        }
        case WM_MBUTTONDOWN:
        {
            myMessageMap[Keys::MiddleMouseButton] = true;

            return true;
        }
        case WM_MBUTTONUP:
        {
            myMessageMap[Keys::MiddleMouseButton] = false;

            return true;
        }
    }

    return false;
}

Input& Input::Update()
{
    HWND hWnd = GetActiveWindow();

    for (int index = 0; index != static_cast<int>(Keys::ClearKey); ++index)
    {
        const Keys key = static_cast<Keys>(index);
        myLastKeyMap[key] = myKeyMap[key];
        myKeyMap[key] = myMessageMap[key];
    }

    myLastMousePosition = myMousePosition;

    POINT cursorPos = { 0, 0 };

    GetCursorPos(&cursorPos);
    ScreenToClient(hWnd, &cursorPos);

    myMousePosition.x = cursorPos.x;
    myMousePosition.y = cursorPos.y;

    return *this;
}

bool Input::GetKeyDown(const Keys& aKey)
{
    if (!ValidateKey(aKey)) return false;
    return myKeyMap[aKey];
}

bool Input::GetKeyJustDown(const Keys& aKey)
{
    if (!ValidateKey(aKey)) return false;
    return myKeyMap[aKey] && !myLastKeyMap[aKey];
}

bool Input::GetKeyJustUp(const Keys& aKey)
{
    if (!ValidateKey(aKey)) return false;
    return !myKeyMap[aKey] && myLastKeyMap[aKey];
}

bool Input::ValidateKey(const Keys& aKey)
{
    return myKeyMap.count(aKey) == 1 || myLastKeyMap.count(aKey) ? true : false;
}

const std::string Input::KeyToString(const Keys& aKey)
{
    if (ourKeyToString.count(aKey) == 0) return ourUndefined;
    return ourKeyToString.at(aKey);
}

POINT Input::GetMousePosition() const
{
    return myMousePosition;
}

POINT Input::GetMouseMovementSinceLastUpdate() const
{
    POINT returnValue;
    returnValue.x = myMousePosition.x - myLastMousePosition.x;
    returnValue.y = myMousePosition.y - myLastMousePosition.y;
    return returnValue;
}
Input& Input::SetAbsoluteMousePosition(const POINT& aPoint)
{
    SetCursorPos(aPoint.x, aPoint.y);

    return *this;
}
Input& Input::SetMousePosition(const POINT& aPoint)
{
    HWND hWnd = GetActiveWindow();

    POINT point = aPoint;
    ClientToScreen(hWnd, &point);
    SetCursorPos(point.x, point.y);

    return *this;
}

Input& Input::ToggleCaptureCursor(const bool aIsCapturing)
{
    if (myMouseIsCaptured && !aIsCapturing)
    {
        ClipCursor(nullptr);
        myMouseIsCaptured = false;
    }
    else
    {
        HWND hWnd = GetActiveWindow();

        RECT rect;
        GetClientRect(hWnd, &rect);

        POINT upperLeft;
        upperLeft.x = rect.left;
        upperLeft.y = rect.top;
        POINT lowerRight;
        lowerRight.x = rect.right;
        lowerRight.y = rect.bottom;

        MapWindowPoints(hWnd, nullptr, &upperLeft, 1);
        MapWindowPoints(hWnd, nullptr, &lowerRight, 1);

        rect.left = upperLeft.x;
        rect.top = upperLeft.y;
        rect.right = lowerRight.x;
        rect.bottom = lowerRight.y;

        ClipCursor(&rect);

        myMouseIsCaptured = true;
    }

    return *this;
}