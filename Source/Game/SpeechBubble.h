#pragma once
#include "GameObject.h" 

class Scene;

class SpeechBubble : public GameObject
{
public:
	SpeechBubble(Scene* aScene);

	void Init(const int aBonfireID, const v2f aPos);
	void Speak();

private:
	void ParseText(const int aIndex);
	std::vector<std::string> SplitText(std::string someText);

	std::vector<std::string> myText;
};

