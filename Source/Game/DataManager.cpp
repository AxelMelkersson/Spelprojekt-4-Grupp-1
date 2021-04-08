#include "stdafx.h"
#include "DataManager.h"
#include <array>
#include <fstream>
#include <sstream>
#include <cassert>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

void DataManager::SetDataStruct(const DataEnum aDataEnum)
{
	rapidjson::Document tempDoc;
	std::string tempDataPath;

	switch (aDataEnum)
	{
	case DataEnum::player:
	{
		tempDataPath = myMasterDoc["PlayerData"].GetString();
		ReadFileIntoDocument(tempDataPath, tempDoc);

		for (size_t i = 0; i < static_cast<size_t>(PlayerData::PlayerFloatEnum::Player_FloatEnum_Size); i++)
		{
			PlayerData::PlayerFloatEnum enumValue = static_cast<PlayerData::PlayerFloatEnum>(i);
			tempDoc[myPlayerData.myFloatNameMap[enumValue].data()].SetFloat(myPlayerData.myFloatValueMap[enumValue]);
		}
	}
	break;
	case DataEnum::enemy:
	{
		tempDataPath = myMasterDoc["EnemyData"].GetString();
		ReadFileIntoDocument(tempDataPath, tempDoc);

		for (size_t i = 0; i < static_cast<size_t>(EnemyData::EnemyFloatEnum::Enemy_FloatEnum_Size); i++)
		{
			EnemyData::EnemyFloatEnum enumValue = static_cast<EnemyData::EnemyFloatEnum>(i);
			tempDoc[myEnemyData.myFloatNameMap[enumValue].data()].SetFloat(myEnemyData.myFloatValueMap[enumValue]);
		}
	}
	break;
	default:
		assert((false) && "Invalid Enum given to DataManager::SetDataStruct().");
		break;
	}

	AcceptJsonWriter(tempDataPath);
}
Data& DataManager::GetDataStruct(const DataEnum aDataEnum)
{
	switch (aDataEnum)
	{
	case DataEnum::player:
	{
		return myPlayerData;
	}
	break;
	case DataEnum::enemy:
	{
		return myEnemyData;
	}
	break;
	default:
	{
		assert((false) && "Invalid Enum given to DataManager::GetDataStruct().");
		return Data();
	}
	break;
	}
}

const rapidjson::Document& DataManager::GetLevel(const unsigned int aLevelIndex, const bool aIsHiddenRoom) const
{
	if (aIsHiddenRoom)
	{
		if (myHiddenRooms.find(static_cast<int>(aLevelIndex)) != myHiddenRooms.end())
		{
			return myHiddenRooms.at(static_cast<int>(aLevelIndex));
		}
	}

	if (aLevelIndex < 0 || aLevelIndex >= myLevelVector.size())
	{
		return myLevelVector[0];
	}

	return myLevelVector[aLevelIndex];
}
const int DataManager::GetLevelCount() const
{
	return static_cast<int>(myLevelVector.size());
}

PlayerData::PlayerData()
{
	for (size_t i = 0; i < static_cast<size_t>(PlayerData::PlayerFloatEnum::Player_FloatEnum_Size); i++)
	{
		myFloatValueMap[static_cast<PlayerData::PlayerFloatEnum>(i)] = 0.0f;
	}
}
EnemyData::EnemyData()
{
	for (size_t i = 0; i < static_cast<size_t>(EnemyData::EnemyFloatEnum::Enemy_FloatEnum_Size); i++)
	{
		myFloatValueMap[static_cast<EnemyData::EnemyFloatEnum>(i)] = 0.0f;
	}
}

void DataManager::SaveHighScores(const std::array<float, 10> &someHighscores)
{
	for (size_t i = 0; i < someHighscores.size(); i++)
	{
		mySaveFile["HighScore"].GetArray()[i]["Score"]["Value"].SetFloat(someHighscores[i]);
	}
	AcceptJsonWriter("JSON/SaveFile.json");
}
void DataManager::SaveBonfireState(const unsigned int anIndex, const bool aState)
{
	assert((mySaveFile["Bonfires"].IsArray()) && "Bonfires is not Array.");
	assert((mySaveFile["Bonfires"][anIndex]["Bonfire"].IsObject()) && "Bonfire is not Object.");
	assert((mySaveFile["Bonfires"][anIndex]["Bonfire"]["IsActive"].IsBool()) && "IsActive is not Boolean.");

	mySaveFile["Bonfires"].GetArray()[anIndex]["Bonfire"]["IsActive"].SetBool(aState);

	AcceptJsonWriter("JSON/SaveFile.json");
}
void DataManager::SaveCollectedCollectible(const unsigned int anID)
{
	for (size_t i = 0; i < myCollectableInfo.size(); i++)
	{
		if (mySaveFile["Collectibles"].GetArray()[i]["Collectible"]["ID"].GetInt() == anID)
		{
			mySaveFile["Collectibles"].GetArray()[i]["Collectible"]["BeenCollected"].SetBool(true);
		}
	}
	AcceptJsonWriter("JSON/SaveFile.json");
}

