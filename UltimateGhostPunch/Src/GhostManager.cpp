#include "GhostManager.h"

#include <GameObject.h>
#include <sstream>

#include "Movement.h"
#include "GhostMovement.h"
#include "Health.h"
#include "RigidBody.h"
#include "PlayerController.h"
#include "PhysicsSystem.h"
#include "MeshRenderer.h"

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

	//aliveMeshId = gameObject->getComponent<MeshRenderer>()->getMesh();
}

void GhostManager::update(float deltaTime)
{
	if (ghost && ghostTime > 0)
		ghostTime -= deltaTime;
	else if (ghost && ghostTime <= 0)
		if(health != nullptr) health->die();

}

void GhostManager::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "ghostTime") {
			if (!(ss >> ghostTime))
				LOG("GHOST MANAGER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "ghostMesh") {
			if (!(ss >> ghostMeshId >> ghostMeshName))
				LOG("HEALTH: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("GHOST MANAGER: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void GhostManager::OnObjectEnter(GameObject* other)
{
	//if (other->getTag() == "player") {
	//	deactivateGhost();
	//	if (health != nullptr) health->resurrect();
	//}
}

void GhostManager::onTriggerEnter(GameObject* other)
{
	//printf("PLAYER %d:\n%s\n\n", gameObject->getComponent<PlayerController>()->getPlayerIndex(), other->getTag().c_str());
	/*if (ghost	// If this player is in ghost mode
		&& other->getTag() == "player" // and other is a player
		&& other->getComponent<Health>()->getHealth() > 0) { // and it is alive
		//printf("\n\nMI VIDA: %d\nSU VIDA: %d\n----FANTASMA RESUCITADO----\n", health->getHealth(), other->getComponent<Health>()->getHealth());
		deactivateGhost();
		if (health != nullptr) health->resurrect();
		//printf("MI VIDA: %d\nSU VIDA: %d\n", health->getHealth(), other->getComponent<Health>()->getHealth());
	}*/

	if (!ghost	// If this player is in ghost mode
		&& other->getTag() == "player" // and other is a player
		&& health->getHealth() > 0) { // and it is alive
		printf("\n\nMI VIDA: %d\nSU VIDA: %d\n----FANTASMA RESUCITADO----\n", health->getHealth(), other->getComponent<Health>()->getHealth());
		other->getComponent<GhostManager>()->deactivateGhost();
		if (other->getComponent<Health>() != nullptr) other->getComponent<Health>()->resurrect();
		printf("MI VIDA: %d\nSU VIDA: %d\n", health->getHealth(), other->getComponent<Health>()->getHealth());
	}
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
	if (rb != nullptr) {
		rb->setTrigger(true);
		rb->setGravity({ 0,0,0 });
	}
}

void GhostManager::deactivateGhost()
{
	ghost = false;

	if (mov != nullptr) mov->setActive(true);
	if (gMov != nullptr) gMov->setActive(false);
	if (rb != nullptr) {
		rb->setTrigger(false);
		rb->setGravity({ 0, PhysicsSystem::GetInstance()->getWorldGravity().y,0 });
	}
}

void GhostManager::changeMesh(std::string id, std::string name)
{

}

