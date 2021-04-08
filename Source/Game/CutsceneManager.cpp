#include "stdafx.h"
#include "CutsceneManager.h"
#include <tga2d/videoplayer/video.h>
#include <tga2d/sprite/sprite.h>
#include "InputWrapper.h"
#include "Game.h"
#include "SpeedrunManager.h"

CutsceneManager::CutsceneManager()
{
	myVideo = nullptr;
	myInput = CGameWorld::GetInstance()->Input();
}

void CutsceneManager::PlayVideo(CutsceneType aCutsceneType)
{
		myVideo = new Tga2D::CVideo();
		myVideo->Init("Video/TestVideo.mp4", true);
		myVideo->Play(false);
		myVideo->GetSprite()->SetSizeRelativeToScreen({ 16.f / 9.f, 1.f });
		myIsPlaying = true;
}

void CutsceneManager::Update(const float& aDeltaTime)
{
	if (myVideo == nullptr) return;
	if (myInput->GetInput()->GetKeyJustDown(Keys::SPACEBAR))
	{
		StopVideo();
		return;
	}
	myVideo->Update(aDeltaTime);
	if (myVideo->GetStatus() == Tga2D::VideoStatus::VideoStatus_ReachedEnd)
	{
		myVideo = nullptr;
		myIsPlaying = false;
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
}

bool CutsceneManager::IsPlaying()
{
	return myIsPlaying;
}