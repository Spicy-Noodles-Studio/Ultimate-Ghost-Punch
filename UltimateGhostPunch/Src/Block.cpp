#include "Block.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>

#include "PlayerState.h"
#include "PlayerFX.h"

REGISTER_FACTORY(Block);

Block::Block(GameObject* gameObject) : UserComponent(gameObject), parent(nullptr), grounded(false), blocking(false), blocked(0), blockedGrab(0), maxBlockTime(0.5f), blockTime(0.5f),
blockRegenTime(1.5f), blockGrabMargin(0.25f), timeElapsed(0.0f), blockDirection(0)
{

}

Block::~Block()
{

}

void Block::start()
{
	parent = gameObject->getParent();
	if(parent != nullptr) playerFX = parent->getComponent<PlayerFX>();
	blockTime = maxBlockTime;
	timeElapsed = 0;

	checkNull(parent)
	checkNull(playerFX)
}

void Block::update(float deltaTime)
{
	if (!grounded && blocking)
	{
		blocking = false;
		return;
	}

	//Recharge block
	if (!blocking && blockTime != maxBlockTime)
	{
		timeElapsed += deltaTime;
		if (timeElapsed > blockRegenTime)
		{
			blockTime = maxBlockTime;
			timeElapsed = 0;
			LOG("BLOCK RECHARGED\n");
		}
	}

	//Blocking
	else if (blocking && blockTime > 0 && grounded)
	{
		blockTime -= deltaTime;
		if (playerFX != nullptr) playerFX->updateShield(blockTime, maxBlockTime);
		if (blockTime <= 0)
		{
			blockTime = 0;
			unblock();
			LOG("BLOCK ENDED\n");
		}
	}
}

void Block::postUpdate(float deltaTime)
{
	if (blocked > 0)blocked--;
	if (blockedGrab > 0)blockedGrab--;
}

void Block::handleData(ComponentData* data)
{
	if (data == nullptr) return;
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "maxBlockTime")
		{
			setFloat(maxBlockTime);
		}
		else if (prop.first == "blockRegenTime")
		{
			setFloat(blockRegenTime);
		}
		else if (prop.first == "blockGrabMargin")
		{
			setFloat(blockGrabMargin);
		}
		else
			LOG("BLOCK: Invalid property name %s", prop.first.c_str());
	}
}

void Block::onObjectEnter(GameObject* other)
{
	if (other != nullptr && other->getTag() == "suelo")
		grounded = true;
}

void Block::onObjectExit(GameObject* other)
{
	if (other != nullptr && other->getTag() == "suelo")
		grounded = false;
}

void Block::block()
{
	if (parent == nullptr) return;

	PlayerState* aux = parent->getComponent<PlayerState>();
	if (!blocking && blockTime > 0 && grounded && aux != nullptr && aux->canBlock())
	{
		blocking = true;
		timeElapsed = 0;
		if (parent->transform != nullptr) blockDirection = parent->transform->getRotation().y;

		PlayerFX* playerFX = parent->getComponent<PlayerFX>();
		if (playerFX != nullptr) playerFX->activateShield();

		LOG("BLOCKING\n");
	}
}

void Block::unblock()
{
	if (!blocking) return;

	blocking = false;

	if (parent == nullptr) return;

	PlayerFX* playerFX = parent->getComponent<PlayerFX>();

	if (playerFX != nullptr) playerFX->deactivateShield();
}

bool Block::blockAttack(Vector3 otherPosition)
{
	if (parent == nullptr || parent->transform == nullptr) return false;

	if (blocking && ((blockDirection > 0 && otherPosition.x > parent->transform->getPosition().x) ||
		(blockDirection < 0 && otherPosition.x < parent->transform->getPosition().x)))
	{
		blockTime -= 0.25f;
		LOG("Attack blocked\n");

		if (blockTime <= 0) blocking = false;

		blocked = 2;
		return true;
	}

	return false;
}

float Block::getMaxBlockTime() const
{
	return maxBlockTime;
}

bool Block::wasGrabBlocked() const
{
	return blocking && blockTime > maxBlockTime - blockGrabMargin;
}
void Block::grabBlocked()
{
	blockedGrab = 2;
	unblock();
}

bool Block::canBlockGrab() const
{
	return blocking && blockTime > maxBlockTime - blockGrabMargin;
}

bool Block::isBlocking() const
{
	return blocking;
}

bool Block::hasBlocked() const
{
	return blocked;
}

bool Block::hasBlockedGrab() const
{
	return blockedGrab;
}
