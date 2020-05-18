#include "TrailManager.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <Trail.h>
#include <MeshRenderer.h>

#include "PlayerState.h"

REGISTER_FACTORY(TrailManager);

TrailManager::TrailManager(GameObject* gameObject) : UserComponent(gameObject), swordTrail(nullptr),
stunDelay(0.0f), stunTimer(0.0f)
{

}

TrailManager::~TrailManager()
{

}

void TrailManager::start()
{
	// PlayerState for info
	playerState = gameObject->getComponent<PlayerState>();
	if (playerState == nullptr)
		LOG_ERROR("TRAIL MANAGER", "PlayerState component not found");

	createSwordTrail();
}

void TrailManager::preUpdate(float deltaTime)
{
	manageSwordTrail();
}

void TrailManager::createSwordTrail()
{
	GameObject* trailGameObject = instantiate("Trail");
	if (trailGameObject == nullptr)
		return;

	gameObject->addChild(trailGameObject);

	swordTrail= trailGameObject->getComponent<Trail>();
	if (swordTrail== nullptr)
		return;

	swordTrail->setOffset(Vector3(-50, 100, 0));
	MeshRenderer* mesh = gameObject->getComponent<MeshRenderer>();
	swordTrail->newTrail("Mano.L", mesh);
	swordTrail->setLength(20);
	swordTrail->setMax(40);
	swordTrail->setColour(Vector3(0.8, 0.8, 0.8), 1);
	swordTrail->setWidth(2);
	swordTrail->setColourChange(Vector3(1, 1, 1), 2);
}

void TrailManager::manageSwordTrail()
{
	if (swordTrail == nullptr) return;

	if (playerState->isHeavyAttacking())
		swordTrail->start();
	else
		swordTrail->stop();

	/*
	particlesObject->transform->setPosition(position);
	(*emitter)->newEmitter(particleName);*/
}

/*
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

void ParticleManager::manageSpectreSplash()
{
	if (spectreSplash == nullptr) return;

	if (playerState->punchHasSucceeded())
		spectreSplash->start();
	else
		spectreSplash->stop();
}
*/