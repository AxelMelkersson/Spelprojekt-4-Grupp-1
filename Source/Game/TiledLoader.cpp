#include "stdafx.h"
#include "TiledLoader.h"
#include <sstream>
#include <istream>
#include <vector>

#include "DataManager.h"
#include "TileSetLayer.hpp"

#include "SpringObject.h"
#include "Bonfire.hpp"
#include "Collectible.hpp"
#include "EnemyFactory.h"
#include "PlatformFactory.hpp"
#include "Ledge.h"
#include "HiddenArea.hpp"
#include "BashableObject.hpp"
#include "MovingPlatform.hpp"
#include "Jesus.hpp"
#include "LevelDoor.hpp"
#include "Player.hpp"
#include "Glide.hpp"
#include "Door.h"
#include "AudioObject.h"
#include "SpeedrunManager.h"

#include "GameWorld.h"

typedef rapidjson::Value::ConstValueIterator iterator;

void TiledLoader::Load(Scene* aScene, int aLevelIndex, GameObject* aPlayer, const bool aIsHiddenRoom)
{
	const rapidjson::Document& levelDoc = DataManager::GetInstance().GetLevel(aLevelIndex, aIsHiddenRoom);
	std::vector<LoadData> loadData;
	std::vector<HiddenArea*> hiddenRoomsData;

	int area = 0;

	if (levelDoc.HasMember("properties"))
	{
		area = levelDoc["properties"][0]["value"].GetInt();
	}

	aScene->GetCamera().SetBounds(v2f(), v2f(levelDoc["width"].GetInt() * 8, levelDoc["height"].GetInt() * 8));

	if (levelDoc.IsObject())
	{
		for (iterator layer = levelDoc["layers"].Begin(); layer != levelDoc["layers"].End(); ++layer)
		{
			if ((*layer).HasMember("objects"))
			{
				//Gather Info
				for (iterator object = (*layer)["objects"].Begin(); object != (*layer)["objects"].End(); ++object)
				{
					LoadData data;
					data.myPosition.x = (*object)["x"].GetFloat();
					data.myPosition.y = (*object)["y"].GetFloat();

					data.mySize.x = (*object)["width"].GetFloat();
					data.mySize.y = (*object)["height"].GetFloat();

					std::string type = (*object)["type"].GetString();
					std::stringstream degree(type);
					degree >> data.myType;


					if ((*object).HasMember("properties"))
					{
						for (iterator property = (*object)["properties"].Begin(); property != (*object)["properties"].End(); ++property)
						{
							if (std::string((*property)["name"].GetString()).compare("Waypoints") == 0)
							{
								data.myWaypoints = (*property)["value"].GetString();
							}
							
							if (std::string((*property)["name"].GetString()).compare("Speed") == 0)
							{
								data.mySpeed = (*property)["value"].GetFloat();
							}

							if (std::string((*property)["name"].GetString()).compare("ButtonX") == 0)
							{
								data.myButtonPosition.x = (*property)["value"].GetFloat();
							}

							if (std::string((*property)["name"].GetString()).compare("ButtonY") == 0)
							{
								data.myButtonPosition.y = (*property)["value"].GetFloat();
							}

							if (std::string((*property)["name"].GetString()).compare("Material") == 0)
							{
								data.myPlatformMaterial = (*property)["value"].GetInt();
							}
							if (std::string((*property)["name"].GetString()).compare("ID") == 0)
							{
								data.myID = (*property)["value"].GetInt();
							}
							if (std::string((*property)["name"].GetString()).compare("BonfireID") == 0)
							{
								data.myBonfireID = (*property)["value"].GetInt();
							}
						}
					}

					loadData.push_back(data);
				}

				std::string name = (*layer)["name"].GetString();

				//Call functions
				if (name == "Bonfire" && CGameWorld::GetInstance()->GetLevelManager().GetSpeedrunManager()->GetIsSpeedrun() == false)
				{
					ParseBonfires(loadData, aScene, dynamic_cast<Player*>(aPlayer));
				}
				else if (name == "Doors")
				{
					ParseDoors(loadData, aScene, dynamic_cast<Player*>(aPlayer));
				}
				else if (name == "Enemies")
				{
					ParseEnemies(loadData, aScene);
				}
				else if (name == "Ledges")
				{
					ParseLedges(loadData, aScene);
				}
				else if (name == "Collectables" && CGameWorld::GetInstance()->GetLevelManager().GetSpeedrunManager()->GetIsSpeedrun() == false)
				{
					ParseCollectables(loadData, aScene);
				}
				else if (name == "AudioObjects")
				{
					ParseAudioObjects(loadData, aScene);
				}
				else if (name == "Glide")
				{
					ParseGlide(loadData, aScene);
				}
				else if (name == "Platforms")
				{
					ParsePlatforms(loadData, aScene);
				}
				else if (name == "HiddenRooms")
				{
					ParseHiddenRooms(loadData, aScene, hiddenRoomsData);
				}
				else if (name == "Springs")
				{
					ParseSprings(loadData, aScene);
				}
				else if (name == "Bashable")
				{
					ParseBashableObjects(loadData, aScene);
				}
				else if (name == "Buttons")
				{
					ParseButtons(loadData, aScene);
				}
				else if (name == "Jesus")
				{
					ParseJesus(loadData, aScene, aPlayer);
				}

				loadData.clear();
			}
			else
			{					 
				int z;
				std::string layerName = (*layer)["name"].GetString();

				if (layerName == "BG1")
				{
					z = myBG1z;
				}
				else if (layerName == "BG2")
				{
				 z = myBG2z;
				}
				else if (layerName == "FG1")
				{
					z = myFG1z;
				}
				else if (layerName == "FG2")
				{
					z = myFG2z;
				}
				else if (layerName == "HR")
				{
					z = myHRz;
				}

				TileSetLayer* tileSet = new TileSetLayer(aScene);
				SpritebatchComponent* batch = tileSet->LoadTileSetLayer(myTileSetLayerProperties, (*layer)["data"].GetArray(), (*layer)["width"].GetInt(), (*layer)["height"].GetInt(), z, area);

				if (layerName == "HR")
				{
					SetBatchForHiddenRooms(batch, hiddenRoomsData);
				}
			}
		}
	}

	myLoadsFromLevelSelect = false;
}

