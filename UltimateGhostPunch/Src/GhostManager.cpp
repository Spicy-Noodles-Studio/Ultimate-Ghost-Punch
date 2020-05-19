#include "GhostManager.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <MeshRenderer.h>
#include <RigidBody.h>
#include <sstream>

#include "Movement.h"
#include "GhostMovement.h"
#include "Health.h"
#include "Respawn.h"
#include "Score.h"
#include "UltimateGhostPunch.h"
#include "PlayerController.h"
#include "PlayerIndex.h"
#include "PlayerUI.h"
#include "Game.h"
#include "GameManager.h"

REGISTER_FACTORY(GhostManager);

GhostManager::GhostManager(GameObject* gameObject) : UserComponent(gameObject), used(false), success(false), positionChanged(false),
game(nullptr), transform(nullptr), meshRenderer(nullptr), rigidBody(nullptr), movement(nullptr), ghostMovement(nullptr), health(nullptr), playerUI(nullptr), control(nullptr),
aliveScale(Vector3::ZERO), ghostScale(Vector3::ZERO), playerColour(Vector3::ZERO), deathPosition(Vector3::ZERO), spawnOffset(Vector3::ZERO), 
resurrectTime(2.0f), dyingTime(1.0f), appearTime(1.0f), disappearTime(0.8f), ghostTime(10), playerGravity(-10), ghostDamage(1), resurrectionHealth(2), mode(ALIVE)
{

}

GhostManager::~GhostManager()
{

}

void GhostManager::start()
{
	transform = gameObject->transform;
	meshRenderer = gameObject->getComponent<MeshRenderer>();
	rigidBody = gameObject->getComponent<RigidBody>();

	movement = gameObject->getComponent<Movement>();
	ghostMovement = gameObject->getComponent<GhostMovement>();
	health = gameObject->getComponent<Health>();

	playerUI = gameObject->getComponent<PlayerUI>();
	control = gameObject->getComponent<PlayerController>();

	GameObject* aux = findGameObjectWithName("Game");
	if (aux != nullptr) game = aux->getComponent<Game>();

	// Store some data for player resurrection
	if (rigidBody != nullptr)
		playerGravity = rigidBody->getGravity().y;

	if (transform != nullptr)
		aliveScale = transform->getScale();
}

void GhostManager::update(float deltaTime)
{
	GhostMode prev = mode;
	handleStates(deltaTime);

	// Has changed
	if (mode != prev) {
		if (mode == APPEAR)
			activateGhost();
		else if (mode == RESURRECT)
			deactivateGhost();
		else if (mode == DEAD)
			deactivatePlayer();
	}
}

