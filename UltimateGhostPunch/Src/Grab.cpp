#include "Grab.h"

#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>


#include "ComponentRegister.h"

REGISTER_FACTORY(Grab);

Grab::Grab(GameObject* gameObject) : UserComponent(gameObject)
{
}

void Grab::start()
{
	

	attackTrigger = gameObject->getComponent<RigidBody>();

	// Deactivate the trigger until the attack is used
	//if (attackTrigger != nullptr) attackTrigger->setActive(false);
}

void Grab::update(float deltaTime)
{
	if (state == GRABBING) {
		//state = IDLE;
	}

	if (remain > 0.0f) remain -= deltaTime;

	if (state == GRABBED) {
		enemy->transform->setPosition(gameObject->getParent()->transform->getPosition() + enemyDiff);
	}

	if (remain <= 0.0f && state == GRABBED) {
		state = IDLE;
		drop();

	}
}







void Grab::onObjectStay(GameObject* other)
{
	if (state == GRABBING) {
		if (other->getTag() == "Player" && other != gameObject->getParent())//If it hits a player different than myself
		{
			enemyDiff = other->transform->getPosition() - gameObject->getParent()->transform->getPosition();
			
			state = GRABBED;
			remain = duration;
			enemy = other;

		}
	}
}

void Grab::onObjectEnter(GameObject* other)
{
	
}

void Grab::onObjectExit(GameObject* other)
{
	
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
	
	//lanzar enemigo
	enemy->getComponent<RigidBody>()->addForce(vDer * force);

	

	enemy = nullptr;
}
