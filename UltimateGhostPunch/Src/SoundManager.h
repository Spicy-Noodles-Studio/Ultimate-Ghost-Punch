#pragma once
#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <UserComponent.h>

class SoundEmitter;
class PlayerState;

class SoundManager : public UserComponent
{
private:
	/* PARTICLE SYSTEMS */
	SoundEmitter* soundEmitter;

	/* COMPONENTS TO GET INFO */
	PlayerState* playerState;

	bool attackStarted;
	bool aimStarted;
	bool ghostStarted;
	bool grabStarted;
	bool blockGrabStarted;
	bool respawnStarted;
	bool punchStarted;
	bool dodgeStarted;
	bool ghostSuccess;
	bool deathStarted;
	bool punchSuccess;
	bool jumpStarted;

	std::vector<std::string> hurtSounds;
	std::vector<std::string> hitSounds;
	std::vector<std::string> tauntSounds;
	std::vector<std::string> jumpSounds;
	std::vector<std::string> throwSounds;

protected:
	virtual void start();
	virtual void update(float deltaTime);

public:
	SoundManager(GameObject* gameObject);
	virtual ~SoundManager();

	void playTaunt();
	void stopAll();

private:
	void playSound(const std::string& sound);
	void stopSound(const std::string& sound);

	void initSounds();

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
	void managePunchSuccessSound();
};

#endif