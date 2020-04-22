#include "GhostManager.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <sstream>

#include "Respawn.h"
#include "Movement.h"
#include "GhostMovement.h"
#include "Health.h"
#include "MeshRenderer.h"
#include "RigidBody.h"
#include "PlayerUI.h"
#include "FightManager.h"
#include "GameManager.h"
#include "Score.h"
#include "PlayerController.h"
#include "PlayerAnimController.h"
#include "UltimateGhostPunch.h"
#include "PlayerIndex.h"
REGISTER_FACTORY(GhostManager);

GhostManager::GhostManager(GameObject* gameObject) : UserComponent(gameObject),deathPosChanged(false), ghost(false), used(false), movement(nullptr), ghostMovement(nullptr), health(nullptr),
													 transform(nullptr), meshRenderer(nullptr), rigidBody(nullptr), fightManager(nullptr), resurrectionHealth(2), playerGravity(-10.0f), ghostTime(10.0f), ghostDamage(1), aliveScale(Vector3()), ghostScale(Vector3()), 
												     deathPosition(Vector3()), ghostSpawnOffset(Vector3()), anim(nullptr)
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

	GameObject* aux = findGameObjectWithName("FightManager");
	if (aux != nullptr) fightManager = aux->getComponent<FightManager>();

	// Store some data for player resurrection
	if (rigidBody != nullptr) playerGravity = rigidBody->getGravity().y;
	if (transform != nullptr) aliveScale = transform->getScale();
}

void GhostManager::update(float deltaTime)
{
	if (health != nullptr && !health->isAlive())
	{
		if (!used && !ghost)
		{
			if (anim != nullptr) anim->notLoopAnimation("Die");
			ghost = true;
			// Deactivate controller while player dies
			auto control = gameObject->getComponent<PlayerController>();
			if (control != nullptr) control->setActive(false);
		}
		else if (used && !ghost && fightManager != nullptr) {
			fightManager->playerDie();
			gameObject->setActive(false);
		}
	}

	if (ghost)
	{
		if (ghostTime > 0)
			ghostTime -= deltaTime;
		else if (!ended && fightManager != nullptr) {
			fightManager->playerDie();
			ended = true;
			if (anim != nullptr) anim->notLoopAnimation("Disappear");
			// Deactivate controller
			auto control = gameObject->getComponent<PlayerController>();
			if (control != nullptr) control->setActive(false);
			if (movement != nullptr) movement->stop();
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
				if (punch != nullptr && punch->isPunching())	//GHOST PUNCH SUCCESS
				{
					anim->punchSuccessAnimation();
					punch->punchSucceeded();
				}
				else
					anim->notLoopAnimation("UGPSuccess");
			}

			auto controll = gameObject->getComponent<PlayerController>();
			if (controll != nullptr) controll->setActive(false);

			movement->stop();
			
			score->lifeStolenBy(other->getComponent<PlayerIndex>()->getIndex(), gameObject->getComponent<PlayerIndex>()->getIndex());
			if (!aux->isAlive())
				score->killedBy(other->getComponent<PlayerIndex>()->getIndex(), gameObject->getComponent<PlayerIndex>()->getIndex());
			
		}
	}
}

bool GhostManager::isGhost()
{
	return ghost;
}

float GhostManager::getGhostTime()
{
	return ghostTime;
}

void GhostManager::activateGhost()
{
	ghost = true;
	used = true;

	if (movement != nullptr) movement->setActive(false);
	if (ghostMovement != nullptr) ghostMovement->setActive(true);

	if (rigidBody != nullptr){
		rigidBody->setTrigger(true);
		rigidBody->setGravity({ 0,0,0 });
	}

	if (transform != nullptr) {
		if (!deathPosChanged) deathPosition = transform->getPosition();
		
		// Change scale
		transform->setScale(ghostScale);

		// Apply position offset
		transform->setPosition(transform->getPosition() + ghostSpawnOffset);
	}

	// Reactivate controller
	auto control = gameObject->getComponent<PlayerController>();
	if (control != nullptr) control->setActive(true);
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

	auto controll = gameObject->getComponent<PlayerController>();
	if (controll != nullptr) controll->setActive(true);

	// Change scale
	if(transform!= nullptr) transform->setScale(aliveScale);

	//Set the player alive
	if (health != nullptr) {
		health->setAlive(true);
		health->setHealth(resurrectionHealth);
	}

	//Respawn the player
	Respawn* respawn = gameObject->getComponent<Respawn>();
	if (respawn != nullptr) respawn->spawn(deathPosition);
}

void GhostManager::setDeathPosition(const Vector3& dPos)
{
	deathPosChanged = true;
	deathPosition = dPos;
}

bool GhostManager::ghostEnded()
{
	return ended;
}
