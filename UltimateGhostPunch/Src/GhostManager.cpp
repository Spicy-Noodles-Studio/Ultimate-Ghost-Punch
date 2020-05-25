#include "GhostManager.h"

#include <ComponentRegister.h>
#include <GameObject.h>
#include <MeshRenderer.h>
#include <RigidBody.h>
#include <Camera.h>
#include <sstream>

#include "Movement.h"
#include "GhostMovement.h"
#include "Health.h"
#include "Respawn.h"
#include "Score.h"
#include "UltimateGhostPunch.h"
#include "PlayerState.h"
#include "PlayerIndex.h"
#include "PlayerUI.h"
#include "Game.h"
#include "GameManager.h"
#include "ParticleManager.h"
#include "SoundManager.h"
#include "TrailManager.h"
#include "CameraEffects.h"

REGISTER_FACTORY(GhostManager);

GhostManager::GhostManager(GameObject* gameObject) : UserComponent(gameObject), used(false), success(false), positionChanged(false), cam(nullptr), cameraEffects(nullptr),
game(nullptr), transform(nullptr), meshRenderer(nullptr), rigidBody(nullptr), movement(nullptr), ghostMovement(nullptr), health(nullptr), playerUI(nullptr), playerState(nullptr),
aliveScale(Vector3::ZERO), ghostScale(Vector3::ZERO), playerColour(Vector3::ZERO), deathPosition(Vector3::ZERO), spawnOffset(Vector3::ZERO),
resurrectTime(2.0f), dyingTime(1.0f), appearTime(1.0f), disappearTime(0.8f), ghostTime(10), playerGravity(-10), ghostDamage(1), resurrectionHealth(2), mode(ALIVE)
{
}

GhostManager::~GhostManager()
{
	cam = nullptr;
	game = nullptr;
	transform = nullptr;
	cameraEffects = nullptr;
	meshRenderer = nullptr;
	rigidBody = nullptr;
	cam = nullptr;
	movement = nullptr;
	ghostMovement = nullptr;
	health = nullptr;
	playerUI = nullptr;
	playerState = nullptr;
}

void GhostManager::start()
{
	checkNullAndBreak(gameObject);

	transform = gameObject->transform;
	meshRenderer = gameObject->getComponent<MeshRenderer>();
	rigidBody = gameObject->getComponent<RigidBody>();

	movement = gameObject->getComponent<Movement>();
	ghostMovement = gameObject->getComponent<GhostMovement>();
	health = gameObject->getComponent<Health>();

	playerUI = gameObject->getComponent<PlayerUI>();
	playerState = gameObject->getComponent<PlayerState>();

	checkNull(transform);
	checkNull(meshRenderer);
	checkNull(rigidBody);

	checkNull(movement);
	checkNull(ghostMovement);
	checkNull(health);

	checkNull(playerUI);
	checkNull(playerState);

	GameObject* aux = findGameObjectWithName("Game");
	if (notNull(aux))
	{
		game = aux->getComponent<Game>();
		if (notNull(game))
			cameraEffects = game->getCameraEffects();
	}
	checkNull(cameraEffects);

	// Store some data for player resurrection
	if (notNull(rigidBody)) playerGravity = rigidBody->getGravity().y;
	if (notNull(transform)) aliveScale = transform->getScale();
}

