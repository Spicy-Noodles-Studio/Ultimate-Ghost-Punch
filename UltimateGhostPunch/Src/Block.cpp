#include "Block.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>

#include "PlayerController.h"
#include "PlayerAnimController.h"
#include "PlayerState.h"
#include "Health.h"

REGISTER_FACTORY(Block);

Block::Block(GameObject* gameObject) : UserComponent(gameObject), grounded(false), blocking(false), blockRegenTime(1.5f), timeElapsed(0.0f), maxBlockTime(0.5f), blockTime(0.5f),
									   blockGrabMargin(0.25f), blockDirection(0)
{

}

Block::~Block()
{

}

void Block::start()
{
	blockTime = maxBlockTime;
	timeElapsed = 0;
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
		if (blockTime <= 0)
		{
			blockTime = 0;
			blocking = false;
			LOG("BLOCK ENDED\n");
		}
	}
}

void Block::handleData(ComponentData* data)
{
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
	if (other->getTag() == "suelo")
		grounded = true;
}

void Block::onObjectExit(GameObject* other)
{
	if (other->getTag() == "suelo")
		grounded = false;
}

void Block::block()
{
	PlayerState* aux = gameObject->getParent()->getComponent<PlayerState>();

	if (!blocking && blockTime > 0 && grounded && aux->canBlock())
	{
		blocking = true;
		timeElapsed = 0;
		blockDirection = gameObject->getParent()->transform->getRotation().y;

		auto anim = gameObject->getParent()->getComponent<PlayerAnimController>();

		if(anim != nullptr)
			anim->blockAnimation();

		LOG("BLOCKING\n");
	}
}

void Block::unblock()
{
	blocking = false;
}

bool Block::blockAttack(float damage, Vector3 otherPosition)
{
	if (blocking && ((blockDirection > 0 && otherPosition.x > gameObject->getParent()->transform->getPosition().x) ||
	   (blockDirection < 0 && otherPosition.x < gameObject->getParent()->transform->getPosition().x)))
	{
		blockTime -= 0.25f;
		LOG("Attack blocked\n");

		if (blockTime <= 0) 
			blocking = false;

		// Attack blocked animation
		PlayerAnimController* anim = gameObject->getParent()->getComponent<PlayerAnimController>();

		if (anim != nullptr)
			anim->blockedAttackAnimation();

		return true;
	}
	else
	{
		Health* health = gameObject->getParent()->getComponent<Health>();

		if (health != nullptr)
			health->receiveDamage(damage);
	}

	return false;
}

bool Block::wasGrabBlocked() const
{
	return blocking && blockTime > maxBlockTime - blockGrabMargin;
}

bool Block::isBlocking() const
{
	return blocking;
}