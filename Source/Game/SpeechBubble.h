#pragma once
#include "GameObject.h" 

class Scene;
class TextComponent;

class SpeechBubble : public GameObject
{
public:
	SpeechBubble(Scene* aScene);

	void Init(const int aBonfireID, const v2f aPos);
	void Update(const float& aDeltaTime) override;

	void Speak();
	bool GetIsSpeaking();

private:
	void ParseText(const int aIndex);
	std::vector<std::string> SplitText(std::string someText);

	bool myIsSpeaking;
	bool myHasFadedIn;
	bool myHasFadedOut;
	unsigned int myRowNumber;
	float myAlpha;
	float myFadeSpeed;

	std::vector<std::string> myText;
	std::vector<TextComponent*> myTextComps;
};

