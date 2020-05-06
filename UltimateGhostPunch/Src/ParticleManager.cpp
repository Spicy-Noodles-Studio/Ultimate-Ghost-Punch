#include "ParticleManager.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <ParticleEmitter.h>

#include "PlayerState.h"

REGISTER_FACTORY(ParticleManager);

ParticleManager::ParticleManager(GameObject* gameObject) : UserComponent(gameObject), floorDust(nullptr), jumpDust(nullptr), bloodSplash(nullptr)
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

	std::vector<GameObject*> children = gameObject->findChildrenWithTag("particles");
	int size = children.size();
	if (!size) return;
	// TODO: instanciar los hijos por codigo para no depender del archivo
	if(size > 0)
		floorDust = children[0]->getComponent<ParticleEmitter>();
	if (size > 1)
		jumpDust = children[1]->getComponent<ParticleEmitter>();
	if (size > 2)
		bloodSplash = children[2]->getComponent<ParticleEmitter>();
	if (size > 3)
		blockSparks = children[3]->getComponent<ParticleEmitter>();
}

void ParticleManager::update(float deltaTime)
{
	/* FLOOR DUST */
	manageFloorDust();

	/* JUMP DUST */
	manageJumpDust();

	/* BLOOD SPLASH */
	manageBloodSplash();

	/* BLOCK SPARKS */
	manageBlockSparks();
}

void ParticleManager::generateFloorDust()
{
	
}

void ParticleManager::generateJumpDust()
{
	
}

void ParticleManager::manageFloorDust()
{
	if (playerState->isGrounded() && playerState->isMoving())
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
