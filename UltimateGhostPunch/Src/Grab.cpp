#include "Grab.h"

#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>

#include "Block.h"
#include "PlayerAnimController.h"

#include "ComponentRegister.h"

REGISTER_FACTORY(Grab);

Grab::Grab(GameObject* gameObject) : UserComponent(gameObject), grabDuration(1.5f), freezeDuration(1.0f), remain(0.0f), throwForce(15.0f), state(IDLE), last(IDLE), enemy(nullptr),
									 controller(nullptr), enemyController(nullptr), enemyDiff(Vector3())
{

}

void Grab::start()
{
	controller = gameObject->getParent()->getComponent<PlayerController>();
}

void Grab::update(float deltaTime)
{
	if (state == GRABBING && last == GRABBING) {
		state = IDLE;
	}

	last = state;

	if (remain > 0.0f) remain -= deltaTime;

	if (state == GRABBED) {
		if(enemy!= nullptr) enemy->transform->setPosition(gameObject->getParent()->transform->getPosition() + enemyDiff);
	}

	if (remain <= 0.0f && state == GRABBED) {
		drop();
		state = IDLE;
	}
	else if (remain <= 0.0f && state == BLOCKED) {
		if (controller != nullptr)controller->setActive(true);
		state = IDLE;
	}
}

void Grab::onObjectStay(GameObject* other)
{
	if (state == GRABBING) {
		if (other->getTag() == "Player" && other != gameObject->getParent())//If it hits a player different than myself
		{
			PlayerAnimController* otherAnim = other->getComponent<PlayerAnimController>();
			PlayerAnimController* myAnim = gameObject->getParent()->getComponent<PlayerAnimController>();

			//Check if we have been blocked
			std::vector<GameObject*> aux = other->findChildrenWithTag("groundSensor");
			Block* enemyBlock = nullptr;
			if (aux.size() > 0) enemyBlock = aux[0]->getComponent<Block>();

			if (enemyBlock != nullptr && enemyBlock->getGrabBlock()) {
				LOG("GRAB BLOCKED!");
				state = BLOCKED;
				remain = freezeDuration;
				if (controller != nullptr)controller->setActive(false);//freeze our character

				
				if (otherAnim != nullptr) otherAnim->blockedEnemyGrabAnimation();
				if (myAnim != nullptr) myAnim->enemyBlockedMyGrabAnimation();

				return;
			}

			//Grab the enemy
			enemyDiff = other->transform->getPosition() - gameObject->getParent()->transform->getPosition();

			state = GRABBED;
			remain = grabDuration;
			enemy = other;
			enemyController = other->getComponent<PlayerController>();
			if (enemyController != nullptr) enemyController->setActive(false);//freeze the enemy
			if (otherAnim != nullptr) otherAnim->grabbedByEnemyAnimation();
		}
	}
}

void Grab::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);
		
		if (prop.first == "freezeDuration")
		{
			if (!(ss >> freezeDuration))
				LOG("GRAB: Invalid value for property %s", prop.first.c_str());
		}
		else if (prop.first == "throwForce") {
			if (!(ss >> throwForce))
				LOG("GRAB: Invalid value for property %s", prop.first.c_str());
		}
		else if (prop.first == "grabDuration") {
			if (!(ss >> grabDuration))
				LOG("GRAB: Invalid value for property %s", prop.first.c_str());
		}
		else
			LOG("GRAB: Invalid property name %s", prop.first.c_str());
	}
}

void Grab::grab()
{
	if (state == IDLE) state = GRABBING;
}

void Grab::drop()
{
	if (enemy == nullptr) return;

	//Throw enemy
	Vector3 dir = Vector3(0, 0, 0);
	dir.x = (gameObject->getParent()->transform->getRotation().y >= 0) ? 1 : -1;
	RigidBody* enemyRb = enemy->getComponent<RigidBody>();
	if(enemyRb!=nullptr) enemyRb->addImpulse(dir * throwForce);
	
	//Return control to the enemy
	if (enemyController != nullptr)enemyController->setActive(true);

	//Play throw animations
	PlayerAnimController* otherAnim = enemy->getComponent<PlayerAnimController>();
	PlayerAnimController* myAnim = gameObject->getParent()->getComponent<PlayerAnimController>();
	if (otherAnim != nullptr)
		otherAnim->thrownAwayAnimation();
	if (myAnim != nullptr)
		myAnim->throwEnemyAnimation();

	//Reset state
	enemyController = nullptr;
	enemy = nullptr;
	state = IDLE;
}

bool Grab::isGrabbing() const
{
	return state== GRABBING || state == GRABBED;
}
