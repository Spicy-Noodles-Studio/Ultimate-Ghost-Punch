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
	ParticleEmitter* bloodSplash;
	ParticleEmitter* blockSparks;
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
	void createFloorDust();
	void createJumpDust();
	//void createBloodSplash();
	//void createBlockSparks();

	/* MANAGE PARTICLES */
	void manageFloorDust();
	void manageJumpDust();
	void manageBloodSplash();
	void manageBlockSparks();
	void manageSpectre();
};

#endif