void TiledLoader::UsedLevelSelect()
{
	myLoadsFromLevelSelect = true;
}

void TiledLoader::ParseBonfires(const std::vector<LoadData> someData, Scene* aScene, Player* aPlayer)
{
	for (int i = 0; i < someData.size(); ++i)
	{
		Bonfire* bonfire = new Bonfire(aScene, someData[i].myID);
		bonfire->SetPosition(someData[i].myPosition);

		if (myLoadsFromLevelSelect)
		{
			aPlayer->SetSpawnPosition(someData[i].myPosition + v2f(0.0f, someData[i].mySize.y - 8.0f));
			aPlayer->SetPosition(someData[i].myPosition + v2f(0.0f, someData[i].mySize.y - 8.0f));
		}
	}
}
void TiledLoader::ParseDoors(const std::vector<LoadData> someData, Scene* aScene, Player* aPlayer)
{
	const v2f roomSize = aScene->GetCamera().GetBoundSize();

	const int lastDoorType = CGameWorld::GetInstance()->GetLevelManager().GetDoorType();

	for (int i = 0; i < someData.size(); ++i)
	{
		LevelDoor* door = new LevelDoor(aScene);
		
		bool doorFound = false;

		v2f doorOffset = v2f(0.0f, 0.0f);

		if (!myLoadsFromLevelSelect)
		{
			if (someData[i].myType == 0 && lastDoorType == 1)
			{
				doorOffset = GetDoorOffset(someData[i].myPosition, roomSize, someData[i].mySize);
				doorFound = true;
			}
			else if (someData[i].myType == 1 && lastDoorType == 0)
			{
				doorOffset = GetDoorOffset(someData[i].myPosition, roomSize, someData[i].mySize);
				doorFound = true;
			}
			else if (someData[i].myType == 2 && lastDoorType == 3)
			{
				doorOffset = GetDoorOffset(someData[i].myPosition, roomSize, someData[i].mySize);
				doorFound = true;
			}
			else if (someData[i].myType == 3 && lastDoorType == 2)
			{
				doorOffset = GetDoorOffset(someData[i].myPosition, roomSize, someData[i].mySize);
				doorFound = true;
			}
		}

		if (doorFound)
		{
			aPlayer->SetSpawnPosition(someData[i].myPosition + doorOffset);
			aPlayer->SetPosition(someData[i].myPosition + doorOffset);
		}

		door->Init(static_cast<LevelDoor::eDoorType>(someData[i].myType), someData[i].mySize);
		door->SetPosition(someData[i].myPosition);
	}
}

