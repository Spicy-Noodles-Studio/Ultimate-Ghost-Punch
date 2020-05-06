#include "ParticleManager.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <ParticleEmitter.h>

#include "PlayerState.h"

REGISTER_FACTORY(ParticleManager);

ParticleManager::ParticleManager(GameObject* gameObject) : UserComponent(gameObject), floorDust(nullptr), jumpDust(nullptr)
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
	if(size > 0)
		floorDust = children[0]->getComponent<ParticleEmitter>();
	if (size > 1)
		jumpDust = children[1]->getComponent<ParticleEmitter>();
}

void ParticleManager::update(float deltaTime)
{
	/* FLOOR DUST */
	manageFloorDust();

	/* JUMP DUST */
	manageJumpDust();
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
	if (playerState->isGrounded() && playerState->isJumping())
		jumpDust->start();
	else
		jumpDust->stop();
}
