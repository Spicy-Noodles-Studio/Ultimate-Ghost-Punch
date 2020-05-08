#pragma once
#ifndef PARTICLE_MANAGER
#define PARTICLE_MANAGER
#include <UserComponent.h>

class SoundEmitter;
class PlayerState;

class SoundManager : public UserComponent
{
public:

private:
	/* PARTICLE SYSTEMS */
	SoundEmitter* soundEmitter;

	/* COMPONENTS TO GET INFO */
	PlayerState* playerState;

public:
	SoundManager(GameObject* gameObject);
	virtual ~SoundManager();

	virtual void start();
	virtual void update(float deltaTime);
	
	void playJumpSound();
	void playLandSound();
	void playWalkSound();
	void playGhostMoveSound();
	void playHurtSound();
	void playBlockSound();
	void playBlockEndSound();
	void playDodgeSound();
	void playHitSound();
	void playStunSound();
	void playHeavyAttackSound();
	void playQuickAttackSound();
	void playAttackHitSound();
	void playAttackBlockedSound();
	void playGrabMissSound();
	void playGrabSound();
	void playThrowSound();
	void playGrabBlockedSound();
	void playUGPAimSound();
	void playUGPSound();
	void playUGPSuccesSound();
	void playGhostDieSound();
	void playGhostSuccesSound();
	void playGhostSound();
	void playRespawnSound();
private:
	void playSound(const std::string& sound);
};

#endif