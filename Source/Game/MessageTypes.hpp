#pragma once
enum class eMessageType
{
	LoadNext,
	LoadPrevious,
	LoadHiddenRoom,
	LoadMainRoom,
	PlayerDeath,
	PlayerSafeLanded,
	PlayerReachedBonfire,
	PlayerLandedParticle,
	PlayerBashedPlayerParticle,
	PlayerBashedSmallParticle,
	PlayerLedgeRightGrabbedHandParticle,
	PlayerLedgeRightGrabbedLegParticle,
	PlayerLedgeLeftGrabbedHandParticle,
	PlayerLedgeLeftGrabbedLegParticle,
	EnemyShootingBulletHitParticle,
	EnemyShootingBulletParticle,
	EnemyShootingTrailParticle,
	RainEffectForegroundParticle,
	RainEffectBackgroundParticle,
	RainEffectNextScreenParticle,
	VelocityLinesParticle,
	CollectibleCollectedParticle,
	KilledFocus,
	Vibrate
};