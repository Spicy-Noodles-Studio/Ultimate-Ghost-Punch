#include "Health.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <sstream>

#include "PlayerAnimController.h"
#include "Grab.h"

REGISTER_FACTORY(Health);

Health::Health(GameObject* gameObject) : UserComponent(gameObject), maxHealth(4), health(4), time(0.0f), invencibleDamageTime(0.5f), alive(true), invencible(false)
{

}

Health::~Health()
{

}

void Health::start()
{
	maxHealth = health;
}

void Health::update(float deltaTime)
{
	if (invencible)
	{
		if (time > 0.0f)
			time -= deltaTime;
		else
			invencible = false;
	}
}

void Health::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "health")
		{
			setInt(health);
		}
		else if (prop.first == "invDamTime")
		{
			setFloat(invencibleDamageTime);
		}
		else
			LOG("HEALTH: Invalid property name \"%s\"", prop.first.c_str());
	}
}

int Health::getMaxHealth()
{
	return maxHealth;
}

int Health::getHealth()
{
	return health;
}

void Health::setHealth(int health)
{
	this->health = health;
}

void Health::receiveDamage(int damage)
{
	if (alive && !invencible)
	{
		std::vector<GameObject*> aux = gameObject->findChildrenWithTag("grabSensor");
		Grab* grab = nullptr;

		if (aux.size() > 0)
		{
			grab = aux[0]->getComponent<Grab>();

			if (grab != nullptr && grab->isGrabbing())
				grab->drop();
		}

		health -= damage;

		if (health < 0)
			health = 0;

		if (health == 0)
			alive = false;
		else
		{
			invencible = true;
			time = invencibleDamageTime;
		}

		gameObject->getComponent<PlayerAnimController>()->hurtAnimation();
	}
}

float Health::getTime()
{
	return time;
}

float Health::getInvDamTime()
{
	return invencibleDamageTime;
}

void Health::setTime(float time)
{
	this->time = time;
}

bool Health::isAlive()
{
	return alive;
}

void Health::setAlive(bool alive)
{
	this->alive = alive;
}

bool Health::isInvencible()
{
	return invencible;
}

void Health::setInvencible(bool invencible)
{
	this->invencible = invencible;
}