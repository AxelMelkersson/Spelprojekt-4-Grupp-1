#pragma once
#include "GameObject.h"
#include "rapidjson/document.h"

class Camera;
class SpriteComponent;
class LevelScene;
class Player;

class Background : GameObject
{
public:
	Background(Scene* aLevelScene);
	~Background();

	void LoadBackground(const int aIndex);
	void Update(const float& aDeltaTime) override;

private:
	Camera* myCamera;
	Player* myPlayer;
	LevelScene* myLevelScene;

	GameObject* myBackgroundSprite1;
	GameObject* myBackgroundSprite2;
	GameObject* myBackgroundSprite3;
	GameObject* myBackgroundSprite4;
	GameObject* myBackgroundSprite5;
	GameObject* myBackgroundSprite6;
	GameObject* myBackgroundSprite7;

	v2f myCurrentRenderSize;
	v2f myStartingCameraPos;
	
	v2f myOffsetBackground1;
	v2f myOffsetBackground2;
	v2f myOffsetBackground3;
	v2f myOffsetBackground4;
	v2f myOffsetBackground5;
	v2f myOffsetBackground6;
	v2f myOffsetBackground7;

	float myCloudSpeed;

	float myTotalCameraDistanceX;

	float* myCloudDistance;
	float* myBackgroundDistanceX;

	float myBackgroundSpeedOneX;
	float myBackgroundSpeedTwoX;
	float myBackgroundSpeedThreeX;
	float myBackgroundSpeedFourX;
	float myBackgroundSpeedFiveX;
	float myBackgroundSpeedSixX;
	float myBackgroundSpeedSevenX;

	float myBackgroundSpeedOneY;
	float myBackgroundSpeedTwoY;
	float myBackgroundSpeedThreeY;
	float myBackgroundSpeedFourY;
	float myBackgroundSpeedFiveY;
	float myBackgroundSpeedSixY;
	float myBackgroundSpeedSevenY;

	float myStartingPlayerPos;

	float myStartingCameraX;

	bool myGetReferences;
	bool myAddedCameraPosX;
	bool myAddedCameraPos;

	const void UpdateBackground(const float& aDeltaTime);
	const void MoveBackground(const float& aDeltaTime);
	const void LoadBackgrounds(Scene* aLevelScene, rapidjson::Document& someDocuments);
	const void CreateBackgrounds(Scene* aLevelScene, const std::string aPath, const int aIndex, const v2f anOffset);
	const void AddStartingCameraPos();
	const void CalculateCameraPositions(const float& aDeltaTime);
	const void ResetDistanceValues();
	const void CheckResetLoop(GameObject* aGameObject);

	const v2f GetHalfImageSize(GameObject* aSprite);
	const void CheckReferences();
};

