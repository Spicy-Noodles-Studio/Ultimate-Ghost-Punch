#include "SoundManager.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <SoundEmitter.h>
#include <MathUtils.h>

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

SoundManager::SoundManager(GameObject* gameObject) : UserComponent(gameObject), soundEmitter(nullptr), playerState(nullptr), attackStarted(false), ghostStarted(false), blockGrabStarted(false), jumpStarted(true),
													 aimStarted(false), grabStarted(false), respawnStarted(false), punchStarted(false), dodgeStarted(false), ghostSuccess(false), deathStarted(false), punchSuccess(false),
													 jumpSounds(std::vector<std::string>()), tauntSounds(std::vector<std::string>()), throwSounds(std::vector<std::string>()), hurtSounds(std::vector<std::string>()), hitSounds(std::vector<std::string>())
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

	initSounds();
	soundEmitter = gameObject->getComponent<SoundEmitter>();
}

void SoundManager::update(float deltaTime)
{
	manageSounds();
}

void SoundManager::playTaunt()
{
	playSound(getRandomSound(tauntSounds));
}

void SoundManager::manageJumpSound()
{
	playSoundOnce(getRandomSound(jumpSounds), isJumping, jumpStarted);
}

void SoundManager::manageLandSound()
{
	playSoundInstant2("land", hasLanded, canMove);
}

void SoundManager::manageWalkSound()
{
	if (playerState != nullptr && playerState->isGrounded() && playerState->isMoving() && playerState->canMove()) 
		playSound("run");
	else
		stopSound("run");
}

void SoundManager::manageGhostMoveSound()
{
	playSoundRepeatedly("heartBeat", isGhost)	
}

void SoundManager::manageHurtSound()
{
	playSoundInstant(getRandomSound(hurtSounds), isHurt)
}

void SoundManager::manageBlockSound()
{
	playSoundRepeatedly("shield", isBlocking)
}

void SoundManager::manageQuickAttackSound()
{
	playSoundOnce("swipe", isQuickAttacking, attackStarted)
}

void SoundManager::manageAttackHitSound()
{
	playSoundInstant(getRandomSound(hitSounds), hasHit)
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
	playSoundInstant(getRandomSound(throwSounds), hasDroppedGrab)
}

void SoundManager::manageGrabBlockedSound()
{
	playSoundOnce("swordOnShield", hasBlockedGrab, blockGrabStarted)
}

void SoundManager::manageDodgeSound()
{
	playSoundOnce("dash",isDodging,dodgeStarted)
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
	playSoundOnce("ghost", isAiming, aimStarted)
}

void SoundManager::manageUGPSound()
{
	playSoundOnce("ugp",isPunching, punchStarted)
}

void SoundManager::manageGhostDieSound()
{
	playSoundInstant("noo",hasGhostDied)
}

void SoundManager::manageGhostSuccessSound()
{
	playSoundOnce("ghostLaugh", hasGhostSucceeded, ghostSuccess)
}

void SoundManager::manageGhostSound()
{
	playSoundOnce("ghostSee",isGhost,ghostStarted)
}

void SoundManager::manageRespawnSound()
{
	playSoundOnce("respawn",isRespawning,respawnStarted)
}

void SoundManager::manageDeathSound()
{
	playSoundOnce("death", hasKnightDied, deathStarted);
}

void SoundManager::managePunchSuccessSound()
{
	playSoundOnce("punch", punchSucceeded, punchSuccess);
}

void SoundManager::playSound(const std::string& sound)
{
	if (soundEmitter != nullptr && !soundEmitter->isPlaying(sound)) soundEmitter->playSound(sound);
}

void SoundManager::stopSound(const std::string& sound)
{
	if (soundEmitter != nullptr && soundEmitter->isPlaying(sound)) soundEmitter->stop(sound);
}

void SoundManager::initSounds()
{
	hurtSounds = { "hurt1", "hurt2" ,"hurt3" ,"hurt4" };
	tauntSounds = { "taunt1", "taunt2", "taunt3" };
	jumpSounds = { "jump1", "jump2" ,"jump3"};
	throwSounds = { "throw1", "throw2" ,"throw3"};
	hitSounds = { "hit1", "hit2" ,"hit3"};
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
	manageDeathSound();
	managePunchSuccessSound();
}

std::string SoundManager::getRandomSound(const std::vector<std::string>& sounds)
{
	return sounds[random(0, sounds.size())];
}
