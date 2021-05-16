#include "stdafx.h"
#include "SpeedrunManager.h"
#include "DataManager.h"
#include <assimp\StringUtils.h>

SpeedrunManager::SpeedrunManager() :
	myIsSpeedrun(false),
	myIsAlreadyInRun(false),
	myCurrentScore(0.f)
{
	myIsUnlocked = DataManager::GetInstance().GetBonfireState(7);
	std::array<float, 10> data = DataManager::GetInstance().GetHighScores();

	for (int i = 0; i < myHighScores.size(); ++i)
	{
		if (data[i] > 0.01)
		{
			myHighScores[i] = data[i];
		}
		else
		{
			myHighScores[i] = 0.0f;
		}
	}
}

void SpeedrunManager::SetIsSpeedrun(const bool aIsSpeedrun)
{
	myIsSpeedrun = aIsSpeedrun;
	if (myIsSpeedrun == false)
	{
		myCurrentScore = 0;
		myIsAlreadyInRun = false;
	}
}
bool SpeedrunManager::GetIsSpeedrun()
{
	return myIsSpeedrun;
}
bool SpeedrunManager::GetIsUnlocked()
{
	return myIsUnlocked;
}
float SpeedrunManager::GetScore()
{
	return myCurrentScore;
}

void SpeedrunManager::Unlock()
{
	myIsUnlocked = true;
}

void SpeedrunManager::SetScore(const float aScore)
{
	myCurrentScore = aScore;
}
void SpeedrunManager::AddToScore(const float aScore)
{
	myCurrentScore += aScore;
}

void SpeedrunManager::ReportScoreToHighscores()
{
	for (int i = 0; i < myHighScores.size(); ++i)
	{
		if (myCurrentScore < myHighScores[i] || myHighScores[i] < 0.01)
		{
			float temp = myHighScores[i];
			myHighScores[i] = myCurrentScore;
			myCurrentScore = temp;
		}
	}
	myCurrentScore = 0;

	DataManager::GetInstance().SaveHighScores(myHighScores);
}

std::array<float, 10> SpeedrunManager::GetHighscores()
{
	return myHighScores;
}

float SpeedrunManager::GetBestHighscore()
{
	return myHighScores[0];
}

std::string SpeedrunManager::GetTimeOutput(float aTime) const
{
	int intMin = aTime / 60;
	float floatSec = floorf((aTime - 60 * static_cast<float>(intMin)) * 100) / 100;
	std::string printTime;
	std::string min;
	std::string sec;

	if (intMin < 10)
	{
		min = "0" + to_string(intMin);
	}
	else
	{
		min = to_string(intMin);
	}
	if (floatSec < 10)
	{
		sec = "0" + to_string(floatSec);
	}
	else
	{
		sec = to_string(floatSec);
	}
	printTime = min + "." + sec;

	return printTime;
}
const bool SpeedrunManager::GetIsAlreadyInRun() const
{
	return myIsAlreadyInRun;
}
void SpeedrunManager::SetIsAlreadyInRun(const bool aState)
{
	myIsAlreadyInRun = aState;
}