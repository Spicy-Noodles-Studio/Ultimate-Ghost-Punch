#pragma once
#ifndef PARTICLE_MANAGER
#define PARTICLE_MANAGER
#include <UserComponent.h>

class ParticleEmitter;
class PlayerState;

class ParticleManager : public UserComponent
{
public:

private:
	/* PARTICLE SYSTEMS */
	ParticleEmitter* floorDust;
	ParticleEmitter* jumpDust;
	ParticleEmitter* landDust;
	ParticleEmitter* bloodSplash;
	ParticleEmitter* blockSparks;
	ParticleEmitter* stunSparks;
	ParticleEmitter* spectre;

	/* COMPONENTS TO GET INFO */
	PlayerState* playerState;

public:
	ParticleManager(GameObject* gameObject);
	virtual ~ParticleManager();

	virtual void start();
	virtual void update(float deltaTime);

private:
	/* CREATE CHILDREN */
	void createParticle(ParticleEmitter** emitter, const std::string& particleName, const Vector3& position = Vector3::ZERO);

	/* MANAGE PARTICLES */
	void manageFloorDust();
	void manageJumpDust();
	void manageLandDust();
	void manageBloodSplash();
	void manageBlockSparks();
	void manageStunSparks();
	void manageSpectre();
};

#endif