#include "Health.h"
#include <sstream>
#include <GameObject.h>

#include "GhostManager.h"
#include "PlayerUI.h"
#include "PlayerController.h"
#include "ComponentRegister.h"

#include "FightManager.h"

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

	ghostManager = gameObject->getComponent<GhostManager>();
	//playerUI = gameObject->getComponent<PlayerUI>();
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
			// TODO: quitar esto, aqui sobra
			//if(playerUI!=nullptr) playerUI->updateState("Alive");
			if (respawning) 
			{
				respawning = false;
				// reactivate movement
			// TODO: quitar esto, aqui sobra
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
	// TODO: esto esta feo
	if ((ghostManager != nullptr && ghostManager->isGhost()) || invencible)
		return;

	health -= damage;
	if (health < 0) health = 0;

	// TODO: quitar esto, aqui sobra
	//if (playerUI != nullptr) playerUI->updateHealth();

	// TODO: esto se debe hacer desde fuera
	if (health == 0)
	{
		if (ghostManager != nullptr && ghostManager->hasGhost())
		{
			// TODO: quitar esto, aqui sobra
			//if (playerUI != nullptr) playerUI->updateState("Ghost");
			ghostManager->activateGhost();
		}
		else
			die();
	}
	else
	{
		invencible = true;
		time = invencibleDamageTime;
		// TODO: quitar esto, aqui sobra
		//if (playerUI != nullptr) playerUI->updateState("Invencible");
	}
}

void Health::die()
{
	alive = false;

	// TODO: quitar esto, aqui sobra
	//if (playerUI != nullptr) playerUI->updateState("Dead");

	findGameObjectWithName("FightManager")->getComponent<FightManager>()->playerDie();

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
	// TODO: quitar esto, aqui sobra
	//update UI
	/*if (playerUI != nullptr) {
		playerUI->updateHealth();
		playerUI->updateState("Respawning");
	}*/
	respawning = true;

	// TODO: esto no es trabajo de HEALTH
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