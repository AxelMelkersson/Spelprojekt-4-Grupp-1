#pragma once
#include <array>

class SpeedrunManager
{
public:
	SpeedrunManager();
	void SetIsSpeedrun(const bool aIsSpeedrun);
	bool GetIsSpeedrun();

	bool GetIsUnlocked();
	void Unlock();

	float GetScore();
	void SetScore(const float aScore);
	void AddToScore(const float aScore);	
	void ReportScoreToHighscores();

	std::array<float, 10> GetHighscores();
	float GetBestHighscore();

	std::string GetTimeOutput(float aTime) const;

private:
	bool myIsSpeedrun;
	bool myIsUnlocked;
	float myCurrentScore;
	std::array<float, 10> myHighScores;
};

