#pragma once
#include "rapidjson/document.h"
#include "Scene.h"

#include "TileSetLayerProperties.hpp"


class GameObject;
class SpritebatchComponent;
class HiddenArea;
class Player;

struct LoadData
{
	v2f myPosition;
	v2f myButtonPosition;
	v2f mySize;
	int myType;
	std::string myWaypoints;
	float mySpeed;
	int myPlatformMaterial;

	int myID;
	int myBonfireID;
};

struct TileLayerData
{
	std::vector<int> myTiles;
	v2f mySize;
	int myZ;
};

class TiledLoader
{
public:
	void Load(Scene* aScene, int aLevelIndex, GameObject* aPlayer, bool aIsHiddenRoom);

	void UsedLevelSelect();
	void Continued();

private:
	void ParseBonfires(const std::vector<LoadData> someData, Scene*, Player* aPlayer);
	void ParseDoors(const std::vector<LoadData> someData, Scene* aScene, Player* aPlayer);
	void ParseEnemies(const std::vector<LoadData> someData, Scene*);
	void ParseLedges(const std::vector<LoadData> someData, Scene*);
	void ParseCollectables(const std::vector<LoadData> someData, Scene*);
	void ParseGlide(const std::vector<LoadData> someData, Scene*);
	void ParseAudioObjects(const std::vector<LoadData> someData, Scene*);
	void ParsePlatforms(const std::vector<LoadData> someData, Scene*);
	void ParseHiddenRooms(const std::vector<LoadData> someData, Scene* aScene, std::vector<HiddenArea*>& aHiddenRoomsData);
	void ParseSprings(const std::vector<LoadData> someData, Scene*);
	void ParseBashableObjects(const std::vector<LoadData> someData, Scene*);
	void ParseButtons(const std::vector<LoadData> someData, Scene*);
	void ParseJesus(const std::vector<LoadData> someData, Scene* aScene, GameObject* aPlayer);

	const v2f GetDoorOffset(const v2f& aDoorPosition, const v2f& aRoomSize, const v2f& aDoorSize);

	void SetBatchForHiddenRooms(SpritebatchComponent* aBatch, std::vector<HiddenArea*>& aHiddenRoomsData);

	std::vector<v2f> GetWaypointPositions(const std::string somePositions, v2f aSpawnPos);

	v2f myTileSize = { 8, 8 };

	TileSetLayerProperties myTileSetLayerProperties;
	
	int myBG1z = 80;
	int myBG2z = 90;
	int myFG1z = 110;
	int myFG2z = 120;
	int myHRz = 130;

	bool myLoadsFromLevelSelect = false;
	bool myContinued = false;
};