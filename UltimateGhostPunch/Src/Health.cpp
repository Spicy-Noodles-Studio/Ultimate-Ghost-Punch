#include "Health.h"

#include <ComponentRegister.h>
#include <GameObject.h>
#include <Camera.h>
#include <Scene.h>
#include <sstream>

#include "Block.h"
#include "Attack.h"
#include "Grab.h"
#include "CameraEffects.h"

REGISTER_FACTORY(Health);

Health::Health(GameObject* gameObject) : UserComponent(gameObject), maxHealth(4), health(4), time(0.0f), invencibleTime(0.5f), alive(true), invencible(false), hurt(false), cameraEffects(nullptr)
{

}

Health::~Health()
{

}

void Health::start()
{
	maxHealth = health;
	GameObject* mainCamera = findGameObjectWithName("MainCamera");
	checkNullAndBreak(mainCamera);

	cameraEffects = mainCamera->getComponent<CameraEffects>();
	checkNull(cameraEffects);
}

void Health::update(float deltaTime)
{
	if (invencible)
	{
		if (time > 0.0f) time -= deltaTime;
		else invencible = false;
	}
}

void Health::postUpdate(float deltaTime)
{
	hurt = false;
}

void Health::handleData(ComponentData* data)
{
	checkNullAndBreak(data);
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "health")
		{
			setInt(health);
		}
		else if (prop.first == "invDamTime")
		{
			setFloat(invencibleTime);
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
		std::vector<GameObject*> aux = gameObject->findChildrenWithTag("groundSensor");
		Block* block = nullptr;

		if (aux.size() > 0)
		{
			block = aux[0]->getComponent<Block>();
			if (notNull(block) && block->isBlocking())
				block->unblock();
		}

		aux = gameObject->findChildrenWithTag("attackSensor");
		Attack* attack = nullptr;

		if (aux.size() > 0)
		{
			attack = aux[0]->getComponent<Attack>();
			if (notNull(attack) && attack->isAttacking())
				attack->stop();
		}

		aux = gameObject->findChildrenWithTag("grabSensor");
		Grab* grab = nullptr;

		if (aux.size() > 0)
		{
			grab = aux[0]->getComponent<Grab>();
			if (notNull(grab) && grab->isGrabbing())
				grab->drop();
		}

		health -= damage;

		if (health < 0)
			health = 0;

		if (health == 0)
		{
			alive = false;
			if (notNull(cameraEffects)) cameraEffects->shake(Vector3(1, 1, 0));
		}
		else
		{
			invencible = true;
			time = invencibleTime;
		}

		hurt = true;
	}
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

bool Health::isHurt() const
{
	return hurt;
}