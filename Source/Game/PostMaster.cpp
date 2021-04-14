//Author: Axel Melkersson

#include "stdafx.h"
#include "PostMaster.hpp"
#include "Subscriber.hpp"

void PostMaster::AddSubcriber(Subscriber* aSubscriber, const eMessageType& aMessageType)
{
	if (mySubscribers.find(aMessageType) == mySubscribers.end())
	{
		mySubscribers.insert({aMessageType, std::vector<Subscriber*>()});
	}

	mySubscribers[aMessageType].push_back(aSubscriber);
}

void PostMaster::RemoveSubcriber(Subscriber* aSubscriber, const eMessageType& aMessageType)
{
	if (mySubscribers.find(aMessageType) != mySubscribers.end())
	{
		for (size_t subscriberIndex = 0; subscriberIndex < mySubscribers[aMessageType].size(); ++subscriberIndex)
		{
			if (mySubscribers[aMessageType][subscriberIndex] == aSubscriber)
			{
				mySubscribers[aMessageType].erase(mySubscribers[aMessageType].begin() + subscriberIndex);
				return;
			}
		}
	}
}

void PostMaster::ReceiveMessage(const Message& aMessage)
{
	if (mySubscribers.find(aMessage.myMessageType) == mySubscribers.end())
	{
		return;
	}

	for (Subscriber* subscriber : mySubscribers[aMessage.myMessageType])
	{
		subscriber->Notify(aMessage);
	}
}

void PostMaster::ReceiveMessage(const Message& aMessage, const bool aWillWait)
{
	myWaitingMessages.push_back(aMessage);
}

void PostMaster::SendWaitingMessages()
{
	for (const Message& message : myWaitingMessages)
	{
		ReceiveMessage(message);
	}

	myWaitingMessages.clear();
}