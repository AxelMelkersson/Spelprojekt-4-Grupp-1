#include "stdafx.h"
#include "IntroLogosScene.h"
#include "SpriteComponent.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "Game.h"

enum class eIntroLogo
{
	tga_logo,
	group_logo,

	enum_intro_logo_size
};

IntroLogosScene::IntroLogosScene()
{
	for (size_t i = 0; i < static_cast<size_t>(eIntroLogo::enum_intro_logo_size); i++)
	{
		mySpriteVector.push_back(nullptr);
	}
	myLogo = nullptr;
	myTimeSinceInited = 0.0f;
	myTotalLogoTime = 5.0f;
}

void IntroLogosScene::Update(const float &aDeltaTime)
{
	myLogo->SetPosition({ static_cast<float>(Config::windowWidth) / 2.0f, static_cast<float>(Config::windowHeight) / 2.0f });
	myTimeSinceInited += aDeltaTime;

	if (myTimeSinceInited > myTotalLogoTime)
	{
		CGameWorld::GetInstance()->GetLevelManager().SingleLoadScene(LevelManager::eScenes::MainMenu);
	}
	else if (myTimeSinceInited <= myTotalLogoTime)
	{
		DisplayLogo(myTimeSinceInited);
	}

	Scene::Update(aDeltaTime);
}

void IntroLogosScene::DisplayLogo(const float aTime)
{
	unsigned int timeInt = static_cast<int>(aTime);

	switch (timeInt)
	{
	case 0:
	{
		if (!(mySpriteVector[0]->IsActive()))
		{
			mySpriteVector[0]->Activate();
		}
	}
	break;
	case 3:
	{
		if (!(mySpriteVector[1]->IsActive()))
		{
			mySpriteVector[0]->Deactivate();
			mySpriteVector[1]->Activate();
		}
	}
	break;
	default:
	break;
	}
}

void IntroLogosScene::Load()
{
	myLogo = new GameObject(this);
	myLogo->SetPivot({ 0.5f, 0.5f });
	myLogo->SetPosition({ static_cast<float>(Config::windowWidth) / 2.0f, static_cast<float>(Config::windowHeight) / 2.0f });

	mySpriteVector[static_cast<size_t>(eIntroLogo::tga_logo)] = myLogo->AddComponent<SpriteComponent>();
	mySpriteVector[static_cast<size_t>(eIntroLogo::tga_logo)]->SetSpritePath("Sprites/tgalogo_W.dds");
	const float sizeFactor = (Config::windowWidth / 3) / (mySpriteVector[static_cast<size_t>(eIntroLogo::tga_logo)]->GetImageSize().x);
	mySpriteVector[static_cast<size_t>(eIntroLogo::tga_logo)]->SetSize(mySpriteVector[static_cast<size_t>(eIntroLogo::tga_logo)]->GetSize() * sizeFactor);
	mySpriteVector[static_cast<size_t>(eIntroLogo::tga_logo)]->Deactivate();

	mySpriteVector[static_cast<size_t>(eIntroLogo::group_logo)] = myLogo->AddComponent<SpriteComponent>();
	mySpriteVector[static_cast<size_t>(eIntroLogo::group_logo)]->SetSpritePath("Sprites/Tommy.dds");
	mySpriteVector[static_cast<size_t>(eIntroLogo::group_logo)]->Deactivate();

	Scene::Load();
}
