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
	void Fade(const bool aIsFadingIn, const float& aDeltaTime);

	bool myIsSpeaking;
	bool myHasFadedIn;
	bool myHasFadedOut;
	bool myPlayerIsStandingByTheBonfire;
	unsigned int myRowNumber;
	float myAlpha;
	const float myFadeSpeed;
	const float mySpeakTime;
	float myTimeIHasSpoken;

	std::vector<std::string> myText;
	std::vector<TextComponent*> myTextComps;
};

