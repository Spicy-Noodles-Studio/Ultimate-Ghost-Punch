#include "Grab.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <MeshRenderer.h>
#include <RigidBody.h>
#include <sstream>

#include "PlayerController.h"
#include "PlayerIndex.h"
#include "PlayerState.h"
#include "Score.h"
#include "Block.h"
#include "Health.h"
#include "GhostManager.h"
#include "GameManager.h"

REGISTER_FACTORY(Grab);

Grab::Grab(GameObject* gameObject) : UserComponent(gameObject), id(0), grabDuration(1.5f), freezeDuration(1.0f), throwForce(15.0f), remain(0.0f), cooldown(2.00f), grabTimer(0.0f),
									 grabVerticalOffset(3.0f), dropHorizontalOffset(0.50f), state(IDLE), parent(nullptr), controller(nullptr), enemy(nullptr), enemyController(nullptr),
									 enemyDiff(Vector3::ZERO), enemyFollowing(false), grabbedPosition(Vector3::ZERO), prevOrientation(1), enemyFollowingThreshold(0.3f), score(nullptr), missed(0), dropped(0)
{

}

Grab::~Grab()
{

}

void Grab::start()
{
	parent = gameObject->getParent();

	if (parent != nullptr)
	{
		id = parent->getComponent<PlayerIndex>()->getIndex();
		controller = parent->getComponent<PlayerController>();	 
	}

	score = GameManager::GetInstance()->getScore();
}

void Grab::update(float deltaTime)
{
	if (remain > 0.0f) remain -= deltaTime;
	if (grabTimer > 0.0f) grabTimer -= deltaTime;

	if (parent == nullptr) return;
	int newOrientation = (parent->transform->getRotation().y >= 0) ? 1 : -1;

	if (state == GRABBED)
	{
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
				grabbedPosition = parent->transform->getPosition() + Vector3(0, parent->transform->getScale().y * grabVerticalOffset, 0);
				objPos = grabbedPosition;
			}

			enemy->transform->setPosition(objPos);
			if (newOrientation != prevOrientation)
				enemy->transform->setRotation({ 0,double(90 * newOrientation),0 });
		}
	}

	if (remain <= 0.0f && state == GRABBED)
		drop();

	else if (remain <= 0.0f && state == BLOCKED)
	{
		PlayerState* playerState = gameObject->getComponent<PlayerState>();
		if (playerState != nullptr)
			playerState->setIgnoringInput(false); // Unfreeze our character

		state = IDLE;
	}

	prevOrientation = newOrientation;
}

void Grab::postUpdate(float deltaTime)
{
	if (missed > 0)missed--;
	if (dropped > 0)dropped--;
}

void Grab::onObjectStay(GameObject* other)
{
	if (other->getTag() == "Player")
	{
		GhostManager* enemyGM = other->getComponent<GhostManager>();

		if (enemyGM == nullptr || enemyGM->isGhost())
		{
			if (enemy == other)
				enemy = nullptr;
		}
		else if (enemy == nullptr && parent != nullptr && other != parent) // If it hits a player different than myself
			enemy = other;
	}
}

void Grab::onObjectEnter(GameObject* other)
{
	if (other->getTag() == "Player")
	{
		GhostManager* enemyGM = other->getComponent<GhostManager>();

		if (enemyGM == nullptr || !enemyGM->isGhost())
			if (parent != nullptr && other != parent) //If it hits a player different than myself
				enemy = other;
	}
}

void Grab::onObjectExit(GameObject* other)
{
	if (other == enemy && state != GRABBED)
		enemy = nullptr;
}

void Grab::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "freezeDuration")
		{
			setFloat(freezeDuration);
		}
		else if (prop.first == "throwForce")
		{
			setFloat(throwForce);
		}
		else if (prop.first == "grabDuration")
		{
			setFloat(grabDuration);
		}
		else if (prop.first == "cooldown")
		{
			if (!(ss >> cooldown))
				LOG("GRAB: Invalid value for property %s", prop.first.c_str());
		}
		else
			LOG("GRAB: Invalid property name %s", prop.first.c_str());
	}
}

void Grab::grab()
{
	if (parent == nullptr) return;
	PlayerState* aux = parent->getComponent<PlayerState>();

	if (state == IDLE && grabTimer <= 0 && aux->canGrab())
	{
		if (enemy != nullptr && !enemy->getComponent<PlayerState>()->isGrabbed() && !enemy->getComponent<Health>()->isInvencible())
			grabEnemy();
		else
			grabMissed();

		grabTimer = cooldown;
	}
}

