#include "SoundManager.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <SoundEmitter.h>

#include "PlayerState.h"

REGISTER_FACTORY(SoundManager);

#define playSoundInstant( sound, condition)\
if (playerState != nullptr && playerState->condition())\
playSound(sound);

#define playSoundInstant2( sound, condition1, condition2)\
if (playerState != nullptr && playerState->condition1() && playerState->condition2())\
playSound(sound);

#define playSoundOnce(sound, condition, boolean)\
if (playerState != nullptr && playerState->condition() && !boolean) {\
	playSound(sound);\
	boolean = true;\
}\
else if (playerState != nullptr && !playerState->condition())\
	boolean = false;

#define playSoundRepeatedly(sound, condition)\
if (playerState != nullptr && playerState->condition())\
	playSound(sound);\
else\
	stopSound(sound);

SoundManager::SoundManager(GameObject* gameObject) : UserComponent(gameObject), soundEmitter(nullptr), playerState(nullptr), attackStarted(false), ghostStarted(false), blockGrabStarted(false),
													 aimStarted(false), grabStarted(false), respawnStarted(false), punchStarted(false), dodgeStarted(false), ghostSuccess(false)
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
	playSoundInstant2("jump", isGrounded, isJumping);
}

void SoundManager::manageLandSound()
{
	playSoundInstant2("land", hasLanded, canMove);
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
	playSoundRepeatedly("heartBeat2", canGhostMove)	
}

void SoundManager::manageHurtSound()
{
	playSoundInstant("hurt2", isHurt)
}

void SoundManager::manageBlockSound()
{
	playSoundRepeatedly("shield", isBlocking)
}

void SoundManager::manageQuickAttackSound()
{
	playSoundOnce("swipe2", isQuickAttacking, attackStarted)
}

void SoundManager::manageAttackHitSound()
{
	playSoundInstant("hit2", hasHit)
}

void SoundManager::manageAttackBlockedSound()
{
	playSoundInstant2("block",isBlocking, hasBlocked)
}

void SoundManager::manageGrabMissSound()
{
	playSoundInstant("swipe", hasMissedGrab)
}

void SoundManager::manageGrabSound()
{
	playSoundOnce("grab",isGrabbing, grabStarted)
}

void SoundManager::manageThrowSound()
{
	playSoundInstant("throw1", hasDroppedGrab)
}

void SoundManager::manageGrabBlockedSound()
{
	playSoundOnce("swordOnShield", hasBlockedGrab, blockGrabStarted)
}

void SoundManager::manageDodgeSound()
{
	playSoundOnce("dash2",isDodging,dodgeStarted)
}

void SoundManager::manageStunSound()
{
	playSoundRepeatedly("stun", isStunned)
}

void SoundManager::manageHeavyAttackSound()
{
	playSoundRepeatedly("heavyAttack", isHeavyAttacking)
}

void SoundManager::manageUGPAimSound()
{
	playSoundOnce("ghost2", isAiming, aimStarted)
}

void SoundManager::manageUGPSound()
{
	playSoundOnce("ugp2",isPunching, punchStarted)
}

void SoundManager::manageGhostDieSound()
{
	playSoundInstant("noo",hasGhostDied)
}

void SoundManager::manageGhostSuccessSound()
{
	playSoundOnce("ghostLaugh2", hasGhostSucceeded, ghostSuccess)
}

void SoundManager::manageGhostSound()
{
	playSoundOnce("ghostSee1",isGhost,ghostStarted)
}

void SoundManager::manageRespawnSound()
{
	playSoundOnce("respawn",isRespawning,respawnStarted)
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

