#include "Health.h"
#include "GameObject.h"

#include "GhostMovement.h"
#include "Movement.h"
#include "PlayerController.h"

Health::Health(GameObject* gameObject) : UserComponent(gameObject)
{

}

Health::~Health()
{

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

void Health::recieveDamage(int damage)
{
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

	// deactivate controllers
	gameObject->getComponent<PlayerController>()->setActive(false);
}

void Health::resurrect()
{
	health = resurrectionHealth;
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

	// activate invencibility for a specified time

}
