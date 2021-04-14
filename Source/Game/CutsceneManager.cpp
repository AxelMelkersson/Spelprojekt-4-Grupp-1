#include "stdafx.h"
#include "CutsceneManager.h"
#include <tga2d/videoplayer/video.h>
#include <tga2d/sprite/sprite.h>
#include "InputWrapper.h"
#include "Game.h"
#include "SpeedrunManager.h"
#include "AudioManager.h"

CutsceneManager::CutsceneManager()
{
	myVideo = nullptr;
	myInput = CGameWorld::GetInstance()->Input();
}

void CutsceneManager::PlayVideo(CutsceneType aCutsceneType)
{
	myVideo = new Tga2D::CVideo();
	myType = aCutsceneType;
	if (aCutsceneType == CutsceneType::Intro)
	{
		myVideo->Init("Video/cutscene_opening.mp4", false);
		AudioManager::GetInstance()->PlayAudio(AudioList::IntroMusic);
		AudioManager::GetInstance()->LockAudio(AudioList::BonfireActivated);
	}
	else
	{
		myVideo->Init("Video/cutscene_ending.mp4", false);
		//AudioManager::GetInstance()->PlayAudio(AudioList::IntroMusic);
	}
	myVideo->Play(false);
	myVideo->GetSprite()->SetSizeRelativeToScreen({ 16.f / 9.f, 1.f });
	myIsPlaying = true;
}

void CutsceneManager::Update(const float& aDeltaTime)
{
	if (myVideo == nullptr) return;
	if (myInput->GetInput()->GetKeyJustDown(Keys::SPACEBAR) || myInput->GetController()->IsButtonPressed(Controller::Button::Cross) || myInput->GetController()->IsButtonPressed(Controller::Button::A))
	{
		StopVideo();
		return;
	}
	myVideo->Update(aDeltaTime);
	if (myVideo->GetStatus() == Tga2D::VideoStatus::VideoStatus_ReachedEnd)
	{
		StopVideo();
	}
}

void CutsceneManager::Render()
{
	if (myVideo == nullptr) return;
	myVideo->Render();
}

void CutsceneManager::StopVideo()
{
	myVideo->Stop();
	myVideo = nullptr;
	myIsPlaying = false;
	if (myType == CutsceneType::Intro)
	{
		AudioManager::GetInstance()->UnLockAudio(AudioList::BonfireActivated);
		AudioManager::GetInstance()->PlayAudio(AudioList::BonfireActivated);
		AudioManager::GetInstance()->Stop(AudioList::IntroMusic);
	}
	else
	{

	}
}

bool CutsceneManager::IsPlaying()
{
	return myIsPlaying;
}