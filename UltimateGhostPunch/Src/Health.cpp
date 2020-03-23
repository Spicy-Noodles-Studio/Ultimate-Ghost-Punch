#include "Health.h"
#include <sstream>
#include "GameObject.h"

#include "GhostManager.h"
#include "UILayout.h"

#include "Scene.h"
#include "Camera.h"

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

	findGameObjectWithName("MainCamera")->getComponent<UILayout>()->
		getUIElement("StaticImage").getChild(gameObject->getName() + "HealthText").setText("Health: " + std::to_string(health));

	// ------hearts creation

	float posX = 0.4f;

	for (int i = 1; i <= health; i++)
	{
		findGameObjectWithName("MainCamera")->getComponent<UILayout>()->
			getUIElement("StaticImage").getChild(gameObject->getName() + "Background").
			createChild("TaharezLook/StaticImage", gameObject->getName() + "Heart" + std::to_string(i));

		findGameObjectWithName("MainCamera")->getComponent<UILayout>()->
			getUIElement("StaticImage").getChild(gameObject->getName() + "Background").
			getChild(gameObject->getName() + "Heart" + std::to_string(i)).setPosition(posX, 0.05f);

		findGameObjectWithName("MainCamera")->getComponent<UILayout>()->
			getUIElement("StaticImage").getChild(gameObject->getName() + "Background").
			getChild(gameObject->getName() + "Heart" + std::to_string(i)).setSize(0.05f, 0.2f);

		posX += 0.05f;
	}

	// ------
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
		{
			invencible = false;

			findGameObjectWithName("MainCamera")->getComponent<UILayout>()->
				getUIElement("StaticImage").getChild(gameObject->getName() + "StateText").setText("State: Alive");
		}
	}

	// ----update cursor

	Vector3 pos = gameObject->getScene()->getMainCamera()->worldToScreen(gameObject->getComponent<Transform>()->getPosition());

	findGameObjectWithName("MainCamera")->getComponent<UILayout>()->
		getUIElement("StaticImage").getChild(gameObject->getName() + "Cursor").setPosition((float)pos.x - 0.005f, (float)pos.y - 0.4f);



	// ----

	// ----update hearts

	for (int i = health + 1; i <= maxHealth; i++)
	{
		findGameObjectWithName("MainCamera")->getComponent<UILayout>()->
			getUIElement("StaticImage").getChild(gameObject->getName() + "Background").
			getChild(gameObject->getName() + "Heart" + std::to_string(i)).setVisible(false);

	}


	// ----

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
		{
			ghost->activateGhost();
			findGameObjectWithName("MainCamera")->getComponent<UILayout>()->
				getUIElement("StaticImage").getChild(gameObject->getName() + "StateText").setText("State: Ghost");

		}
		else
		{
			die();

		}
	}
}

void Health::die()
{
	alive = false;

	// update UI
	findGameObjectWithName("MainCamera")->getComponent<UILayout>()->
		getUIElement("StaticImage").getChild(gameObject->getName() + "StateText").setText("State: Dead");

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

	findGameObjectWithName("MainCamera")->getComponent<UILayout>()->
		getUIElement("StaticImage").getChild(gameObject->getName() + "StateText").setText("State: Invencible");
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