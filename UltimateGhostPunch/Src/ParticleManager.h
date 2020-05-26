#pragma once
#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#include <UserComponent.h>

class ParticleEmitter;
class PlayerState;

class ParticleManager : public UserComponent
{
private:
	/* PARTICLE SYSTEMS */
	ParticleEmitter* floorDust;
	ParticleEmitter* jumpDust;
	ParticleEmitter* landDust;
	ParticleEmitter* bloodSplash;
	ParticleEmitter* blockSparks;
	ParticleEmitter* stunSparks;
	ParticleEmitter* spectre;
	ParticleEmitter* spectreSplash;

	/* COMPONENTS TO GET INFO */
	PlayerState* playerState;

	/* DELAY AND TIMER */
	float stunDelay;
	float stunTimer;

protected:
	virtual void start();
	virtual void preUpdate(float deltaTime);

public:
	ParticleManager(GameObject* gameObject);
	virtual ~ParticleManager();

	void stopAll();

private:
	/* CREATE CHILDREN */
	void createParticle(ParticleEmitter** emitter, const std::string& particleName, const Vector3& position = Vector3::ZERO);

	/* MANAGE PARTICLES */
	void manageFloorDust();
	void manageJumpDust();
	void manageLandDust();
	void manageBloodSplash();
	void manageBlockSparks();
	void manageStunSparks(float deltaTime);
	void manageSpectre();
	void manageSpectreSplash();
};

#endif