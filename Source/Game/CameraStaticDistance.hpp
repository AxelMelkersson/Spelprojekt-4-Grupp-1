#pragma once

class Distance
{
public:
	static Distance& GetInstance()
	{
		static Distance instance;
		return instance;
	}

	void ResetDistance()
	{
		myBackgroundDistanceX = {};
	}

private:
	friend class Background;

	int myCurrentAreaIndex = -1;
	float myStartingCameraPos = {};
	float myBackgroundDistanceX = {};
	float myCloudDistance = {};
	bool myAreaIndexChanged = {};

};