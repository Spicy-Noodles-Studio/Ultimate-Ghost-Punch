#include "TrailManager.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <Trail.h>
#include <MeshRenderer.h>
#include <GaiaData.h>
#include <sstream>

#include "PlayerState.h"

REGISTER_FACTORY(TrailManager);

TrailManager::TrailManager(GameObject* gameObject) : UserComponent(gameObject), quickAttackTrail(nullptr), heavyAttackTrail(nullptr),
time(0), thrownTime(1)
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

	createTrail(&heavyAttackTrail, "heavyAttackTrail");
	createTrail(&quickAttackTrail, "quickAttackTrail");
	createTrail(&dashTrail, "dashTrail");
	dashTrail->setColour(gameObject->getComponent<MeshRenderer>()->getDiffuse(0), 1);
	createTrail(&thrownTrail, "thrownTrail");
	thrownTrail->setColour(gameObject->getComponent<MeshRenderer>()->getDiffuse(0), 1);
	createTrail(&UGPTrail, "UGPTrail");
}

void TrailManager::preUpdate(float deltaTime)
{
	manageQuickAttackTrail();
	manageHeavyAttackTrail();
	manageDashTrail();
	manageThrownTrail(deltaTime);
	manageUGPTrail();
}

void TrailManager::createTrail(Trail** trail, const std::string& trailFilename)
{
	if (*trail != nullptr) return;

	GameObject* trailObject = instantiate("Trail");
	if (trailObject == nullptr)
		return;

	gameObject->addChild(trailObject);

	*trail = trailObject->getComponent<Trail>();
	if (*trail == nullptr)
		return;

	// Set parameters from file
	MeshRenderer* mesh = gameObject->getComponent<MeshRenderer>();
	(*trail)->setMeshRenderer(mesh);
	(*trail)->configureTrail("./Assets/Trails/" + trailFilename + ".trail");
}

void TrailManager::manageQuickAttackTrail()
{
	if (quickAttackTrail == nullptr) return;

	if (playerState->isQuickAttacking())
		quickAttackTrail->start();
	else
		quickAttackTrail->stop();
}

void TrailManager::manageHeavyAttackTrail()
{
	if (heavyAttackTrail == nullptr) return;

	if (playerState->isHeavyAttacking()) {
		if (!heavyAttackTrail->started()) heavyAttackTrail->start();
	}
	else {
		if (heavyAttackTrail->started()) heavyAttackTrail->stop();
	}
}

void TrailManager::manageDashTrail()
{
	if (dashTrail == nullptr) return;

	if (playerState->isDodging()) {
		if (!dashTrail->started()) dashTrail->start();
	}
	else {
		if (dashTrail->started()) dashTrail->stop();
	}
}

void TrailManager::manageThrownTrail(float deltaTime)
{
	if (thrownTrail == nullptr) return;

	if (playerState->hasBeenThrown()) {
		time = thrownTime;
		if (!thrownTrail->started()) thrownTrail->start();
	}

	if (time > 0)
	{
		time -= deltaTime;
	}
	else if (time <= 0)
	{
		if (thrownTrail->started()) thrownTrail->stop();
	}
}

void TrailManager::manageUGPTrail()
{
	if (UGPTrail == nullptr) return;

	if (playerState->isPunching()) {
		if (!UGPTrail->started()) UGPTrail->start();
	}
	else {
		if (UGPTrail->started()) UGPTrail->stop();
	}
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