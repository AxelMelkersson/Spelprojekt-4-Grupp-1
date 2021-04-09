#pragma once
#include "GameObject.h" 

class SpeechBubble : public GameObject
{
public:

	void Init(std::vector<std::string> someMessages);
	//void Update(const float& aDeltaTime) override;
	//void Render() override;

private:
	std::vector<std::string> myMessages;
};

