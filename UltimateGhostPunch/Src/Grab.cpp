#include "Grab.h"

#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>

#include "Health.h"

Grab::Grab(GameObject* gameObject) : UserComponent(gameObject)
{
}

void Grab::start()
{
	attackTrigger = gameObject->getComponent<RigidBody>();

	// Deactivate the trigger until the attack is used
	if (attackTrigger != nullptr) attackTrigger->setActive(false);
}

void Grab::update(float deltaTime)
{
	if (state == GRABBING) {
		state = IDLE;
	}

	if (remain > 0.0f) remain -= deltaTime;

	if (remain <= 0.0f) {
		state = IDLE;
		drop();

	}
}







void Grab::onObjectStay(GameObject* other)
{
	if (other->getTag() == "player" && other != gameObject->getParent() && state ==	GRABBING)//If it hits a player different than myself
	{

		gameObject->addChild(other);
		state = GRABBED;
		remain = duration;
		enemy = other;
	}
}

void Grab::onObjectEnter(GameObject* other)
{
	if (other->getTag() == "suelo") {
		isGrounded = true;
	}
}

void Grab::onObjectExit(GameObject* other)
{
	if (other->getTag() == "suelo") {
		isGrounded = false;
	}
}

void Grab::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		
	}
}

void Grab::grab()
{
	if (state == IDLE) state = GRABBING;
}

void Grab::drop()
{
	gameObject->removeChild(enemy);

	//lanzar enemigo

	enemy = nullptr;
}
