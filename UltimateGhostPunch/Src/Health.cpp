#include "Health.h"
#include <sstream>
#include "GameObject.h"

#include "GhostMovement.h"
#include "Movement.h"
#include "PlayerController.h"
#include "Attack.h"

Health::Health(GameObject* gameObject) : UserComponent(gameObject)
{

}

Health::~Health()
{

}

void Health::start()
{
	alive = true;
	ghost = false;
	hasGhost = true;
	invencible = false;
}

void Health::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "health") {
			int health; ss >> health;
			this->health = health;
		}
		else if (prop.first == "resHealth") {
			int resHealth; ss >> resHealth;
			this->resurrectionHealth = resHealth;
		}
		else if (prop.first == "ghostTime") {
			float ghostTime; ss >> ghostTime;
			this->ghostTime = ghostTime;
		}
		else if (prop.first == "invTime") {
			float invTime; ss >> invTime;
			this->invencibleTime = invTime;
		}
		else
		{
			printf("HEALTH: Invalid property name \"%s\"", prop.first.c_str());
		}
	}
}

void Health::onTriggerEnter(GameObject* other)
{
	// seria algo asi??
	//recieveDamage(other->getComponent<Attack>()->quickAttackDamage());

	recieveDamage(10);
}

void Health::recieveDamage(int damage)
{
	if (ghost || invencible)
		return;

	// update UI health

	health -= damage;
	if (health <= 0)
	{
		if (hasGhost)
			activateGhost();
		else
			die();
	}
}

void Health::die()
{
	alive = false;

	// update UI

	// deactivate controllers
	gameObject->getComponent<PlayerController>()->setActive(false);

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
	//invencibleTimer->start() (???)
}

void Health::activateGhost()
{
	hasGhost = false;
	// TODO:
	// movement components
	gameObject->getComponent<Movement>()->setActive(false);
	gameObject->getComponent<GhostMovement>()->setActive(true);

	// render components
	//gameObject->getComponent<RenderComponent>()->changeMesh();
	//gameObject->getComponent<ParticleEmitter>()->stop();

	//...
}

int Health::getHealth()
{
	return health;
}

void Health::setHealth(int health)
{
	this->health = health;
}

bool Health::isAlive()
{
	return alive;
}

bool Health::isGhost()
{
	return ghost;
}

bool Health::isInvencible()
{
	return invencible;
}