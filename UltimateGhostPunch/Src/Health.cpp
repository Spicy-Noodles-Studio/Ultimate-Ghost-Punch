#include "Health.h"
#include <GameObject.h>
#include <sstream>

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
	maxHealth = health;
	time = 0.0f;

	alive = true;
	invencible = false;
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
			if (!(ss >> health))
				LOG("HEALTH: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "resHealth")
		{
			if (!(ss >> resurrectionHealth))
				LOG("HEALTH: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "invDamTime")
		{
			if (!(ss >> invencibleDamageTime))
				LOG("HEALTH: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "invResTime")
		{
			if (!(ss >> invencibleResurrectionTime))
				LOG("HEALTH: Invalid property with name \"%s\"", prop.first.c_str());
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

int Health::getResurrectionHealth()
{
	return resurrectionHealth;
}

void Health::setHealth(int health)
{
	this->health = health;
}

void Health::receiveDamage(int damage)
{
	if (alive && !invencible)
	{
		health -= damage;
		if (health < 0) health = 0;

		if (health == 0)
			alive = false;
		else
		{
			invencible = true;
			time = invencibleDamageTime;
		}
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

float Health::getInvResTime()
{
	return invencibleResurrectionTime;
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