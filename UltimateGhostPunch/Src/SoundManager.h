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

	bool attackStarted, aimStarted, ghostStarted, grabStarted, blockGrabStarted, respawnStarted, punchStarted, dodgeStarted, ghostSuccess, deathStarted;

public:
	SoundManager(GameObject* gameObject);
	virtual ~SoundManager();

	virtual void start();
	virtual void update(float deltaTime);

	void playTaunt();
	
private:
	void playSound(const std::string& sound);
	void stopSound(const std::string& sound);

	void manageSounds();

	std::string getRandomSound(const std::vector<std::string>& sounds);

	void manageJumpSound();
	void manageLandSound();
	void manageWalkSound();
	void manageGhostMoveSound();
	void manageHurtSound();
	void manageBlockSound();
	void manageDodgeSound();
	void manageStunSound();
	void manageHeavyAttackSound();
	void manageQuickAttackSound();
	void manageAttackHitSound();
	void manageAttackBlockedSound();
	void manageGrabMissSound();
	void manageGrabSound();
	void manageThrowSound();
	void manageGrabBlockedSound();
	void manageUGPAimSound();
	void manageUGPSound();
	void manageGhostDieSound();
	void manageGhostSuccessSound();
	void manageGhostSound();
	void manageRespawnSound();
	void manageDeathSound();
};

#endif