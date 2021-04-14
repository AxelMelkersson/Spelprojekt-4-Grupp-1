//Author: Axel Melkersson

#include "stdafx.h"
#include "Subscriber.hpp"
#include "PostMaster.hpp"

Subscriber::~Subscriber()
{
	for (size_t messageTypeIndex = 0; messageTypeIndex < myMessagesSubscribedTo.size(); ++messageTypeIndex)
	{
		PostMaster::GetInstance().RemoveSubcriber(this, myMessagesSubscribedTo[messageTypeIndex]);
	}

	myMessagesSubscribedTo.clear();
}

void Subscriber::Subscribe(const eMessageType& aMessageType)
{
	for (eMessageType& messageType : myMessagesSubscribedTo)
	{
		if (messageType == aMessageType) return;
	}

	myMessagesSubscribedTo.push_back(aMessageType);
	PostMaster::GetInstance().AddSubcriber(this, aMessageType);
}

void Subscriber::Unsubscribe(const eMessageType& aMessageType)
{
	for (size_t messageTypeIndex = 0; messageTypeIndex < myMessagesSubscribedTo.size(); ++messageTypeIndex)
	{
		if (myMessagesSubscribedTo[messageTypeIndex] == aMessageType)
		{
			PostMaster::GetInstance().RemoveSubcriber(this, aMessageType);
			myMessagesSubscribedTo.erase(myMessagesSubscribedTo.begin() + messageTypeIndex);
			return;
		}
	}
}