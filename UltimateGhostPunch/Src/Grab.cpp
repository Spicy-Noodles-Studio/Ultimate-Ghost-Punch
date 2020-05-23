#include "Grab.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>

#include "PlayerIndex.h"
#include "PlayerState.h"
#include "Score.h"
#include "Block.h"
#include "Health.h"
#include "GhostManager.h"
#include "GameManager.h"

REGISTER_FACTORY(Grab);

Grab::Grab(GameObject* gameObject) : UserComponent(gameObject), id(0), grabDuration(1.5f), freezeDuration(1.0f), throwForce(15.0f), remain(0.0f), cooldown(2.00f), grabTimer(0.0f),
grabVerticalOffset(3.0f), dropHorizontalOffset(0.50f), state(IDLE), parent(nullptr), enemy(nullptr), enemyDiff(Vector3::ZERO), enemyFollowing(false),
grabbedPosition(Vector3::ZERO), prevOrientation(1), enemyFollowingThreshold(0.3f), score(nullptr), missed(0), dropped(0)
{

}

Grab::~Grab()
{

}

void Grab::start()
{
	parent = gameObject->getParent();

	if (notNull(GameManager::GetInstance()))
		score = GameManager::GetInstance()->getScore();
	checkNull(score);

	checkNullAndBreak(parent);
	PlayerIndex* index = parent->getComponent<PlayerIndex>();

	checkNullAndBreak(index);
	id = index->getIndex();

}

void Grab::update(float deltaTime)
{
	if (remain > 0.0f) remain -= deltaTime;
	if (grabTimer > 0.0f) grabTimer -= deltaTime;

	checkNullAndBreak(parent);
	checkNullAndBreak(parent->transform);
	int newOrientation = (parent->transform->getRotation().y >= 0) ? 1 : -1;

	if (state == GRABBED)
	{
		if (notNull(enemy) && notNull(enemy->transform))
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
		if (notNull(playerState)) playerState->setIgnoringInput(false); // Unfreeze our character

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
	if (notNull(other) && parent != nullptr && other->getTag() == "Player" && other != parent)
	{
		GhostManager* enemyGM = other->getComponent<GhostManager>();

		if (enemyGM == nullptr || enemyGM->isGhost()) {
			if (enemy == other) enemy = nullptr;
		}
		else if (enemy == nullptr) // If it hits a player different than myself
			enemy = other;
	}
}

void Grab::onObjectEnter(GameObject* other)
{
	if (notNull(other) && parent != nullptr && other->getTag() == "Player" && other != parent)
	{
		GhostManager* enemyGM = other->getComponent<GhostManager>();

		if (enemyGM == nullptr || enemyGM->isGhost()) {
			if (enemy == nullptr) //If it hits a player different than myself
				enemy = other;
		}
	}
}

void Grab::onObjectExit(GameObject* other)
{
	if (notNull(other) && other == enemy && state != GRABBED)
		enemy = nullptr;
}

void Grab::handleData(ComponentData* data)
{
	checkNullAndBreak(data);
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
			setFloat(cooldown);
		}
		else
			LOG("GRAB: Invalid property name %s", prop.first.c_str());
	}
}

void Grab::grab()
{
	checkNullAndBreak(parent);
	PlayerState* aux = parent->getComponent<PlayerState>();

	if (state == IDLE && grabTimer <= 0 && notNull(aux) && aux->canGrab())
	{
		if (notNull(enemy) && (!notNull(enemy->getComponent<PlayerState>()) || !enemy->getComponent<PlayerState>()->isGrabbed()) &&
			(!notNull(enemy->getComponent<Health>()) || !enemy->getComponent<Health>()->isInvencible()))
			grabEnemy();
		else
			grabMissed();

		grabTimer = cooldown;
	}
}

void Grab::drop()
{
	if (state != GRABBED || !notNull(enemy) || !notNull(parent) ||
		!notNull(parent->transform) || !notNull(enemy->transform)) return;

	resetEnemy();

	//Throw enemy
	Vector3 dir = Vector3::ZERO;
	dir.x = (parent->transform->getRotation().y >= 0) ? 1 : -1;
	enemy->transform->setPosition(parent->transform->getPosition() + Vector3((parent->transform->getScale().x / 2) + (dropHorizontalOffset * dir.x), enemy->transform->getPosition().y - parent->transform->getPosition().y, 0));

	RigidBody* enemyRb = enemy->getComponent<RigidBody>();
	if (notNull(enemyRb)) enemyRb->addImpulse(dir * throwForce);

	//Reset state
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

bool Grab::isGrabbing() const
{
	return state == GRABBED;
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
	checkNullAndBreak(enemy);

	//Reactivate enemy's RigidBody
	RigidBody* enemyRB = enemy->getComponent<RigidBody>();

	if (notNull(enemyRB))
	{
		enemyRB->setTrigger(false);
		enemyRB->setActive(true);
	}

	PlayerState* enemyState = enemy->getComponent<PlayerState>();
	if (notNull(enemyState))
	{
		//Return control to the enemy
		enemyState->setIgnoringInput(false);
		enemyState->setThrown();
	}

	grabbedPosition = Vector3::ZERO;
}

void Grab::grabEnemy()
{
	if (!notNull(enemy) || !notNull(parent) ||
		!notNull(parent->transform) || !notNull(enemy->transform)) return;

	//Check if we have been blocked
	Block* enemyBlock = nullptr;
	std::vector<GameObject*> groundSensorChildren = enemy->findChildrenWithTag("groundSensor");
	if (groundSensorChildren.size() > 0) enemyBlock = groundSensorChildren[0]->getComponent<Block>();

	if (notNull(enemyBlock) && enemyBlock->canBlockGrab())
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
			if (notNull(playerState)) playerState->setIgnoringInput(true); // Freeze our character

			return;
		}
	}

	//Grab the enemy
	if (notNull(score)) score->grabHitted(id);

	if (notNull(enemyBlock) && enemyBlock->isBlocking()) enemyBlock->unblock();

	state = GRABBED;
	remain = grabDuration;

	//Freeze the enemy while grabbing
	PlayerState* enemyState = enemy->getComponent<PlayerState>();
	if (notNull(enemyState))
	{
		enemyState->setGrabbed();
		enemyState->setIgnoringInput(true);
	}

	enemyDiff = enemy->transform->getPosition() - parent->transform->getPosition();
	grabbedPosition = parent->transform->getPosition() + Vector3(0, parent->transform->getScale().y * grabVerticalOffset, 0);

	RigidBody* enemyRB = enemy->getComponent<RigidBody>();
	if (notNull(enemyRB))
	{
		enemyRB->setTrigger(true);
		enemyRB->setActive(false);
	}
}