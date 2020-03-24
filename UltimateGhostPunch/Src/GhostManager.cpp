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
		else if (prop.first == "ghostScale") {
			double x, y, z;
			if (!(ss >> x >> y >> z))
				LOG("HEALTH: Invalid property with name \"%s\"", prop.first.c_str());
			else
				ghostScale = { x,y,z };
		}
		else if (prop.first == "spawnOffset") {
			double x, y, z;
			if (!(ss >> x >> y >> z))
				LOG("HEALTH: Invalid property with name \"%s\"", prop.first.c_str());
			else
				ghostSpawnOffset = { x,y,z };
		}
		else
			LOG("GHOST MANAGER: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void GhostManager::onTriggerEnter(GameObject* other)
{
	if (!ghost	// If this player is in ghost mode
		&& other->getTag() == "player" // and other is a player
		&& health->getHealth() > 0) { // and it is alive
		
		other->getComponent<GhostManager>()->deactivateGhost();
		if (other->getComponent<Health>() != nullptr) other->getComponent<Health>()->resurrect();
		
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
	Transform* t = gameObject->getComponent<Transform>();
	ghost = true;
	ghostAble = false;

	if(mov != nullptr) mov->setActive(false);
	if(gMov != nullptr) gMov->setActive(true);
	if (rb != nullptr) {
		rb->setTrigger(true);
		rb->setGravity({ 0,0,0 });
	}

	aliveMeshId = gameObject->getComponent<MeshRenderer>()->getMeshId();
	aliveMeshName = gameObject->getComponent<MeshRenderer>()->getMeshName();
	aliveScale = t->getScale();
	// Change player's mesh -> ghost mesh
	gameObject->getComponent<MeshRenderer>()->changeMesh(ghostMeshId, ghostMeshName);
	// Change scale
	t->setScale(ghostScale);
	// Apply position offset
	t->setPosition({ t->getPosition().x + ghostSpawnOffset.x, t->getPosition().y + ghostSpawnOffset.y, t->getPosition().z + ghostSpawnOffset.z });
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

	// Change player's mesh -> alive mesh
	gameObject->getComponent<MeshRenderer>()->changeMesh(aliveMeshId, aliveMeshName);
	gameObject->getComponent<Transform>()->setScale(aliveScale);
}

void GhostManager::changeMesh(std::string id, std::string name)
{

}

