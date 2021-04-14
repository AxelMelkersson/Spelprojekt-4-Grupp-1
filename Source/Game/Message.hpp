//Author: Axel Melkersson
#pragma once
#include "MessageTypes.hpp"
#include <string>
#include <variant>
#include "../External/Headers/CU/Vector2.hpp"
#include "GameObject.h"

struct Message
{
	Message(const eMessageType& aMessageType, const int& aIntData)
	{
		myMessageType = aMessageType;
		myData = aIntData;
	}

	Message(const eMessageType& aMessageType, const float& aFloatData)
	{
		myMessageType = aMessageType;
		myData = aFloatData;
	}

	Message(const eMessageType& aMessageType, const std::string& aStringData)
	{
		myMessageType = aMessageType;
		myData = aStringData;
	}

	Message(const eMessageType& aMessageType, const v2f& aPosition)
	{
		myMessageType = aMessageType;
		myData = aPosition;
	}

	Message(const eMessageType& aMessageType, GameObject* aGameObject)
	{
		myMessageType = aMessageType;
		myEffectObject = aGameObject;
	}

	~Message() = default;

	eMessageType myMessageType;

	std::variant<int, float, std::string, v2f> myData;
	GameObject* myEffectObject = nullptr;
};

