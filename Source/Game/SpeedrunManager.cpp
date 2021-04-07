#include "stdafx.h"
#include "SpeedrunManager.h"
#include "DataManager.h"

SpeedrunManager::SpeedrunManager()
{
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

void SpeedrunManager::Unlock()
{
	myIsUnlocked = true;
}

float SpeedrunManager::GetScore()
{
	return myCurrentScore;
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