void DataManager::ResetSaveFile()
{
	ResetCollectibles();
	ResetBonfires();
}
void DataManager::ResetBonfires()
{
	for (size_t i = 0; i < mySaveFile["Bonfires"].GetArray().Size(); i++)
	{
		mySaveFile["Bonfires"].GetArray()[i]["Bonfire"]["IsActive"].SetBool(false);
	}
}
void DataManager::ResetCollectibles()
{
	// Clears Array
	mySaveFile["Collectibles"].GetArray().Clear();

	// Assigns Value and Pushes Objects.
	for (size_t i = 0; i < myCollectableInfo.size(); i++)
	{
		rapidjson::Document::AllocatorType& allocator = mySaveFile.GetAllocator();
		rapidjson::Value jsonObject(rapidjson::Type::kObjectType);

		rapidjson::Value collectible(rapidjson::Type::kObjectType);
		jsonObject.AddMember("Collectible", collectible, allocator);

		rapidjson::Value ID(rapidjson::Type::kNumberType);
		ID.SetInt(myCollectableInfo[i].myID);
		jsonObject["Collectible"].AddMember("ID", ID, allocator);

		rapidjson::Value bonfireID(rapidjson::Type::kNumberType);
		bonfireID.SetInt(myCollectableInfo[i].myBonfireID);
		jsonObject["Collectible"].AddMember("BonfireID", bonfireID, allocator);

		rapidjson::Value state(rapidjson::Type::kFalseType);
		jsonObject["Collectible"].AddMember("BeenCollected", state, allocator);

		rapidjson::Value difficulty(rapidjson::Type::kNumberType);
		difficulty.SetInt(myCollectableInfo[i].myDifficulty);
		jsonObject["Collectible"].AddMember("Difficulty", difficulty, allocator);

		mySaveFile["Collectibles"].PushBack(jsonObject, allocator);
	}

	AcceptJsonWriter("JSON/SaveFile.json");
}
void DataManager::ResetHighScores()
{
	for (size_t i = 0; i < mySaveFile["HighScore"].GetArray().Size(); i++)
	{
		mySaveFile["HighScore"].GetArray()[i]["Score"]["Value"].SetFloat(0);
	}
	AcceptJsonWriter("JSON/SaveFile.json");
}

const CollectableInfo &DataManager::GetCollectableInfo(const int anID) const
{
	for (size_t i = 0; i < myCollectableInfo.size(); i++)
	{
		if (mySaveFile["Collectibles"].GetArray()[i]["Collectible"]["ID"].GetInt() == anID)
		{
			return myCollectableInfo[i];
		}
	}
	assert((false) && "A Collectible ID not found in DataManager::GetCollectableInfo().");
}

const CollectableInfo& DataManager::GetCollectableInfoIndex(const int anID) const
{
	return myCollectableInfo[anID];
}

const unsigned int DataManager::GetCollectableCount() const
{
	return static_cast<int>(myCollectableInfo.size());
}
const bool DataManager::GetBonfireState(const unsigned int anIndex) const
{
	return mySaveFile["Bonfires"].GetArray()[anIndex]["Bonfire"]["IsActive"].GetBool();
}
const std::array<float, 10> &DataManager::GetHighScores() const
{
	std::array<float, 10> tempArray;
	for (size_t i = 0; i < mySaveFile["HighScore"].GetArray().Size(); i++)
	{
		tempArray[i] = mySaveFile["HighScore"].GetArray()[i]["Score"]["Value"].GetFloat();
	}
	return tempArray;
}
void DataManager::ParseCollectableInfo(){
	for (const auto& levelDoc : myLevelVector)
	{
		if (levelDoc.IsObject())
		{
			for (auto layer = levelDoc["layers"].Begin(); layer != levelDoc["layers"].End(); ++layer)
			{
				std::string name = (*layer)["name"].GetString();

				if (name == "Collectables" && (*layer).HasMember("objects"))
				{
					for (auto object = (*layer)["objects"].Begin(); object != (*layer)["objects"].End(); ++object)
					{
						CollectableInfo info;
						std::string type = (*object)["type"].GetString();
						std::stringstream degree(type);
						degree >> info.myDifficulty;
						
						if ((*object).HasMember("properties"))
						{
							for (auto property = (*object)["properties"].Begin(); property != (*object)["properties"].End(); ++property)
							{
								if (std::string((*property)["name"].GetString()).compare("BonfireID") == 0)
								{
									info.myBonfireID = (*property)["value"].GetInt();
								}
								if (std::string((*property)["name"].GetString()).compare("ID") == 0)
								{
									info.myID = (*property)["value"].GetInt();
								}
							}
							myCollectableInfo.push_back(info);
						}
					}
				}
			}
		}
	}

#ifndef _RETAIL
	ResetSaveFile();
#endif // !_RETAIL
	AssignCollectedState();
}

