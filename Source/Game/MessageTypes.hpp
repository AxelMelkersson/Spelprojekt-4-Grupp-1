#pragma once
enum class eMessageType
{
	LoadNext,
	LoadPrevious,
	LoadHiddenRoom,
	LoadMainRoom,
	PlayerDeath,
	PlayerLandedParticle,
	PlayerBashedPlayerParticle,
	PlayerBashedSmallParticle,
	PlayerLedgeRightGrabbedHandParticle,
	PlayerLedgeRightGrabbedLegParticle,
	PlayerLedgeLeftGrabbedHandParticle,
	PlayerLedgeLeftGrabbedLegParticle,
	PlayerSafeLanded,
	PlayerReachedBonfire,
	EnemyShootingBulletHit,
	EnemyShootingBulletParticle,
	EnemyShootingTrailParticle,
	Vibrate
};