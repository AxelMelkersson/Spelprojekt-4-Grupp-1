#include "stdafx.h"
#include "AudioObject.h"
#include "AudioLibrary.h"
#include "AudioComponent.h"
#include "AudioManager.h"

AudioObject::AudioObject(Scene* aScene, const int aType) : GameObject(aScene), myType(static_cast<AudioList>(aType))
{
	AudioManager::GetInstance()->FadeIn(myType);
}