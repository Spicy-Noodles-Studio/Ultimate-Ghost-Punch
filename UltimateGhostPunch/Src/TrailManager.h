#pragma once
#ifndef TRAIL_MANAGER
#define TRAIL_MANAGER
#include <UserComponent.h>

class Trail;
class PlayerState;

class TrailManager : public UserComponent
{
private:
	Trail* swordTrail;

	PlayerState* playerState;

	/* DELAY AND TIMER */
	float stunDelay;
	float stunTimer;

public:
	TrailManager(GameObject* gameObject);
	virtual ~TrailManager();

	virtual void start();
	virtual void preUpdate(float deltaTime);

private:
	//void createParticle(ParticleEmitter** emitter, const std::string& particleName, const Vector3& position = Vector3::ZERO);

	void createSwordTrail();

	void manageSwordTrail();
};

#endif