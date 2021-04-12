#pragma once
#include "GameObject.h"

class Camera;
class SpriteComponent;


class UIObject : public GameObject
{
public:
	UIObject(Scene* aLevelScene);
	
	void Init(const std::string aPathString, const v2f aSize, const v2f aPosition, int aZIndex);
	void InitAnimation(const std::string aPathString, const v2f aSize, const v2f aPosition, int aZIndex);

	void SetPosition(const v2f& aPosition);

	void SetPositionX(const float& aPositionX);
	void SetPositionY(const float& aPositionY);

	void UpdateUIObjects(const float& aDeltaTime);
	void Render() override;

	void SetActive(const bool aActiveState);

	const v2f GetStartPosition();


private:
	Camera* myCamera;

	v2f myStartPosition;

	bool myIsActive;

};

