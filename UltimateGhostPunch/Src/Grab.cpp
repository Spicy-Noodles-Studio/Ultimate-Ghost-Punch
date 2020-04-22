#include "Grab.h"

#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>

#include "Block.h"
#include "PlayerAnimController.h"

#include "ComponentRegister.h"
#include "GameManager.h"
#include "Score.h"
#include "PlayerIndex.h"
#include "MeshRenderer.h"

REGISTER_FACTORY(Grab);

Grab::Grab(GameObject* gameObject) : UserComponent(gameObject), grabDuration(1.5f), freezeDuration(1.0f), remain(0.0f), throwForce(15.0f), state(IDLE), enemy(nullptr),
controller(nullptr), enemyController(nullptr), enemyDiff(Vector3()), cooldown(2.00f), grabTimer(0.0f), grabVerticalOffset(3.0f), dropHorizontalOffset(0.50f), prevOrientation(1), enemyFollowingThreshold(0.3f)
{

}

void Grab::start()
{
	controller = gameObject->getParent()->getComponent<PlayerController>();
	id= gameObject->getParent()->getComponent<PlayerIndex>()->getIndex();
	score = GameManager::GetInstance()->getScore();
}

void Grab::update(float deltaTime)
{
	int newOrientation = (gameObject->getParent()->transform->getRotation().y >= 0) ? 1 : -1;

	// Check if it can grab
	if (state == GRABBING && enemy != nullptr)
		grabEnemy();
	else if (state == GRABBING && enemy == nullptr) // state is GRABBING but enemy is null ==> failed grab
	{
		PlayerAnimController* myAnim = gameObject->getParent()->getComponent<PlayerAnimController>();
		if (myAnim != nullptr) myAnim->grabFailedAnimation();

		state = IDLE;
	}

	if (remain > 0.0f) remain -= deltaTime;

	if (grabTimer > 0.0f) grabTimer -= deltaTime;

	if (state == GRABBED) {
		if (enemy != nullptr)
		{
			Vector3 objPos;
			float dist = (grabbedPosition - enemy->transform->getPosition()).magnitude();
			if (!enemyFollowing && dist > enemyFollowingThreshold)
			{
				objPos = enemy->transform->getPosition();
				objPos.lerp(grabbedPosition, 0.2f);
			}
			else if (dist <= enemyFollowingThreshold)
			{
				grabbedPosition = gameObject->getParent()->transform->getPosition() + Vector3(0, gameObject->getParent()->transform->getScale().y * grabVerticalOffset, 0);
				objPos = grabbedPosition;
			}

			enemy->transform->setPosition(objPos);
			if(newOrientation != prevOrientation)
				enemy->transform->setRotation({ 0,double(90 * newOrientation),0 });
		}
	}

	if (remain <= 0.0f && state == GRABBED) {
		resetEnemy(); 

		//Reset state
		enemyController = nullptr;
		enemy = nullptr;
		state = IDLE;
	}
	else if (remain <= 0.0f && state == BLOCKED) {
		if (controller != nullptr)controller->setActive(true);
		state = IDLE;
	}

	prevOrientation = newOrientation;
}

void Grab::onObjectStay(GameObject* other)
{
	if (enemy == nullptr && other->getTag() == "Player" && other != gameObject->getParent())//If it hits a player different than myself
		enemy = other;
}

void Grab::onObjectEnter(GameObject* other)
{
	if (other->getTag() == "Player" && other != gameObject->getParent())//If it hits a player different than myself
		enemy = other;
}

void Grab::onObjectExit(GameObject* other)
{
	if (other == enemy && state != GRABBED)
		enemy = nullptr;
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
		else if (prop.first == "cooldown") {
			if (!(ss >> cooldown))
				LOG("GRAB: Invalid value for property %s", prop.first.c_str());
		}
		else
			LOG("GRAB: Invalid property name %s", prop.first.c_str());
	}
}

void Grab::grab()
{
	if (state == IDLE && grabTimer <= 0)
	{
		state = GRABBING;
		grabTimer = cooldown;
	}
}

