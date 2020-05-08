#include "SoundManager.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <SoundEmitter.h>

#include "PlayerState.h"

REGISTER_FACTORY(SoundManager);

SoundManager::SoundManager(GameObject* gameObject) : UserComponent(gameObject), soundEmitter(nullptr), playerState(nullptr), attackStarted(false), ghostStarted(false), blockGrabStarted(false),
													 aimStarted(false), grabStarted(false), respawnStarted(false), punchStarted(false), dodgeStarted(false)
{

}

SoundManager::~SoundManager()
{

}

void SoundManager::start()
{
	// PlayerState for info
	playerState = gameObject->getComponent<PlayerState>();
	if (playerState == nullptr)
		LOG_ERROR("SOUND MANAGER", "PlayerState component not found");

	soundEmitter = gameObject->getComponent<SoundEmitter>();
}

void SoundManager::update(float deltaTime)
{
	manageSounds();
}

void SoundManager::manageJumpSound()
{
	if (playerState != nullptr && playerState->isGrounded() && playerState->isJumping())
		playSound("jump");
}

void SoundManager::manageLandSound()
{
	if (playerState != nullptr && playerState->hasLanded() && playerState->canMove())
		playSound("land");
}

void SoundManager::manageWalkSound()
{
	if (playerState != nullptr && playerState->isGrounded() && playerState->isMoving() && playerState->canMove()) 
		playSound("run2");
	else
		stopSound("run2");
}

void SoundManager::manageGhostMoveSound()
{
	if (playerState != nullptr && playerState->canGhostMove())
		playSound("heartBeat2");
	else
		stopSound("heartBeat2");		
}

void SoundManager::manageHurtSound()
{
	if (playerState != nullptr && playerState->isHurt())
		playSound("hurt2");
}

void SoundManager::manageBlockSound()
{
	if (playerState != nullptr && playerState->isBlocking())
		playSound("shield");
	else
		stopSound("shield");
}

void SoundManager::manageQuickAttackSound()
{
	if (playerState != nullptr && playerState->isQuickAttacking() && !attackStarted) {
		playSound("swipe2");
		attackStarted = true;
	}
	else if (playerState != nullptr && !playerState->isQuickAttacking())
		attackStarted = false;
}

void SoundManager::manageAttackHitSound()
{
	if(playerState!= nullptr && playerState->hasHit())
		playSound("hit2");
}

void SoundManager::manageAttackBlockedSound()
{
	if (playerState != nullptr && playerState->isBlocking() && playerState->hasBlocked())
		playSound("block");
}

void SoundManager::manageGrabMissSound()
{
	if(playerState != nullptr && playerState->hasMissedGrab())
		playSound("swipe");
}

void SoundManager::manageGrabSound()
{
	if (playerState != nullptr && playerState->isGrabbing() && !grabStarted) {
		playSound("grab");
		grabStarted = true;
	}
	else if (playerState == nullptr || !playerState->isGrabbing())
		grabStarted = false;
}

void SoundManager::manageThrowSound()
{
	if(playerState != nullptr && playerState->hasDroppedGrab())
		playSound("throw1");
}

void SoundManager::manageGrabBlockedSound()
{
	if (playerState != nullptr && playerState->hasBlockedGrab() && !blockGrabStarted) {
		playSound("swordOnShield");
		blockGrabStarted = true;
	}
	else if (playerState != nullptr && !playerState->hasBlockedGrab())
		blockGrabStarted = false; 
}

void SoundManager::manageDodgeSound()
{
	if (playerState != nullptr && playerState->isDodging() && !dodgeStarted) {
		playSound("dash2");
		dodgeStarted = true;
	}
	else if (playerState != nullptr && !playerState->isDodging())
		dodgeStarted = false;
}

void SoundManager::manageStunSound()
{
	if (playerState != nullptr && playerState->isStunned())
		playSound("stun");
	else
		stopSound("stun");
}

void SoundManager::manageHeavyAttackSound()
{
	if (playerState != nullptr && playerState->isHeavyAttacking())
		playSound("heavyAttack");
	else 
		stopSound("heavyAttack");
}

void SoundManager::manageUGPAimSound()
{
	if (playerState != nullptr && playerState->isAiming() && !aimStarted) {
		playSound("ghost");
		aimStarted = true;
	}
	else if (playerState == nullptr || !playerState->isAiming())
		aimStarted = false;
}

void SoundManager::manageUGPSound()
{
	if (playerState != nullptr && playerState->isPunching() && !punchStarted) {
		playSound("ugp2");
		punchStarted = true;
	}
	else
		punchStarted = false;
}

void SoundManager::manageGhostDieSound()
{
	if(playerState!= nullptr && playerState->hasGhostDied())
		playSound("noo");
}

void SoundManager::manageGhostSuccessSound()
{
	if (playerState != nullptr && playerState->hasGhostSucceeded())
		playSound("ghostLaugh");
}

void SoundManager::manageGhostSound()
{
	if (playerState != nullptr && playerState->isGhost() && !ghostStarted) {
		playSound("ghostSee1");
		ghostStarted = true;
	}
	else if (playerState == nullptr || !playerState->isGhost()) 
		ghostStarted = false;
}

void SoundManager::manageRespawnSound()
{
	if (playerState != nullptr && playerState->isRespawning() && !respawnStarted) {
		playSound("respawn");
		respawnStarted = true;
	}
	else if(playerState != nullptr && !playerState->isRespawning())
		respawnStarted = false;
}

void SoundManager::playSound(const std::string& sound)
{
	if (soundEmitter != nullptr && !soundEmitter->isPlaying(sound)) soundEmitter->playSound(sound);
}

void SoundManager::stopSound(const std::string& sound)
{
	if (soundEmitter != nullptr && soundEmitter->isPlaying(sound)) soundEmitter->stop(sound);
}

void SoundManager::manageSounds()
{
	manageJumpSound();
	manageLandSound();
	manageWalkSound();
	manageGhostMoveSound();
	manageHurtSound();
	manageBlockSound();
	manageDodgeSound();
	manageStunSound();
	manageHeavyAttackSound();
	manageQuickAttackSound();
	manageAttackHitSound();
	manageAttackBlockedSound();
	manageGrabMissSound();
	manageGrabSound();
	manageThrowSound();
	manageGrabBlockedSound();
	manageUGPAimSound();
	manageUGPSound();
	manageGhostDieSound();
	manageGhostSuccessSound();
	manageGhostSound();
	manageRespawnSound();
}

