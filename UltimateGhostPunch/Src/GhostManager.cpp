#include "GhostManager.h"
#include <GameObject.h>
#include <sstream>

#include "PlayerController.h"
#include "Movement.h"
#include "GhostMovement.h"
#include "Health.h"
#include "MeshRenderer.h"
#include "RigidBody.h"
#include "PlayerUI.h"
#include "FightManager.h"

#include "ComponentRegister.h"

REGISTER_FACTORY(GhostManager);

GhostManager::GhostManager(GameObject* gameObject) : UserComponent(gameObject)
{

}

GhostManager::~GhostManager()
{

}

void GhostManager::start()
{
	movement = gameObject->getComponent<Movement>();
	ghostMovement = gameObject->getComponent<GhostMovement>();
	health = gameObject->getComponent<Health>();
	transform = gameObject->transform;
	meshRenderer = gameObject->getComponent<MeshRenderer>();
	rigidBody = gameObject->getComponent<RigidBody>();
	playerUI = gameObject->getComponent<PlayerUI>();

	ghost = false;
	used = false;

	// Store some data for player resurrection
	if (meshRenderer != nullptr) aliveMeshId = meshRenderer->getMeshId();
	if (meshRenderer != nullptr) aliveMeshName = meshRenderer->getMeshName();
	if (rigidBody != nullptr) playerGravity = rigidBody->getGravity().y;
	aliveScale = transform->getScale();
}

void GhostManager::update(float deltaTime)
{
	if (health != nullptr && !health->isAlive())
	{
		if (!used && !ghost)
			activateGhost();

		if (used && !ghost)
		{
			if (playerUI != nullptr) playerUI->updateState("Dead");
			findGameObjectWithName("FightManager")->getComponent<FightManager>()->playerDie();
		}
	}

	if (ghost)
	{
		if (ghostTime > 0)
			ghostTime -= deltaTime;
		else
		{
			if (playerUI != nullptr) playerUI->updateState("Dead");
			findGameObjectWithName("FightManager")->getComponent<FightManager>()->playerDie();
		}
	}
}

void GhostManager::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "ghostTime")
		{
			if (!(ss >> ghostTime))
				LOG("GHOST MANAGER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "ghostDamage")
		{
			if (!(ss >> ghostDamage))
				LOG("GHOST MANAGER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "ghostMesh")
		{
			if (!(ss >> ghostMeshId >> ghostMeshName))
				LOG("GHOST MANAGER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "ghostScale")
		{
			double x, y, z;
			if (!(ss >> x >> y >> z))
				LOG("GHOST MANAGER: Invalid property with name \"%s\"", prop.first.c_str());
			else
				ghostScale = { x,y,z };
		}
		else if (prop.first == "spawnOffset")
		{
			double x, y, z;
			if (!(ss >> x >> y >> z))
				LOG("GHOST MANAGER: Invalid property with name \"%s\"", prop.first.c_str());
			else
				ghostSpawnOffset = { x,y,z };
		}
		else if (prop.first == "deathPosition")
		{
			double x, y, z;
			if (!(ss >> x >> y >> z))
				LOG("HEALTH: Invalid property with name \"%s\"", prop.first.c_str());
			else
				deathPosition = { x,y,z };
		}
		else
			LOG("GHOST MANAGER: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void GhostManager::onObjectEnter(GameObject* other)
{
	// If this player is in ghost mode and other is a player
	if (ghost && other->getTag() == "Player")
	{
		Health* aux = other->getComponent<Health>();

		//If the other player is alive
		if (aux != nullptr && aux->isAlive())
		{
			if (health != nullptr) health->setAlive(true);

			deactivateGhost();
			aux->receiveDamage(ghostDamage);
			gameObject->getComponent<PlayerController>()->setFrozen(true);
			gameObject->getComponent<PlayerController>()->respawn(deathPosition);
		}
	}
}

bool GhostManager::isGhost()
{
	return ghost;
}

void GhostManager::activateGhost()
{
	deathPosition = transform->getPosition();

	ghost = true;
	used = true;

	if (movement != nullptr) movement->setActive(false);
	if (ghostMovement != nullptr) ghostMovement->setActive(true);

	if (rigidBody != nullptr)
	{
		rigidBody->setTrigger(true);
		rigidBody->setGravity({ 0,0,0 });
	}

	// Change player's mesh -> ghost mesh
	if (meshRenderer != nullptr) meshRenderer->changeMesh(ghostMeshId, ghostMeshName);

	// Change scale
	transform->setScale(ghostScale);

	// Apply position offset
	transform->setPosition({ transform->getPosition().x + ghostSpawnOffset.x, transform->getPosition().y + ghostSpawnOffset.y, transform->getPosition().z + ghostSpawnOffset.z });

	if (playerUI != nullptr) playerUI->updateState("Ghost");
}

void GhostManager::deactivateGhost()
{
	ghost = false;

	if (movement != nullptr) movement->setActive(true);
	if (ghostMovement != nullptr) ghostMovement->setActive(false);

	if (rigidBody != nullptr)
	{
		rigidBody->setTrigger(false);
		rigidBody->setGravity({ 0, playerGravity, 0 });
	}

	// Change player's mesh -> alive mesh
	gameObject->getComponent<MeshRenderer>()->changeMesh(aliveMeshId, aliveMeshName);

	// Change scale
	gameObject->getComponent<Transform>()->setScale(aliveScale);

	if (playerUI != nullptr) playerUI->updateState("Alive");
}

void GhostManager::setDeathPosition(const Vector3& dPos)
{
	deathPosition = dPos;
	LOG("{%f, %f, %f}\n", dPos.x, dPos.y, dPos.z);
}