void Grab::drop()
{
	if (enemy == nullptr) 
		return;

	resetEnemy();

	//Throw enemy
	Vector3 dir = Vector3(0, 0, 0);
	dir.x = (gameObject->getParent()->transform->getRotation().y >= 0) ? 1 : -1;
	enemy->transform->setPosition(gameObject->getParent()->transform->getPosition() + Vector3((gameObject->getParent()->transform->getScale().x/2) + (dropHorizontalOffset * dir.x), enemy->transform->getPosition().y - gameObject->getParent()->transform->getPosition().y, 0));
	RigidBody* enemyRb = enemy->getComponent<RigidBody>();
	if(enemyRb!=nullptr) enemyRb->addImpulse(dir * throwForce);

	//Reset state
	enemyController = nullptr;
	enemy = nullptr;
	state = IDLE;
}

bool Grab::isGrabbing() const
{
	return state== GRABBING || state == GRABBED;
}

bool Grab::isOnCooldown() const
{
	return cooldown > 0;
}

void Grab::resetEnemy()
{
	grabTimer = cooldown;
	if (enemy == nullptr) return;

	//Reactivate enemy's RigidBody
	RigidBody* enemyRB = enemy->getComponent<RigidBody>();
	if (enemyRB != nullptr) { enemyRB->setTrigger(false); enemyRB->setActive(true); }

	//Return control to the enemy
	if (enemyController != nullptr)enemyController->setActive(true);


	PlayerAnimController* enemyAnim = enemy->getComponent<PlayerAnimController>();
	if (enemyAnim != nullptr)
		enemyAnim->thrownAwayAnimation();

	//Play throw animation
	PlayerAnimController* myAnim = gameObject->getParent()->getComponent<PlayerAnimController>();
	if (myAnim != nullptr)
		myAnim->throwEnemyAnimation();

	grabbedPosition = Vector3();
}

void Grab::grabEnemy()
{
	PlayerAnimController* enemyAnim = enemy->getComponent<PlayerAnimController>();
	PlayerAnimController* myAnim = gameObject->getParent()->getComponent<PlayerAnimController>();

	
	//Check if we have been blocked
	std::vector<GameObject*> aux = enemy->findChildrenWithTag("groundSensor");
	Block* enemyBlock = nullptr;
	if (aux.size() > 0) enemyBlock = aux[0]->getComponent<Block>();

	
	if (enemyBlock != nullptr && enemyBlock->getGrabBlock()) {
		int dir = (gameObject->getParent()->transform->getRotation().y >= 0) ? 1 : -1;
		int enemyDir = (enemy->transform->getRotation().y >= 0) ? 1 : -1;
		if (dir != enemyDir) // ONLY BLOCK if blocking dir is correct
		{
			LOG("GRAB BLOCKED!");
			state = BLOCKED;
			remain = freezeDuration;
			if (controller != nullptr)controller->setActive(false);//freeze our character


			if (enemyAnim != nullptr) enemyAnim->blockedEnemyGrabAnimation();
			if (myAnim != nullptr) myAnim->enemyBlockedMyGrabAnimation();

			return;
		}
			

	}

	//Grab the enemy
	enemyDiff = enemy->transform->getPosition() - gameObject->getParent()->transform->getPosition();
	if(score!=nullptr)
	score->grabbedBy(enemy->getComponent<PlayerIndex>()->getIndex(), id);
	state = GRABBED;
	remain = grabDuration;
	enemy = enemy;
	enemyController = enemy->getComponent<PlayerController>();
	if (enemyController != nullptr) enemyController->setActive(false);//freeze the enemy
	grabTimer = 10000000; // initially infinite, will be set to cooldown time after droping the enemy
	grabbedPosition = gameObject->getParent()->transform->getPosition() + Vector3(0, gameObject->getParent()->transform->getScale().y * grabVerticalOffset, 0);

	RigidBody* enemyRB = enemy->getComponent<RigidBody>();
	if (enemyRB != nullptr) { enemyRB->setTrigger(true); enemyRB->setActive(false); }


	if (myAnim != nullptr) myAnim->grabAnimation();
	if (enemyAnim != nullptr) enemyAnim->grabbedByEnemyAnimation();
}