// Private Methos
DataManager::DataManager()
{
	//Assign MasterDoc & SaveFile
	ReadFileIntoDocument("Master.json", myMasterDoc);
	ReadFileIntoDocument("JSON/SaveFile.json", mySaveFile);

	//Assign LevelDocs
	ReadFileIntoDocument("Levels/LevelMaster.json", myLevelMasterDoc);
	assert(myLevelMasterDoc["FilePathArray"].IsArray());
	for (size_t i = 0; i < myLevelMasterDoc["FilePathArray"].GetArray().Size(); i++)
	{
		myLevelVector.push_back(rapidjson::Document());
		ReadFileIntoDocument(myLevelMasterDoc["FilePathArray"].GetArray()[i]["FilePath"].GetString(), myLevelVector.back());
	}

	ReadFileIntoDocument("HiddenRooms/HiddenRooms.json", myHiddenRoomMasterDoc);
	for (size_t i = 0; i < myHiddenRoomMasterDoc["HiddenRooms"].GetArray().Size(); i++)
	{
		const int hiddenRoomKey = myHiddenRoomMasterDoc["HiddenRooms"].GetArray()[i]["LevelIndex"].GetInt();

		myHiddenRooms.insert({ hiddenRoomKey, rapidjson::Document() });
		ReadFileIntoDocument(myHiddenRoomMasterDoc["HiddenRooms"].GetArray()[i]["FilePath"].GetString(), myHiddenRooms.at(hiddenRoomKey));
	}

	//Assign PlayerDoc
	std::string playerDataPath = myMasterDoc["PlayerData"].GetString();
	rapidjson::Document playerDoc;
	ReadFileIntoDocument(playerDataPath, playerDoc);
	//Assign Player Values
	AssignValues(DataEnum::player, playerDoc);

	//Assign EnemyDoc
	std::string enemyDataPath = myMasterDoc["EnemyData"].GetString();
	rapidjson::Document enemyDoc;
	ReadFileIntoDocument(enemyDataPath, enemyDoc);
	//Assign Enemy Values
	AssignValues(DataEnum::enemy, enemyDoc);

#ifndef _RETAIL
	ResetSaveFile();
#endif // !_RETAIL
}

