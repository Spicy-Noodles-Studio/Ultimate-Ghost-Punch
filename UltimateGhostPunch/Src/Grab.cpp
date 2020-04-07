#include "Grab.h"

#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>

#include "Block.h"

#include "ComponentRegister.h"

REGISTER_FACTORY(Grab);

Grab::Grab(GameObject* gameObject) : UserComponent(gameObject)
{
	duration = 1.5f;
	remain = 0.0f;

	force = 1500.0f;

	isGrounded = true;
	state = IDLE;
	last = IDLE;
	enemy = nullptr;

	vIzq = { -1,0,0 };
	vDer = { 1,0,0 };
}

void Grab::start()
{
	

	attackTrigger = gameObject->getComponent<RigidBody>();

	controller = gameObject->getParent()->getComponent<PlayerController>();

	// Deactivate the trigger until the attack is used
	//if (attackTrigger != nullptr) attackTrigger->setActive(false);
}

void Grab::update(float deltaTime)
{
	if (state == GRABBING && last == GRABBING) {
		state = IDLE;
	}

	last = state;

	if (remain > 0.0f) remain -= deltaTime;

	if (state == GRABBED) {
		enemy->transform->setPosition(gameObject->getParent()->transform->getPosition() + enemyDiff);
	}

	if (remain <= 0.0f && state == GRABBED) {
		drop();
		state = IDLE;
	}
	if (remain <= 0.0f && state == BLOCKED) {
		if (controller != nullptr)controller->setFrozen(false);
		state = IDLE;
	}
}







void Grab::onObjectStay(GameObject* other)
{
	if (state == GRABBING) {
		if (other->getTag() == "Player" && other != gameObject->getParent())//If it hits a player different than myself
		{

			std::vector<GameObject*> aux = other->findChildrenWithTag("groundSensor");
			Block* enemyBlock = nullptr;
			if (aux.size() > 0) enemyBlock = aux[0]->getComponent<Block>();
			if (enemyBlock != nullptr && enemyBlock->getGrabBlock()) {
				LOG("GRAB BLOCKED!");
				state = BLOCKED;
				remain = freezeDuration;
				if (controller != nullptr)controller->setFrozen(true);
				return;
			}

			enemyDiff = other->transform->getPosition() - gameObject->getParent()->transform->getPosition();

			state = GRABBED;
			remain = duration;
			enemy = other;
			enemyController = other->getComponent<PlayerController>();
			if (enemyController)enemyController->setFrozen(true);
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
		
		if (prop.first == "freezeDuration")
		{
			ss >> freezeDuration;
		}
	}
}

void Grab::grab()
{
	if (state == IDLE) state = GRABBING;
}

void Grab::drop()
{
	if (!enemy) return;
	//lanzar enemigo
	if (gameObject->getParent()->transform->getRotation().y >= 0) enemy->getComponent<RigidBody>()->addForce(vDer * force);
	else enemy->getComponent<RigidBody>()->addForce(vIzq * force);
	
	if (enemyController)enemyController->setFrozen(false);
	
	enemyController = nullptr;
	enemy = nullptr;
}
