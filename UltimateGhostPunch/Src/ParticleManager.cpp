#include "ParticleManager.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <ParticleEmitter.h>

#include "PlayerState.h"

REGISTER_FACTORY(ParticleManager);

ParticleManager::ParticleManager(GameObject* gameObject) :	UserComponent(gameObject), floorDust(nullptr), jumpDust(nullptr), landDust(nullptr),
															bloodSplash(nullptr), blockSparks(nullptr), stunSparks(nullptr),
															spectre(nullptr), playerState(nullptr),
															stunDelay(0.0f), stunTimer(0.0f)
{

}

ParticleManager::~ParticleManager()
{

}

void ParticleManager::start()
{
	// PlayerState for info
	playerState = gameObject->getComponent<PlayerState>();
	if (playerState == nullptr)
		LOG_ERROR("PARTICLE MANAGER", "PlayerState component not found");

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

}

void ParticleManager::update(float deltaTime)
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
}

void ParticleManager::createParticle(ParticleEmitter** emitter, const std::string& particleName, const Vector3& position)
{
	if (*emitter != nullptr) return;
	
	GameObject* particlesObject = instantiate("ParticleEmitter");
	if (particlesObject == nullptr)
		return;

	// Add child
	gameObject->addChild(particlesObject);

	*emitter = particlesObject->getComponent<ParticleEmitter>();
	if (*emitter == nullptr)
		return;

	particlesObject->transform->setPosition(position);
	(*emitter)->newEmitter(particleName);
}

void ParticleManager::manageFloorDust()
{
	if (floorDust == nullptr) return;

	if (playerState->isGrounded() && playerState->isMoving() && playerState->canMove())
		floorDust->start();
	else
		floorDust->stop();
}

void ParticleManager::manageJumpDust()
{
	if (jumpDust == nullptr) return;

	if (playerState->isGrounded() && playerState->isJumping())
		jumpDust->start();
	else
		jumpDust->stop();
}

void ParticleManager::manageLandDust()
{
	if (landDust == nullptr) return;

	if (playerState->hasLanded() && playerState->canMove())
		landDust->start();
	else
		landDust->stop();
}

void ParticleManager::manageBloodSplash()
{
	if (bloodSplash == nullptr) return;

	if (playerState->isHurt())
		bloodSplash->start();
	else
		bloodSplash->stop();
}

void ParticleManager::manageBlockSparks()
{
	if (blockSparks == nullptr) return;

	if (playerState->isBlocking() && playerState->hasBlocked())
		blockSparks->start();
	else
		blockSparks->stop();
}

void ParticleManager::manageStunSparks(float deltaTime)
{
	if (stunSparks == nullptr) return;

	if (playerState->isStunned()) {
		stunTimer -= deltaTime;
		if(stunTimer <= 0.0f)
			stunSparks->start();
	}
	else {
		stunSparks->stop();
		stunTimer = stunDelay;
	}
}

void ParticleManager::manageSpectre()
{
	if (spectre == nullptr) return;

	if (playerState->canGhostMove())
		spectre->start();
	else
		spectre->stop();
}
