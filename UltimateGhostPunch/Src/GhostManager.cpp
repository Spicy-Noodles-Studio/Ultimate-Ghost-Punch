#include "GhostManager.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <sstream>

#include "Movement.h"
#include "GhostMovement.h"
#include "Health.h"
#include "MeshRenderer.h"
#include "RigidBody.h"
#include "Respawn.h"
#include "Score.h"
#include "PlayerController.h"
#include "PlayerAnimController.h"
#include "PlayerIndex.h"
#include "PlayerUI.h"
#include "UltimateGhostPunch.h"
#include "Game.h"
#include "GameManager.h"

REGISTER_FACTORY(GhostManager);

GhostManager::GhostManager(GameObject* gameObject) : UserComponent(gameObject), deathPosChanged(false), ended(false), ghost(false), used(false),
													 movement(nullptr), ghostMovement(nullptr), health(nullptr), transform(nullptr), meshRenderer(nullptr), rigidBody(nullptr), game(nullptr), anim(nullptr),
													 resurrectionHealth(2), playerGravity(-10.0f), ghostTime(10.0f), ghostDamage(1), aliveScale(Vector3::ZERO), ghostScale(Vector3::ZERO), deathPosition(Vector3::ZERO), ghostSpawnOffset(Vector3::ZERO),
													 punchSuccess(false)
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
	anim = gameObject->getComponent<PlayerAnimController>();
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
	if (health != nullptr && !health->isAlive() && !ghost && mode == ALIVE)
	{
		if (anim != nullptr) anim->notLoopAnimation("Die");
		mode = DYING;
		// Deactivate controller while player dies
		if (control != nullptr) control->setActive(false);
	}

	if (ghost)
	{
		if (ghostTime > 0)
			ghostTime -= deltaTime;
		else if (!ended && game != nullptr) {
			ended = true;
			ghost = false;
			if (anim != nullptr) anim->notLoopAnimation("Disappear");
			// Deactivate controller
			auto control = gameObject->getComponent<PlayerController>();
			if (control != nullptr)
				control->setActive(false);

			if (movement != nullptr)
				movement->stop();
		}
	}
}

void GhostManager::postUpdate(float deltaTime)
{
	punchSuccess = false;
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
			setVector3(ghostSpawnOffset);
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
	// If this player is in ghost mode and other is a player
	if (ghost && used && other->getTag() == "Player")
	{
		Score* score = GameManager::GetInstance()->getScore();
		Health* aux = other->getComponent<Health>();

		//If the other player is alive
		if (aux != nullptr && aux->isAlive())
		{
			int health = aux->getHealth();
			aux->receiveDamage(ghostDamage);

			auto otherAnim = other->getComponent<PlayerAnimController>();
			if (otherAnim != nullptr)
				otherAnim->hurtAnimation();

			score->lifeStolenBy(other->getComponent<PlayerIndex>()->getIndex(), gameObject->getComponent<PlayerIndex>()->getIndex());

			if (!aux->isAlive())
				score->killedBy(other->getComponent<PlayerIndex>()->getIndex(), gameObject->getComponent<PlayerIndex>()->getIndex());

			if (anim != nullptr)
			{
				auto punch = gameObject->getComponent<UltimateGhostPunch>();
				if (punch != nullptr && punch->isPunching()) //GHOST PUNCH SUCCESS
				{
					anim->punchSuccessAnimation();
					punch->punchSucceeded();
					punchSuccess = true;
				}
				else
					anim->notLoopAnimation("UGPSuccess");
			}

			auto controll = gameObject->getComponent<PlayerController>();
			if (controll != nullptr)
				controll->setActive(false);

			movement->stop();

			score->lifeStolenBy(other->getComponent<PlayerIndex>()->getIndex(), gameObject->getComponent<PlayerIndex>()->getIndex());

			if (!aux->isAlive())
				score->killedBy(other->getComponent<PlayerIndex>()->getIndex(), gameObject->getComponent<PlayerIndex>()->getIndex());
			
			ghost = false;
		}
	}
}

bool GhostManager::isGhost()
{
	return ghost;
}

bool GhostManager::ghostEnded()
{
	return ended;
}

float GhostManager::getGhostTime()
{
	return ghostTime;
}

void GhostManager::activateGhost()
{
	ghost = true;
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
		if (!deathPosChanged)
			deathPosition = transform->getPosition();

		// Change scale
		transform->setScale(ghostScale);

		// Apply position offset
		transform->setPosition(transform->getPosition() + ghostSpawnOffset);
	}

	// Set player colour
	if (meshRenderer != nullptr)
		meshRenderer->setDiffuse(0, playerColour, 1);

	// Reactivate controller
	auto control = gameObject->getComponent<PlayerController>();
	if (control != nullptr) control->setActive(true);
	mode = GHOST;
}

void GhostManager::deactivateGhost()
{
	ghost = false;

	if (movement != nullptr)
		movement->setActive(true);

	if (ghostMovement != nullptr)
		ghostMovement->setActive(false);

	if (rigidBody != nullptr)
	{
		rigidBody->setTrigger(false);
		rigidBody->setGravity({ 0, playerGravity, 0 });
	}

	auto control = gameObject->getComponent<PlayerController>();
	if (control != nullptr) control->setActive(true);

	// Change scale
	if (transform != nullptr)
		transform->setScale(aliveScale);

	//Set the player alive
	if (health != nullptr)
	{
		health->setAlive(true);
		health->setHealth(resurrectionHealth);
	}

	//Respawn the player
	Respawn* respawn = gameObject->getComponent<Respawn>();
	if (respawn != nullptr) respawn->spawn(deathPosition);

	mode = ALIVE;
}

void GhostManager::setDeathPosition(const Vector3& dPos)
{
	deathPosChanged = true;
	deathPosition = dPos;
}

void GhostManager::setPlayerColour(const Vector3& colour)
{
	playerColour = colour;
}

void GhostManager::deactivatePlayer()
{
	if (movement != nullptr) movement->stop();
	if (meshRenderer != nullptr) meshRenderer->setVisible(false);
	if (playerUI != nullptr) playerUI->setVisible(false);
	game->playerDie();
	gameObject->setActive(false);
}

void GhostManager::handlePlayerDeath()
{
	if (!used)
	{
		ghost = true;
		if (anim != nullptr) anim->enterMode(PlayerAnimController::GHOST);
	}
	else
	{
		deactivatePlayer();
	}
}

bool GhostManager::hasPunchSuccess() const
{
	return punchSuccess;
}
