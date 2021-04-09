#include "stdafx.h"
#include "AudioObject.h"
#include "AudioLibrary.h"
#include "AudioComponent.h"
#include "AudioManager.h"

AudioObject::AudioObject(Scene* aScene, const int aType) : GameObject(aScene), myType(static_cast<AudioList>(aType))
{
	AudioManager::GetInstance()->FadeOut(AudioList::Forest_Theme);
	AudioManager::GetInstance()->FadeOut(AudioList::Village_Theme);
	AudioManager::GetInstance()->FadeOut(AudioList::Castle_Theme);
	AudioManager::GetInstance()->FadeIn(myType);
}