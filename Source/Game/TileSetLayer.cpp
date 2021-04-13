#include "stdafx.h"
#include "TileSetLayer.hpp"

#include "Scene.h"
#include "SpritebatchComponent.h"
#include "SpriteComponent.h"

#include "../External/Headers/rapidjson/istreamwrapper.h"
#include <fstream>

#include "TiledLoader.h"

#include <iostream>

typedef rapidjson::Value::ConstValueIterator iterator;

TileSetLayer::TileSetLayer(Scene* aLevelScene)
	: GameObject(aLevelScene),
	myBatch(nullptr)
{
	
}

TileSetLayer::~TileSetLayer()
{
	
}

SpritebatchComponent* TileSetLayer::LoadTileSetLayer(const TileSetLayerProperties& aTileSetLayerProperties, const GenericArray& aLayerData, const int& aWidth, const int& aHeight, const int& aZIndex, const int& aTilesetIndex)
{
	SetZIndex(aZIndex);

	const int tilesetIndex = aTilesetIndex;

	myBatch = AddComponent<SpritebatchComponent>();
	myBatch->SetSpritePath(aTileSetLayerProperties.mySpritePath[tilesetIndex]);
	myBatch->SetSamplerFilter(ESamplerFilter_Point);

	const GenericArray& data = aLayerData;

	const int width = aWidth;
	const int height = aHeight;

	for (int dataIndex = 0; dataIndex < data.Size(); ++dataIndex)
	{
		if (data[dataIndex].GetInt() == 0)
		{
			continue;
		}

		float x = dataIndex % width + 0.5f;
		float y = dataIndex / width + 0.5f;

		SpriteComponent* sprite = AddComponent<SpriteComponent>();

		sprite->SetSpritePath(aTileSetLayerProperties.mySpritePath[tilesetIndex]);
		sprite->SetSize({ aTileSetLayerProperties.mySpriteSizeX, aTileSetLayerProperties.mySpriteSizeY });

		const int realQuad = data[dataIndex].GetInt() - 1;
		int xQ = static_cast<int>(realQuad % aTileSetLayerProperties.myQuadLengthX);
		int yQ = static_cast<int>(realQuad / aTileSetLayerProperties.myQuadLengthX);

		int texelX = static_cast<int>(1.0f / aTileSetLayerProperties.myImageSizeX);
		int texelY = static_cast<int>(1.0f / aTileSetLayerProperties.myImageSizeY);

		sprite->SetSpriteRect(texelX + xQ * aTileSetLayerProperties.myRectQuadX, texelY + yQ * aTileSetLayerProperties.myRectQuadY, (xQ + 1.0f) * aTileSetLayerProperties.myRectQuadX - texelX, (yQ + 1.0f) * aTileSetLayerProperties.myRectQuadY - texelY);
		sprite->SetRelativePosition({ x * (aTileSetLayerProperties.mySpriteSizeX), y * (aTileSetLayerProperties.mySpriteSizeY) });
		sprite->SetCeilPosition(true);

		myBatch->AddSprite(sprite);
	}

	myBatch->Init();
	return myBatch;
}