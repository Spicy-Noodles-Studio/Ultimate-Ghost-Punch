#include "Health.h"
#include <sstream>
#include "GameObject.h"

#include "GhostManager.h"
#include "UILayout.h"

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

	ghost = gameObject->getComponent<GhostManager>();
}

void Health::update(float deltaTime)
{
	if (invencible)
	{
		if (time > 0.0f)
		{
			time -= deltaTime;
		}
		else
			invencible = false;
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
		else if (prop.first == "invTime") {
			if (!(ss >> invencibleDamageTime))
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

	invencible = true;
	time = invencibleDamageTime;

	findGameObjectWithName("MainCamera")->getComponent<UILayout>()->
		getUIElement("StaticImage").getChild(gameObject->getName() + "HealthText").setText("Health: " + std::to_string(health));

	if (health <= 0)
	{
		if (ghost != nullptr && ghost->hasGhost())
			ghost->activateGhost();
		else
			die();
	}
}

void Health::die()
{
	alive = false;

	// update UI

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

bool Health::isInvencible()
{
	return invencible;
}