const v2f TiledLoader::GetDoorOffset(const v2f& aDoorPosition, const v2f& aRoomSize, const v2f& aDoorSize)
{
	v2f doorOffset;

	if (aDoorPosition.x < 0.0f)
	{
		doorOffset.x = 24.0f + aDoorSize.x;
		doorOffset.y = aDoorSize.y - 8.0f;
	}
	else if (aDoorPosition.x > aRoomSize.x)
	{
		doorOffset.x = -24.0f;
		doorOffset.y = aDoorSize.y - 8.0f;
	}
	else if (aDoorPosition.y < 0.0f)
	{
		doorOffset.x = 24.0f;
		doorOffset.y = 40.0f;
	}
	else if (aDoorPosition.y > aRoomSize.y)
	{
		doorOffset.x = 24.0f;
		doorOffset.y = -48.0f;
	}

	return doorOffset;
}


void TiledLoader::ParseEnemies(const std::vector<LoadData> someData, Scene* aScene)
{
	EnemyFactory enemyFactory;

	for (int i = 0; i < someData.size(); ++i)
	{
		switch (someData[i].myType)
		{
		case 0:
			enemyFactory.CreateNormalEnemy(aScene, GetWaypointPositions(someData[i].myWaypoints, someData[i].myPosition), someData[i].mySpeed);
			break;
		case 1:
			enemyFactory.CreateShootingEnemy(aScene, GetWaypointPositions(someData[i].myWaypoints, someData[i].myPosition), someData[i].mySpeed);
			break;
		}
	}
}
void TiledLoader::ParseLedges(const std::vector<LoadData> someData, Scene* aScene)
{
	const v2f ledgeSize = v2f(2.0f, 1.0f);
	for (int i = 0; i < someData.size(); ++i)
	{
		Ledge* ledge = new Ledge(aScene);
		ledge->Init(someData[i].myPosition, ledgeSize);
	}
}
void TiledLoader::ParseCollectables(const std::vector<LoadData> someData, Scene* aScene)
{
	for (int i = 0; i < someData.size(); ++i)
	{
		Collectible::eCollectibleType aType = Collectible::eCollectibleType::Easy;

		switch (someData[i].myType)
		{
		case 0:
			aType = Collectible::eCollectibleType::Easy;
			break;
		case 1:
			aType = Collectible::eCollectibleType::Medium;
			break;
		case 2:
			aType = Collectible::eCollectibleType::Hard;
			break;
		}

		Collectible* collectible = new Collectible(aScene, someData[i].myID, someData[i].myBonfireID);
		collectible->Init(someData[i].myPosition, aType);
	}
}
void TiledLoader::ParseGlide(const std::vector<LoadData> someData, Scene* aScene)
{
	for (int i = 0; i < someData.size(); ++i)
	{
		Glide* glide = new Glide(aScene);
		glide->Init(someData[i].myPosition);
	}
}
void TiledLoader::ParseAudioObjects(const std::vector<LoadData> someData, Scene* aScene)
{
	for (int i = 0; i < someData.size(); ++i)
	{
		AudioObject* audioObj = new AudioObject(aScene, someData[i].myType);
	}
}
void TiledLoader::ParsePlatforms(const std::vector<LoadData> someData, Scene* aScene)
{
	PlatformFactory platformFactory;

	for (int i = 0; i < someData.size(); ++i)
	{
		switch (someData[i].myType)
		{
		case 0:
			platformFactory.CreateStaticPlatform(aScene, someData[i].myPosition, someData[i].mySize, someData[i].mySize, false, someData[i].myPlatformMaterial);
			break;
		case 1:
			platformFactory.CreateMovingPlatform(aScene, someData[i].myPosition, someData[i].mySize, someData[i].mySize, GetWaypointPositions(someData[i].myWaypoints, someData[i].myPosition), someData[i].mySpeed);
			break;
		case 2:
			platformFactory.CreateUnstablePlatform(aScene, someData[i].myPosition, someData[i].mySize, someData[i].mySize, 0.5f, 2.0f);
			break;
		case 3:
			platformFactory.CreateDestructiblePlatform(aScene, someData[i].myPosition, someData[i].mySize, someData[i].mySize);
			break;
		case 4:
			platformFactory.CreateDeadlyPlatform(aScene, someData[i].myPosition, someData[i].mySize, someData[i].mySize);
			break;
		case 5:
			platformFactory.CreateStaticPlatform(aScene, someData[i].myPosition, someData[i].mySize, someData[i].mySize, true, 3);
		}
	}
}
void TiledLoader::ParseHiddenRooms(const std::vector<LoadData> someData, Scene* aScene, std::vector<HiddenArea*>& aHiddenRoomsData)
{
	for (int i = 0; i < someData.size(); ++i)
	{
		HiddenArea* hiddenArea = new HiddenArea(aScene, someData[i].myPosition, someData[i].mySize);
		aHiddenRoomsData.push_back(hiddenArea);
	}
}
void TiledLoader::ParseSprings(const std::vector<LoadData> someData, Scene* aScene)
{
	for (int i = 0; i < someData.size(); ++i)
	{
		SpringObject* aSpring = new SpringObject(aScene);
		aSpring->Init(someData[i].myPosition);
	}
}
void TiledLoader::ParseBashableObjects(const std::vector<LoadData> someData, Scene* aScene)
{
	constexpr float radius = 20.0f;
	for (int i = 0; i < someData.size(); ++i)
	{
		BashableObject* bashObj = new BashableObject(aScene);
		bashObj->Init(someData[i].myPosition, radius);
	}
}
void TiledLoader::ParseButtons(const std::vector<LoadData> someData, Scene* aScene)
{
	PlatformFactory platformFactory;

	for (int i = 0; i < someData.size(); ++i)
	{
		if (someData[i].myType == 3)
		{
			Door* myDoor = new Door(aScene);
			myDoor->Init(someData[i].myPosition);
			myDoor->AddButton(someData[i].myButtonPosition);
		}
		else
		{
			MovingPlatform* platform = platformFactory.CreateMovingPlatform(aScene, someData[i].myPosition, someData[i].mySize, someData[i].mySize, GetWaypointPositions(someData[i].myWaypoints, someData[i].myPosition), someData[i].mySpeed);
			MovingPlatform::eMovingPlatformType aType = MovingPlatform::eMovingPlatformType::MovingPlatform;

			switch (someData[i].myType)
			{
			case 0:
				aType = MovingPlatform::eMovingPlatformType::MovingPlatform;
				break;
			case 1:
				aType = MovingPlatform::eMovingPlatformType::ReversePlatform;
				break;
			case 2:
				aType = MovingPlatform::eMovingPlatformType::PointAtoBPlatform;
				break;
			}
			platform->AddButton(someData[i].myButtonPosition, aType);
		}
	}
}
void TiledLoader::ParseJesus(const std::vector<LoadData> someData, Scene* aScene, GameObject* aPlayer)
{
	for (size_t jesusIndex = 0; jesusIndex < someData.size(); ++jesusIndex)
	{
		Jesus* jesus = new Jesus(aScene);
		jesus->Init(someData[jesusIndex].myPosition);
		jesus->SetTarget(aPlayer);
	}
}

void TiledLoader::SetBatchForHiddenRooms(SpritebatchComponent* aBatch, std::vector<HiddenArea*>& aHiddenRoomsData)
{
	for (HiddenArea* hiddenArea : aHiddenRoomsData)
	{
		hiddenArea->SetBatch(aBatch);
	}
}
std::vector<v2f> TiledLoader::GetWaypointPositions(const std::string somePositions, v2f aSpawnPos)
{
	std::vector<v2f> waypoints;
	waypoints.push_back(aSpawnPos);

	std::stringstream sstream;

	sstream << somePositions;
	std::string tempWord = {};
	int tempNum = {};
	int tempX = {};
	bool hasX = false;

	while (!sstream.eof())
	{
		sstream >> tempWord;

		if (std::stringstream(tempWord) >> tempNum)
		{
			if (hasX == false)
			{
				tempX = tempNum;
				hasX = true;
			}
			else
			{
				waypoints.push_back({ static_cast<float>(tempX) * myTileSize.x + aSpawnPos.x, static_cast<float>(tempNum) * myTileSize.y + aSpawnPos.y });
				hasX = false;
			}
		}
	}

	return waypoints;
}