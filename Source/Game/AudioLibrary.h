#pragma once
#include <map>

class AudioClip;
enum class AudioList
{
	PlayerJump, PlayerDoubleJump, PlayerDeath, PlayerRespawn, PlayerHover, PlayerJumpPad,
	EnemyNormalIdle, MenuStart, EnemyShooting,
	MenuMove, MenuSelect, MenuAmbience,
	GrabLedge, GrabLedge2, LeaveLedge,
	ButtonPress,
	SpikeDeath, SpikeHit, ThornDeath, FallDeath, EnemyDeath,
	MenuBack, HiddenRoomUnlock,
	WeakPlatform, MovingPlatform,
	WalkGravelRight, WalkGravelLeft, WalkGrassRight, WalkGrassLeft, WalkStoneRight, WalkStoneLeft, WalkWoodLeft, WalkWoodRight, WalkBrickLeft, WalkBrickRight,
	LandOnGrassHeavy, LandOnGrassEasy, LandOnStoneHeavy, LandOnStoneLight, LandBricksHeavy, LandBricksLight, LandWoodHeavy, LandWoodLight, LandGravelHeavy, LandGravelLight,
	BashCharge, BashRelease,
	CollectableV1, BonfireActivated,

	Main_Menu, Forest_Theme, Village_Theme, Castle_Theme, Finale, Wind, Crows, Castle, HiddenRoom1,

	CollectibleDown, CollectibleGone, HoverActivate, EnemyKill, SuperBash, IntroMusic, GhostAttack, GhostAtmosphere, GhostVoice
};

enum class AudioLayer
{
	SoundEffect, Music
};

class AudioLibrary
{
	//static AudioLibrary& GetInstance()
	//{
	//	static AudioLibrary lib;
	//	return lib;
	//}
	~AudioLibrary() = default;
	std::map<AudioList, AudioClip*> myAudioList;
	AudioLibrary();
	friend class AudioManager;
	friend class AudioClip;
};

