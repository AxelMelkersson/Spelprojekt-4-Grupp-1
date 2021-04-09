#pragma once
#include "GameObject.h"

class Scene;

class CreditsMenu : public GameObject
{
public:
	CreditsMenu(Scene* aScene);
	~CreditsMenu() = default;

};

