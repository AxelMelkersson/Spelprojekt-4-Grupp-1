#include "stdafx.h"
#include "AudioLibrary.h"
#include "AudioClip.h"

AudioLibrary::AudioLibrary()
{
	myAudioList.insert_or_assign(AudioList::BashRelease,     new AudioClip("Sounds/Effects/PushfromBash.wav",             false, 0.4f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::BonfireActivated,new AudioClip("Sounds/Effects/BonfireActivated.wav", false, 0.2f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::CollectableV1,   new AudioClip("Sounds/Effects/Collectible_V1.wav",   false, 0.2f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::PlayerRespawn,   new AudioClip("Sounds/Effects/Respawn.wav",          false, 0.07, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::PlayerHover,     new AudioClip("Sounds/Effects/HoverAbility.wav",     true,  0.2f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::PlayerJumpPad,   new AudioClip("Sounds/Effects/spring.wav",           false, 0.4f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::SpikeHit,        new AudioClip("Sounds/Effects/SpikeHit.wav",         false, 0.1f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::SpikeDeath,      new AudioClip("Sounds/Effects/SpikeDeath.mp3",       false, 1.0f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::FallDeath,       new AudioClip("Sounds/Effects/FallDeath.wav",        false, 0.8f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::ThornDeath,      new AudioClip("Sounds/Effects/ThornDeath.wav",       false, 0.8f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::EnemyDeath,      new AudioClip("Sounds/Effects/SpikeDeath.mp3",       false, 0.8f, 1,    AudioLayer::SoundEffect));

	myAudioList.insert_or_assign(AudioList::EnemyNormalIdle, new AudioClip("Sounds/Effects/IdleEnemy.wav",        true,  0.1f, 0.4f, AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::EnemyShooting,   new AudioClip("Sounds/Effects/EnemyFire.wav",        false, 0.1f, 1,    AudioLayer::SoundEffect));

	myAudioList.insert_or_assign(AudioList::GrabLedge,       new AudioClip("Sounds/Effects/GrabLedge.wav",        false, 0.1f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::GrabLedge2,      new AudioClip("Sounds/Effects/GrabLedge2.wav",       false, 0.1f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::LeaveLedge,      new AudioClip("Sounds/Effects/LeaveLedge.wav",       false, 0.1f, 1,    AudioLayer::SoundEffect));

	myAudioList.insert_or_assign(AudioList::MenuMove,        new AudioClip("Sounds/Effects/MenuMove.wav",         false, 0.2f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::MenuBack,        new AudioClip("Sounds/Effects/MenuBack.wav",         false, 0.2f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::MenuSelect,      new AudioClip("Sounds/Effects/MenuSelect.wav",       false, 0.2f, 1,    AudioLayer::SoundEffect));

	myAudioList.insert_or_assign(AudioList::WalkGravelRight, new AudioClip("Sounds/Effects/Step_Gravel_Right.wav",false, 0.4f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::WalkGravelLeft,  new AudioClip("Sounds/Effects/Step_Gravel_Left.wav", false, 0.4f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::WalkGrassRight,  new AudioClip("Sounds/Effects/Step_Grass_Right.wav", false, 0.4f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::WalkGrassLeft,   new AudioClip("Sounds/Effects/Step_Grass_Left.wav",  false, 0.4f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::WalkBrickRight,  new AudioClip("Sounds/Effects/Step_Bricks_Right.wav",false, 0.4f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::WalkBrickLeft,   new AudioClip("Sounds/Effects/Step_Bricks_Left.wav", false, 0.4f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::WalkWoodRight,   new AudioClip("Sounds/Effects/Step_Wood_Right.wav",  false, 0.4f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::WalkWoodLeft,    new AudioClip("Sounds/Effects/Step_Wood_Left.wav",   false, 0.4f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::WalkStoneRight,  new AudioClip("Sounds/Effects/Step_Stone_Right.wav", false, 0.4f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::WalkStoneLeft,   new AudioClip("Sounds/Effects/Step_Stone_Left.wav",  false, 0.4f, 1,    AudioLayer::SoundEffect));

	myAudioList.insert_or_assign(AudioList::LandOnGrassEasy, new AudioClip("Sounds/Effects/LandGrassLight.wav",   false, 0.3f, 1,   AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::LandOnGrassHeavy,new AudioClip("Sounds/Effects/LandGrassHeavy.wav",   false, 0.3f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::LandOnStoneLight,new AudioClip("Sounds/Effects/LandStoneLight.wav",   false, 0.3f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::LandOnStoneHeavy,new AudioClip("Sounds/Effects/LandStoneHeavy.wav",   false, 0.3f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::LandBricksLight, new AudioClip("Sounds/Effects/LandBricksLight.wav",  false, 0.5f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::LandBricksHeavy, new AudioClip("Sounds/Effects/LandBricksHeavy.wav",  false, 0.1f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::LandWoodLight,   new AudioClip("Sounds/Effects/LandWoodLight.wav",    false, 0.4f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::LandWoodHeavy,   new AudioClip("Sounds/Effects/LandWoodHeavy.wav",    false, 0.2f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::LandGravelLight, new AudioClip("Sounds/Effects/LandGravelLight.wav",  false, 0.4f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::LandGravelHeavy, new AudioClip("Sounds/Effects/LandGravelHeavy.wav",  false, 0.2f, 1,    AudioLayer::SoundEffect));

	myAudioList.insert_or_assign(AudioList::MovingPlatform,  new AudioClip("Sounds/Effects/MovingPlatform.wav",   true,  0.1f, 0.15f,AudioLayer::SoundEffect));

	myAudioList.insert_or_assign(AudioList::HiddenRoomUnlock,new AudioClip("Sounds/Effects/HiddenRoom.wav",       false, 0.1f, 0.15f,AudioLayer::SoundEffect));

	myAudioList.insert_or_assign(AudioList::BashCharge,      new AudioClip("Sounds/Effects/Bash.wav",     false, 0.4f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::PlayerDoubleJump,new AudioClip("Sounds/Effects/DoubleJump.wav",       false, 0.4f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::PlayerJump,      new AudioClip("Sounds/Effects/Jump.wav",             false, 0.4f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::WeakPlatform,    new AudioClip("Sounds/Effects/Temporaryplatform.wav",false, 0.2f, 0.2f, AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::PlayerDeath,     new AudioClip("Sounds/Effects/death.wav",            false, 0.3f, 1,    AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::ButtonPress,     new AudioClip("Sounds/Effects/ButtonPress.wav",      false, 0.4f, 1,    AudioLayer::SoundEffect));

	myAudioList.insert_or_assign(AudioList::MenuAmbience,    new AudioClip("Sounds/Effects/MenuAmbience.wav",     true,  0.1f, 0.3f, AudioLayer::SoundEffect));
	myAudioList.insert_or_assign(AudioList::Main_Menu,       new AudioClip("Sounds/Music/Main_Menu.wav",          true,  0.1f, 0.5f, AudioLayer::Music));
	myAudioList.insert_or_assign(AudioList::Forest_Theme,    new AudioClip("Sounds/Music/Forest_Theme.wav",       true, 0.1f, 0.5f,  AudioLayer::Music));
	myAudioList.insert_or_assign(AudioList::Village_Theme,   new AudioClip("Sounds/Music/Village_Theme.wav",      true, 0.1f, 0.5f,  AudioLayer::Music));
	myAudioList.insert_or_assign(AudioList::Castle_Theme,    new AudioClip("Sounds/Music/Castle_Theme.wav",       true, 0.1f, 0.5f,  AudioLayer::Music));

}
