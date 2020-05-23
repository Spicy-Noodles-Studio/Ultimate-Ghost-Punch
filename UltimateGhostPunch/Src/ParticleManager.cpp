#include "ParticleManager.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <ParticleEmitter.h>

#include "PlayerState.h"

REGISTER_FACTORY(ParticleManager);

ParticleManager::ParticleManager(GameObject* gameObject) : UserComponent(gameObject), floorDust(nullptr), jumpDust(nullptr), landDust(nullptr),
bloodSplash(nullptr), blockSparks(nullptr), stunSparks(nullptr), spectre(nullptr), spectreSplash(nullptr), playerState(nullptr),
stunDelay(0.0f), stunTimer(0.0f)
{

}

ParticleManager::~ParticleManager()
{

}

void ParticleManager::stopAll()
{
	if (notNull(floorDust))
		floorDust->stop();

	if (notNull(jumpDust))
		jumpDust->stop();

	if (notNull(landDust))
		landDust->stop();

	if (notNull(bloodSplash))
		bloodSplash->stop();

	if (notNull(blockSparks))
		blockSparks->stop();

	if (notNull(stunSparks))
		stunSparks->stop();

	if (notNull(spectre))
		spectre->stop();

	if (notNull(spectreSplash))
		spectreSplash->stop();
}

void ParticleManager::start()
{
	// PlayerState for info
	playerState = gameObject->getComponent<PlayerState>();
	checkNull(playerState);

	/* FLOOR DUST */
	createParticle(&floorDust, "FloorDust", Vector3::NEGATIVE_UP * 2.5f);

	/* JUMP DUST */
	createParticle(&jumpDust, "JumpDust", Vector3::NEGATIVE_UP * 2.5f);

	/* LAND DUST */
	createParticle(&landDust, "LandDust", Vector3::NEGATIVE_UP * 2.5f);

	/* BLOOD SPLASH */
	createParticle(&bloodSplash, "BloodSplash");

	/* BLOCK SPARKS */
	createParticle(&blockSparks, "BlockSparks");

	/* STUN SPARKS */
	createParticle(&stunSparks, "StunSparks", Vector3::UP * 2.5f);
	stunDelay = 0.5f;

	/* SPECTRE */
	createParticle(&spectre, "Spectre");

	/* SPECTRE SPLASH */
	createParticle(&spectreSplash, "SpectreSplash");
}

void ParticleManager::preUpdate(float deltaTime)
{
	/* FLOOR DUST */
	manageFloorDust();

	/* JUMP DUST */
	manageJumpDust();

	/* LAND DUST */
	manageLandDust();

	/* BLOOD SPLASH */
	manageBloodSplash();

	/* BLOCK SPARKS */
	manageBlockSparks();

	/* STUN SPARKS */
	manageStunSparks(deltaTime);

	/* SPECTRE */
	manageSpectre();

	/* SPECTRE SPLASH */
	manageSpectreSplash();
}

void ParticleManager::createParticle(ParticleEmitter** emitter, const std::string& particleName, const Vector3& position)
{
	if (*emitter != nullptr) return;

	GameObject* particlesObject = instantiate("ParticleEmitter");
	checkNullAndBreak(particlesObject);

	// Add child
	gameObject->addChild(particlesObject);

	*emitter = particlesObject->getComponent<ParticleEmitter>();
	checkNullAndBreak(*emitter);

	if (notNull(particlesObject->transform))
		particlesObject->transform->setPosition(position);

	(*emitter)->newEmitter(particleName);
}

void ParticleManager::manageFloorDust()
{
	checkNullAndBreak(playerState);
	checkNullAndBreak(floorDust);

	if (playerState->isGrounded() && playerState->isMoving() && playerState->canMove())
		floorDust->start();
	else
		floorDust->stop();
}

void ParticleManager::manageJumpDust()
{
	checkNullAndBreak(playerState);
	checkNullAndBreak(jumpDust);

	if ((playerState->isGrounded() && playerState->isJumping()) || playerState->hasJumped())
		jumpDust->start();
	else
		jumpDust->stop();
}

void ParticleManager::manageLandDust()
{
	checkNullAndBreak(playerState);
	checkNullAndBreak(landDust);

	if (playerState->hasLanded() && playerState->canMove())
		landDust->start();
	else
		landDust->stop();
}

void ParticleManager::manageBloodSplash()
{
	checkNullAndBreak(playerState);
	checkNullAndBreak(bloodSplash);

	if (playerState->isHurt())
		bloodSplash->start();
	else
		bloodSplash->stop();
}

void ParticleManager::manageBlockSparks()
{
	checkNullAndBreak(playerState);
	checkNullAndBreak(blockSparks);

	if (playerState->isBlocking() && playerState->hasBlocked())
		blockSparks->start();
	else
		blockSparks->stop();
}

void ParticleManager::manageStunSparks(float deltaTime)
{
	checkNullAndBreak(playerState);
	checkNullAndBreak(stunSparks);

	if (playerState->isStunned())
	{
		stunTimer -= deltaTime;
		if (stunTimer <= 0.0f)
			stunSparks->start();
	}
	else
	{
		stunSparks->stop();
		stunTimer = stunDelay;
	}
}

void ParticleManager::manageSpectre()
{
	checkNullAndBreak(playerState);
	checkNullAndBreak(spectre);

	if (playerState->canGhostMove())
		spectre->start();
	else
		spectre->stop();
}

void ParticleManager::manageSpectreSplash()
{
	checkNullAndBreak(playerState);
	checkNullAndBreak(spectreSplash);

	if (playerState->hasPunchSucceeded())
		spectreSplash->start();
	else
		spectreSplash->stop();
}