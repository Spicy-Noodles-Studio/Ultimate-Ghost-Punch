#include "GhostManager.h"

#include <GameObject.h>
#include "Movement.h"
#include "GhostMovement.h"
#include "Health.h"
#include "RigidBody.h"

GhostManager::GhostManager(GameObject* gameObject) : UserComponent(gameObject)
{

}

void GhostManager::start()
{
	mov = gameObject->getComponent<Movement>();
	gMov = gameObject->getComponent<GhostMovement>();
	health = gameObject->getComponent<Health>();
	rb = gameObject->getComponent<RigidBody>();

	ghost = false;
	ghostAble = true;
}

void GhostManager::update(float deltaTime)
{
	if (ghost && ghostTime > 0)
		ghostTime -= deltaTime;
	else if (ghost && ghostTime <= 0)
		if(health != nullptr) health->die();
}

bool GhostManager::isGhost()
{
	return ghost;
}

bool GhostManager::hasGhost()
{
	return ghostAble;
}

void GhostManager::activateGhost()
{
	ghost = true;
	ghostAble = false;

	if(mov != nullptr) mov->setActive(false);
	if(gMov != nullptr) gMov->setActive(true);
	if(rb != nullptr) rb->setTrigger(true);
}

void GhostManager::deactivateGhost()
{
	ghost = false;

	if (mov != nullptr) mov->setActive(true);
	if (gMov != nullptr) gMov->setActive(false);
	if (rb != nullptr) rb->setTrigger(false);
}

