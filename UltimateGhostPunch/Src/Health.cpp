#include "Health.h"
#include <sstream>
#include <GameObject.h>

#include "GhostManager.h"
#include "PlayerUI.h"
#include "PlayerController.h"
#include "ComponentRegister.h"

REGISTER_FACTORY(Health);

Health::Health(GameObject* gameObject) : UserComponent(gameObject)
{

}

Health::~Health()
{

}

void Health::start()
{
	alive = true;
	invencible = false;

	maxHealth = health;

	ghost = gameObject->getComponent<GhostManager>();
	playerUI = gameObject->getComponent<PlayerUI>();
}

void Health::update(float deltaTime)
{
	if (invencible)
	{
		if (time > 0.0f)
			time -= deltaTime;
		else
		{
			invencible = false;
			if(playerUI!=nullptr) playerUI->updateState("Alive");
			if (respawning) 
			{
				respawning = false;
				// reactivate movement
				PlayerController* input = gameObject->getComponent<PlayerController>();
				if (input != nullptr) input->setFrozen(false);
			}
		}
	}
}

void Health::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "health") {
			if (!(ss >> health))
				LOG("HEALTH: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "resHealth") {
			if (!(ss >> resurrectionHealth))
				LOG("HEALTH: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "invDamTime") {
			if (!(ss >> invencibleDamageTime))
				LOG("HEALTH: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "invResTime") {
			if (!(ss >> invencibleResurrectionTime))
				LOG("HEALTH: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("HEALTH: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void Health::receiveDamage(int damage)
{
	if ((ghost != nullptr && ghost->isGhost()) || invencible)
		return;

	health -= damage;
	if (health < 0) health = 0;

	if (playerUI != nullptr) playerUI->updateHealth();

	if (health == 0)
	{
		if (ghost != nullptr && ghost->hasGhost())
		{
			if (playerUI != nullptr) playerUI->updateState("Ghost");
			ghost->activateGhost();
		}
		else
			die();
	}
	else
	{
		invencible = true;
		time = invencibleDamageTime;
		if (playerUI != nullptr) playerUI->updateState("Invencible");
	}
}

void Health::die()
{
	alive = false;

	if (playerUI != nullptr) playerUI->updateState("Dead");

	// deactivate gameObject
	gameObject->setActive(false);

	// kick the player out of the game (?)
	//...
	// save info to show in the winner screen (position of the podium, kills, etc.) (?)
	//...
}

void Health::resurrect()
{
	health = resurrectionHealth;

	// activate invencibility for a specified time
	invencible = true;
	time = invencibleResurrectionTime;
	//update UI
	if (playerUI != nullptr) {
		playerUI->updateHealth();
		playerUI->updateState("Respawning");
	}
	respawning = true;

	// deactivate movement while reapearing
	PlayerController* input = gameObject->getComponent<PlayerController>();
	if (input != nullptr) input->setFrozen(true);
	
	
}

int Health::getHealth()
{
	return health;
}

int Health::getMaxHealth()
{
	return maxHealth;
}

void Health::setHealth(int health)
{
	this->health = health;
}

bool Health::isAlive()
{
	return alive;
}

bool Health::isInvencible()
{
	return invencible;
}