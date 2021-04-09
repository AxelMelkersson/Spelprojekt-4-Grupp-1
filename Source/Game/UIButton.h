#pragma once
#include "GameObject.h"
#include "..\External\Headers\CU\Vector4.hpp"

class Camera;
class Scene;
class Player;
class SpriteComponent;

class UIButton : public GameObject
{
public:
	UIButton(Scene* aLevelScene);
	~UIButton();

	void Init(const std::string aPathString, const v2f aSize, const v2f aPosition, const std::string aAnimationPathString, const int aBoundX, const std::string aLockedPathString = "", bool aIsUnlocked = true);

	void Update(const float& aDeltaTime) override;
	void UpdateButton(const float& aDeltaTime);
	void Render() override;

	void SetIsHighlightActive(const bool aHighlightBool);
	void SetActive(const bool aActiveState);

	void SetHighlightOffset(const v2f& aOffset);
	bool GetIsUnlocked() const;

private:
	Camera& myCamera;

	SpriteComponent* mySprite;

	bool myBtnHighlighted;
	bool myIsActive;
	bool myIsUnlocked;

	v4f myOriginalColor;
	v4f myHighlightedColor;

	v2f myPosition;

};

