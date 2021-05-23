#pragma once
#include "GameObject.h"
#include "EFontsSizes.hpp"

class TextComponent;
class Scene;

class UIText : public GameObject
{
public:



	UIText(Scene* aLevelScene);

	void Init(const std::string aTextString, const char* aPathAndName = "Text/alagard.ttf", EFontSize aFontSize = EFontSize::EFontSize_72, unsigned char aBorderSize = 0);

	void SetPosition(const v2f aPosition);
	void SetTga2dPosition(const v2f aPosition);

	const v2f GetPosition();

	void SetText(const std::string& aTextString);

	float GetWidth();
	float GetHeight();

private:
	TextComponent* myText;

	
};

