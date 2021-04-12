#pragma once
#include <fstream>
#include <string>
#include "GameWorld.h"

#ifndef _RETAIL
#include "Debugger.h"
#endif

namespace std
{
	class thread;
}

class Config
{
public:
	Config() = delete;
	Config(const Config& aConfig) = delete;
	~Config() = delete;

	static uint16_t width;
	static uint16_t height;
	static uint16_t windowWidth;
	static uint16_t windowHeight;
	static std::wstring appName;
	static v2f ourReferenceSize;
};

class CGame
{
public:
	CGame();
	~CGame();
	bool Init(const std::wstring& aVersion = L"", HWND aHWND = nullptr);

	static void SetResolution(const uint16_t& aWidth, const uint16_t& aHeight);
	void SetZoom(const int aZoomX, const int aZoomY)
	{
		myZoomX = aZoomX;
		myZoomY = aZoomY;
	}
	int GetZoomX()
	{
		return myZoomX;
	}
	int GetZoomY()
	{
		return myZoomY;
	}

	void UpdateWindowSize(const uint16_t& aWidth, const uint16_t& aHeight);

private:
	void InitCallBack();
	void UpdateCallBack();
	LRESULT WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	CGameWorld myGameWorld;

	Utils::Timer* myTimer;

	int myZoomX = 0;
	int myZoomY = 0;

	int myGotOutOfFocusSizeX = 0;
	int myGotOutOfFocusSizeY = 0;

	int myMonitorSizeX = 0;
	int myMonitorSizeY = 0;

#ifndef _RETAIL
	void InitDebugger();
	Debugger myDebugger;
#endif
};