void DataManager::ReadFileIntoDocument(const std::string aFilePath, rapidjson::Document& anOutDoc)
{
	std::ifstream dataFile(aFilePath);
	std::ostringstream outStringStream;
	outStringStream << dataFile.rdbuf();
	std::string outString = outStringStream.str();
	anOutDoc.Parse(outString.c_str());
	dataFile.close();
}
void DataManager::AcceptJsonWriter(const std::string aDataPath) const
{
	std::ofstream ofs{ aDataPath };
	rapidjson::OStreamWrapper osw{ ofs };
	rapidjson::Writer<rapidjson::OStreamWrapper> writer{ osw };
	mySaveFile.Accept(writer);
}
void DataManager::AssignValues(const DataEnum anEnum, const rapidjson::Document &aDoc)
{
	switch (anEnum)
	{
	case DataEnum::player:
	{
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Acceleration] = "Acceleration";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Max_Speed] = "MaxSpeed";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Retardation] = "Retardation";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Lerp_Acceleration] = "Lerp Acceleration";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Platform_Velocity_Retardation] = "Platform Velocity Retardation";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Coyote_Time] = "Coyote Time";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Jump_Velocity] = "Jump Velocity";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Double_Jump_Velocity] = "Double Jump Velocity";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Max_Fall_Speed] = "Max Fall Speed";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Ledge_Jump_Velocity] = "Ledge Jump Velocity";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Jump_When_Falling_Time] = "Jump When Falling Time";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Trigger_Falling_Speed] = "Trigger Falling Speed";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Die_Vibration_Strength] = "Die Vibration Strength";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Land_Vibration_Strength] = "Land Vibration Strength";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Springs_Vibration_Strength] = "Springs Vibration Strength";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Die_Vibration_Length] = "Die Vibration Length";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Land_Vibration_Length] = "Land Vibration Length";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Springs_Vibration_Length] = "Springs Vibration Length";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Die_Shake_Duration] = "Die Shake Duration";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Die_Shake_Intensity] = "Die Shake Intensity";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Die_Shake_DropOff] = "Die Shake DropOff";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Land_Shake_Duration] = "Land Shake Duration";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Land_Shake_Intensity] = "Land Shake Intensity";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Land_Shake_DropOff] = "Land Shake DropOff";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Spring_Shake_Duration] = "Spring Shake Duration";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Spring_Shake_Intensity] = "Spring Shake Intensity";
		myPlayerData.myFloatNameMap[PlayerData::PlayerFloatEnum::Spring_Shake_DropOff] = "Spring Shake DropOff";

		for (size_t i = 0; i < static_cast<size_t>(PlayerData::PlayerFloatEnum::Player_FloatEnum_Size); i++)
		{
			PlayerData::PlayerFloatEnum enumValue = static_cast<PlayerData::PlayerFloatEnum>(i);
			myPlayerData.myFloatValueMap[enumValue] = aDoc[myPlayerData.myFloatNameMap[enumValue].data()].GetFloat();
		}
	}
	break;
	case DataEnum::enemy:
	{
		myEnemyData.myFloatNameMap[EnemyData::EnemyFloatEnum::NE_SpriteSizeX] = "NE_SpriteSizeX";
		myEnemyData.myFloatNameMap[EnemyData::EnemyFloatEnum::NE_SpriteSizeY] = "NE_SpriteSizeY";
		myEnemyData.myFloatNameMap[EnemyData::EnemyFloatEnum::NE_CollisionSizeX] = "NE_CollisionSizeX";
		myEnemyData.myFloatNameMap[EnemyData::EnemyFloatEnum::NE_CollisionSizeY] = "NE_CollisionSizeY";
		myEnemyData.myFloatNameMap[EnemyData::EnemyFloatEnum::SE_SpriteSizeX] = "SE_SpriteSizeX";
		myEnemyData.myFloatNameMap[EnemyData::EnemyFloatEnum::SE_SpriteSizeY] = "SE_SpriteSizeY";
		myEnemyData.myFloatNameMap[EnemyData::EnemyFloatEnum::SE_CollisionSizeX] = "SE_CollisionSizeX";
		myEnemyData.myFloatNameMap[EnemyData::EnemyFloatEnum::SE_CollisionSizeY] = "SE_CollisionSizeY";
		myEnemyData.myFloatNameMap[EnemyData::EnemyFloatEnum::FireRate] = "FireRate";
		myEnemyData.myFloatNameMap[EnemyData::EnemyFloatEnum::FireRadius] = "FireRadius";
		myEnemyData.myFloatNameMap[EnemyData::EnemyFloatEnum::P_SpriteSizeX] = "P_SpriteSizeX";
		myEnemyData.myFloatNameMap[EnemyData::EnemyFloatEnum::P_SpriteSizeY] = "P_SpriteSizeY";
		myEnemyData.myFloatNameMap[EnemyData::EnemyFloatEnum::P_CollisionSizeX] = "P_CollisionSizeX";
		myEnemyData.myFloatNameMap[EnemyData::EnemyFloatEnum::P_CollisionSizeY] = "P_CollisionSizeY";
		myEnemyData.myFloatNameMap[EnemyData::EnemyFloatEnum::Speed] = "Speed";

		for (size_t i = 0; i < static_cast<size_t>(EnemyData::EnemyFloatEnum::Enemy_FloatEnum_Size); i++)
		{
			EnemyData::EnemyFloatEnum enumValue = static_cast<EnemyData::EnemyFloatEnum>(i);
			myEnemyData.myFloatValueMap[enumValue] = aDoc[myEnemyData.myFloatNameMap[enumValue].data()].GetFloat();
		}
	}
	break;
	default:
		assert((false) && "Invalid Enum given to DataManager::AssignValues().");
		break;
	}
}
void DataManager::AssignCollectedState()
{
	for (size_t i = 0; i < myCollectableInfo.size(); i++)
	{
		myCollectableInfo[i].myCollectedState = mySaveFile["Collectibles"].GetArray()[i]["Collectible"]["BeenCollected"].GetBool();
	}
}