void Grab::drop()
{
	if (state != GRABBED || enemy == nullptr || parent == nullptr)
		return;

	resetEnemy();

	//Throw enemy
	Vector3 dir = Vector3(0, 0, 0);
	dir.x = (parent->transform->getRotation().y >= 0) ? 1 : -1;
	enemy->transform->setPosition(parent->transform->getPosition() + Vector3((parent->transform->getScale().x / 2) + (dropHorizontalOffset * dir.x), enemy->transform->getPosition().y - parent->transform->getPosition().y, 0));

	RigidBody* enemyRb = enemy->getComponent<RigidBody>();
	if (enemyRb != nullptr)
		enemyRb->addImpulse(dir * throwForce);

	//Reset state
	enemyController = nullptr;
	enemy = nullptr;
	state = IDLE;
	grabTimer = cooldown;

	dropped = 2;
}

void Grab::grabMissed()
{
	state = IDLE;
	missed = 2;
}

void Grab::setGrabbed(bool grabbed)
{
	beingGrabbed = grabbed;
}

bool Grab::isGrabbing() const
{
	return state == GRABBED;
}

bool Grab::isGrabbed() const
{
	return beingGrabbed;
}

bool Grab::isOnCooldown() const
{
	return cooldown > 0;
}

bool Grab::isStunned() const
{
	return state == BLOCKED;
}

bool Grab::hasMissed() const
{
	return missed > 0;
}

bool Grab::hasDropped() const
{
	return dropped > 0;
}

void Grab::resetEnemy()
{
	if (enemy == nullptr)
		return;

	//Reactivate enemy's RigidBody
	RigidBody* enemyRB = enemy->getComponent<RigidBody>();
	Grab* enemyGrab = nullptr;
	std::vector<GameObject*> aux = enemy->findChildrenWithTag("grabSensor");
	if (aux.size() > 0)
		enemyGrab = aux[0]->getComponent<Grab>();

	if (enemyRB != nullptr)
	{
		enemyRB->setTrigger(false);
		enemyRB->setActive(true);
	}

	PlayerState* enemyState = enemy->getComponent<PlayerState>();
	if (enemyState != nullptr)
	{
		enemyState->setThrown();
		//Return control to the enemy
		enemyState->setIgnoringInput(false);
	}

	


	grabbedPosition = Vector3();
}

void Grab::grabEnemy()
{
	if (enemy == nullptr || parent == nullptr)
		return;

	//Check if we have been blocked
	std::vector<GameObject*> aux = enemy->findChildrenWithTag("groundSensor");
	Block* enemyBlock = nullptr;
	if (aux.size() > 0)
		enemyBlock = aux[0]->getComponent<Block>();
	std::vector<GameObject*> grabSensorChildren = enemy->findChildrenWithTag("grabSensor");
	Grab* enemyGrab = nullptr;
	if (grabSensorChildren.size() > 0)
		enemyGrab = grabSensorChildren[0]->getComponent<Grab>();


	if (enemyBlock != nullptr && enemyBlock->canBlockGrab())
	{
		int dir = (parent->transform->getRotation().y >= 0) ? 1 : -1;
		int enemyDir = (enemy->transform->getRotation().y >= 0) ? 1 : -1;

		if (dir != enemyDir) // ONLY BLOCK if blocking dir is correct
		{
			LOG("GRAB BLOCKED!");
			enemyBlock->grabBlocked();
			state = BLOCKED;
			remain = freezeDuration;
			PlayerState* playerState = gameObject->getComponent<PlayerState>();
			if (playerState != nullptr)
				playerState->setIgnoringInput(true); // Freeze our character

			return;
		}
	}

	//Grab the enemy
	if (score != nullptr)
		score->grabHitted(id);

	if (enemyBlock != nullptr && enemyBlock->isBlocking())
		enemyBlock->unblock();

	state = GRABBED;
	remain = grabDuration;
	
	PlayerState* enemyState = enemy->getComponent<PlayerState>();
	if (enemyState != nullptr)
	{
		enemyState->setGrabbed();
		enemyState->setIgnoringInput(true);
	}

	
	enemyDiff = enemy->transform->getPosition() - parent->transform->getPosition();

	//Freeze the enemy while grabbing
	if (enemyGrab != nullptr) enemyGrab->setGrabbed(true);
	if (enemyState != nullptr) enemyState->setIgnoringInput(true);

	grabbedPosition = parent->transform->getPosition() + Vector3(0, parent->transform->getScale().y * grabVerticalOffset, 0);

	RigidBody* enemyRB = enemy->getComponent<RigidBody>();
	if (enemyRB != nullptr)
	{
		enemyRB->setTrigger(true);
		enemyRB->setActive(false);
	}
}