void GhostManager::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "ghostTime")
		{
			setFloat(ghostTime);
		}
		else if (prop.first == "ghostDamage")
		{
			setInt(ghostDamage);
		}
		else if (prop.first == "resurrectionHealth")
		{
			setInt(resurrectionHealth);
		}
		else if (prop.first == "ghostScale")
		{
			setVector3(ghostScale);
		}
		else if (prop.first == "spawnOffset")
		{
			setVector3(spawnOffset);
		}
		else if (prop.first == "deathPosition")
		{
			setVector3(deathPosition);
		}
		else
			LOG("GHOST MANAGER: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void GhostManager::onObjectEnter(GameObject* other)
{
	// If is in ghost mode and other is a player
	if (mode == GHOST && other->getTag() == "Player")
	{
		Health* otherHealth = other->getComponent<Health>();
		if (otherHealth != nullptr && otherHealth->isAlive())
		{
			otherHealth->receiveDamage(ghostDamage);

			Score* score = GameManager::GetInstance()->getScore();
			if (score != nullptr)
			{
				PlayerIndex* playerIndex = gameObject->getComponent<PlayerIndex>();
				PlayerIndex* otherIndex = other->getComponent<PlayerIndex>();
				if (playerIndex != nullptr && otherIndex != nullptr)
				{
					score->lifeStolenBy(otherIndex->getIndex(), playerIndex->getIndex());

					if (!otherHealth->isAlive())
						score->killedBy(otherIndex->getIndex(), playerIndex->getIndex());
				}
			}

			UltimateGhostPunch* punch = gameObject->getComponent<UltimateGhostPunch>();
			if (punch != nullptr && punch->isPunching())
				punch->punchSucceeded();

			mode = DISAPPEAR;
			success = true;
		}
	}
}

bool GhostManager::isGhost() const
{
	return mode == GHOST;
}

bool GhostManager::ghostUsed() const
{
	return used;
}

bool GhostManager::ghostSuccess() const
{
	return success;
}

bool GhostManager::ghostDeath() const
{
	return mode == DEAD;
}

float GhostManager::getGhostTime() const
{
	return ghostTime;
}

void GhostManager::setPlayerColour(const Vector3& colour)
{
	playerColour = colour;
}

void GhostManager::setDeathPosition(const Vector3& position)
{
	positionChanged = true;
	deathPosition = position;
}

void GhostManager::activateGhost()
{
	used = true;

	if (movement != nullptr)
		movement->setActive(false);

	if (ghostMovement != nullptr)
		ghostMovement->setActive(true);

	if (rigidBody != nullptr)
	{
		rigidBody->setTrigger(true);
		rigidBody->setGravity({ 0,0,0 });
	}

	if (transform != nullptr)
	{
		if (!positionChanged)
			deathPosition = transform->getPosition();

		// Change scale and position
		transform->setScale(ghostScale);
		transform->setPosition(deathPosition + spawnOffset);
	}

	// Set player colour
	if (meshRenderer != nullptr) {
		meshRenderer->changeMesh("ghost", "Ghost.mesh");
		meshRenderer->setDiffuse(0, playerColour, 1);
	}
}

void GhostManager::deactivateGhost()
{
	if (movement != nullptr)
		movement->setActive(true);

	if (ghostMovement != nullptr) 
		ghostMovement->setActive(false);

	if (rigidBody != nullptr)
	{
		rigidBody->setTrigger(false);
		rigidBody->setGravity({ 0, playerGravity, 0 });
	}

	// Change scale
	if (transform != nullptr)
		transform->setScale(aliveScale);

	//Set the player alive
	if (health != nullptr)
	{
		health->setAlive(true);
		health->setHealth(resurrectionHealth);
	}

	if (meshRenderer != nullptr)
		meshRenderer->changeMesh("player", "Knight.mesh");

	//Respawn the player
	Respawn* respawn = gameObject->getComponent<Respawn>();
	if (respawn != nullptr)
		respawn->spawn(deathPosition);
}

void GhostManager::deactivatePlayer()
{
	if (meshRenderer != nullptr)
		meshRenderer->setVisible(false);

	if (playerUI != nullptr)
		playerUI->setVisible(false);

	PlayerIndex* playerIndex = gameObject->getComponent<PlayerIndex>();

	if (playerIndex != nullptr)
		game->playerDie(playerIndex->getIndex());

	gameObject->setActive(false);
}

bool GhostManager::isResurrecting() const
{
	return mode == RESURRECT;
}

bool GhostManager::isDying() const
{
	return mode == DYING;
}

bool GhostManager::isAppearing() const
{
	return mode == APPEAR;
}

bool GhostManager::isDisappearing() const
{
	return mode == DISAPPEAR;
}

void GhostManager::handleStates(float deltaTime)
{
	if (health != nullptr && !health->isAlive() && mode == ALIVE)
	{
		if (!used)
		{
			mode = DYING;
		}
		else
		{
			mode = DEAD;
		}
	}

	if (mode == RESURRECT)
	{
		if (resurrectTime > 0)
			resurrectTime -= deltaTime;
		else
			mode = ALIVE;
	}
	else if (mode == DYING)
	{
		if (dyingTime > 0)
			dyingTime -= deltaTime;
		else {
			mode = used ? DEAD : APPEAR;
			dyingTime = 1.0f;
		}
	}
	else if (mode == APPEAR)
	{
		if (appearTime > 0)
			appearTime -= deltaTime;
		else
			mode = GHOST;
	}
	else if (mode == DISAPPEAR)
	{
		if (disappearTime > 0)
			disappearTime -= deltaTime;
		else
			mode = success ? RESURRECT : DEAD;
	}

	if (mode == GHOST)
	{
		if (ghostTime > 0)
			ghostTime -= deltaTime;
		else
		{
			mode = DEAD;
		}
	}

	bool controllerFreezed = mode == RESURRECT || mode == DYING || mode == APPEAR || mode == DISAPPEAR || mode == DEAD;
	if (controllerFreezed)
	{
		if (control != nullptr)	
			control->setActive(false);
		
		if (ghostMovement != nullptr)
			ghostMovement->stop();

		if (movement != nullptr)
			movement->stop();
	}
	else {
		if (control != nullptr)
			control->setActive(true);
	}

}
