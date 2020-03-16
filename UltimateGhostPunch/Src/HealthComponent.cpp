#include "HealthComponent.h"
#include "GameObject.h"

HealthComponent::HealthComponent(GameObject* gameObject) : UserComponent(gameObject)
{

}

HealthComponent::~HealthComponent()
{

}

int HealthComponent::getHealth()
{
	return health;
}

void HealthComponent::setHealth(int health)
{
	this->health = health;
}

bool HealthComponent::isAlive()
{
	return alive;
}

bool HealthComponent::isGhost()
{
	return ghost;
}

bool HealthComponent::isInvencible()
{
	return invencible;
}

void HealthComponent::recieveDamage(int damage)
{
	health -= damage;
	if (health <= 0)
	{

		if (hasGhost)
			activateGhost();
		else
			alive = false;
	}
}

void HealthComponent::die()
{
}

void HealthComponent::resurrect()
{
	health = resurrectionHealth;
}

void HealthComponent::activateGhost()
{
	
	// enters ghost mode here:
	// TODO:

	hasGhost = false;

	// change components
	//gameObject->getComponent<KnightMovement>()->setActive(false);
	//gameObject->getComponent<GhostMovement>()->setActive(true);
	//...

	// activate invencibility for a specified time

}