void GhostManager::update(float deltaTime)
{
	GhostMode prev = mode;
	handleStates(deltaTime);

	// Has changed
	if (mode != prev)
	{
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
	checkNullAndBreak(data);
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

void GhostManager::onObjectStay(GameObject* other)
{
	checkNullAndBreak(gameObject);

	// If is in ghost mode and other is a player
	if (mode == GHOST && notNull(other) && other->getTag() == "Player")
	{
		Health* otherHealth = other->getComponent<Health>();
		if (notNull(otherHealth) && otherHealth->isAlive() && !otherHealth->isInvencible())
		{
			otherHealth->receiveDamage(ghostDamage);


			Score* score = notNull(GameManager::GetInstance()) ? GameManager::GetInstance()->getScore() : nullptr;
			if (notNull(score))
			{
				PlayerIndex* playerIndex = gameObject->getComponent<PlayerIndex>();
				PlayerIndex* otherIndex = other->getComponent<PlayerIndex>();
				if (notNull(playerIndex) && notNull(otherIndex))
				{
					score->lifeStolenBy(otherIndex->getIndex(), playerIndex->getIndex());

					if (!otherHealth->isAlive())
						score->killedBy(otherIndex->getIndex(), playerIndex->getIndex());
				}
			}

			UltimateGhostPunch* punch = gameObject->getComponent<UltimateGhostPunch>();
			if (notNull(punch) && punch->isPunching())
			{
				punch->punchSucceeded();
				if (notNull(cameraEffects)) cameraEffects->shake(Vector3(1, 1, 0));
			}

			mode = DISAPPEAR;
			success = true;
		}
	}
}

bool GhostManager::isGhost() const
{
	return mode == GHOST;
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

bool GhostManager::ghostUsed() const
{
	return used;
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

	if (notNull(movement))
		movement->setActive(false);

	if (notNull(ghostMovement))
		ghostMovement->setActive(true);

	if (notNull(rigidBody))
	{
		rigidBody->setTrigger(true);
		rigidBody->setGravity({ 0, 0, 0 });
	}

	if (notNull(transform))
	{
		if (!positionChanged) deathPosition = transform->getPosition();

		// Change scale and position
		transform->setScale(ghostScale);
		transform->setPosition(deathPosition + spawnOffset);
	}

	// Set player colour
	if (notNull(meshRenderer))
	{
		meshRenderer->detachEntityFromBone("player", "sword");
		meshRenderer->changeMesh("ghost", "Ghost.mesh");
		meshRenderer->setDiffuse(0, playerColour, 1);
	}
}

void GhostManager::deactivateGhost()
{
	if (notNull(movement))
		movement->setActive(true);

	if (notNull(ghostMovement))
		ghostMovement->setActive(false);

	if (notNull(rigidBody))
	{
		rigidBody->setTrigger(false);
		rigidBody->setGravity({ 0, playerGravity, 0 });
	}

	// Change scale
	if (notNull(transform)) {
		transform->setScale(aliveScale);
		transform->setRotation(0, transform->getRotation().y, 0);
	}

	//Set the player alive
	if (notNull(health))
	{
		health->setAlive(true);
		health->setHealth(resurrectionHealth);
	}

	if (notNull(meshRenderer))
	{
		meshRenderer->changeMesh("player", "Knight.mesh");
		meshRenderer->attachEntityToBone("player", "Mano.L", "sword");
	}

	//Respawn the player
	checkNullAndBreak(gameObject);
	Respawn* respawn = gameObject->getComponent<Respawn>();
	if (notNull(respawn))
		respawn->spawn(deathPosition);
}

void GhostManager::deactivatePlayer()
{
	if (notNull(meshRenderer))
		meshRenderer->setVisible(false);

	if (notNull(playerUI))
	{
		playerUI->setVisible(false);
		playerUI->updateHearts();
	}

	checkNullAndBreak(gameObject);

	ParticleManager* particleManager = gameObject->getComponent<ParticleManager>();

	if (notNull(particleManager))
		particleManager->stopAll();

	SoundManager* soundManager = gameObject->getComponent<SoundManager>();

	if (notNull(soundManager))
		soundManager->stopAll();

	TrailManager* trailManager = gameObject->getComponent<TrailManager>();

	if (notNull(trailManager))
		trailManager->stopAll();

	if (notNull(game))
		game->playerDeath();

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

bool GhostManager::isDead() const
{
	return mode == DEAD;
}

void GhostManager::handleStates(float deltaTime)
{
	GhostMode aux = mode;
	if (health != nullptr && !health->isAlive() && mode == ALIVE)
	{
		if (!used)
			mode = DYING;
		else
			mode = DEAD;
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
		else
		{
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
			mode = DEAD;
	}

	bool controllerFreezed = mode == RESURRECT || mode == DYING || mode == APPEAR || mode == DISAPPEAR || mode == DEAD;
	if (controllerFreezed)
	{
		if (playerState != nullptr)
			playerState->setIgnoringInput(true);

		if (ghostMovement != nullptr)
			ghostMovement->stop();

		if (movement != nullptr)
			movement->stop();
	}
	else if (mode != aux)
	{
		if (playerState != nullptr)
			playerState->setIgnoringInput(false);
	}
}