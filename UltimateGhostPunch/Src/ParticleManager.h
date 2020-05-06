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
	//ParticleEmmiter* blood;
	//ParticleEmmiter* blockSparks;

	/* COMPONENTS TO GET INFO */
	PlayerState* playerState;

public:
	ParticleManager(GameObject* gameObject);
	virtual ~ParticleManager();

	virtual void start();
	virtual void update(float deltaTime);
private:
	/* CREATE CHILDREN */
	void generateFloorDust();
	void generateJumpDust();
	//void generateBlood();
	//void generateBlockSparks();

	/* MANAGE PARTICLES */
	void manageFloorDust();
	void manageJumpDust();
	//void manageBlood();
	//void manageBlockSparks();
};

#endif