#include "SoundManager.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <SoundEmitter.h>

#include "PlayerState.h"

REGISTER_FACTORY(SoundManager);

SoundManager::SoundManager(GameObject* gameObject) :	UserComponent(gameObject), soundEmitter(nullptr)
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
	playJumpSound();
	playGhostMoveSound();
	playHurtSound();
	playAttackBlockedSound();
}

void SoundManager::playJumpSound()
{
	if (playerState == nullptr && playerState->isGrounded() && playerState->isJumping())
		playSound("jump");
}

void SoundManager::playLandSound()
{
	playSound("land");
}

void SoundManager::playWalkSound()
{
	//if ( playerState == nullptr && playerState->isGrounded() && playerState->isMoving() && playerState->canMove())
	//	playSound("");
}

void SoundManager::playGhostMoveSound()
{
	if (playerState != nullptr && playerState->canGhostMove())
		playSound("heartBeat");
}

void SoundManager::playHurtSound()
{
	if (playerState == nullptr && playerState->isHurt())
		playSound("hurt2");
}

void SoundManager::playBlockSound()
{

}

void SoundManager::playQuickAttackSound()
{
}

void SoundManager::playAttackHitSound()
{
}

void SoundManager::playAttackBlockedSound()
{
	if (playerState == nullptr && playerState->isBlocking() && playerState->hasBlocked())
		playSound("block");
}

void SoundManager::playGrabMissSound()
{
}

void SoundManager::playGrabSound()
{
}

void SoundManager::playThrowSound()
{
}

void SoundManager::playGrabBlockedSound()
{
}

void SoundManager::playBlockEndSound()
{
}

void SoundManager::playDodgeSound()
{
	if (playerState == nullptr && playerState->isBlocking() && playerState->hasBlocked())
		playSound("block");
}

void SoundManager::playHitSound()
{
}

void SoundManager::playStunSound()
{
}

void SoundManager::playHeavyAttackSound()
{
}

void SoundManager::playUGPAimSound()
{
}

void SoundManager::playUGPSound()
{
}

void SoundManager::playUGPSuccesSound()
{
}

void SoundManager::playGhostDieSound()
{
}

void SoundManager::playGhostSuccesSound()
{
}

void SoundManager::playGhostSound()
{
}

void SoundManager::playRespawnSound()
{
}

void SoundManager::playSound(const std::string& sound)
{
	if (soundEmitter != nullptr && !soundEmitter->isPlaying(sound)) soundEmitter->playSound(sound);
}

