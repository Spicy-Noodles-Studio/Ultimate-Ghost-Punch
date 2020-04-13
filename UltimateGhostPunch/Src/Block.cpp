#include "Block.h"
#include <sstream>

#include "Health.h"
#include "PlayerAnimController.h"

#include "ComponentRegister.h"

REGISTER_FACTORY(Block);

Block::Block(GameObject* gameObject) : UserComponent(gameObject), isGrounded(false), isBlocking(false),blockRegenTime(1.5f), timeElapsed(0.0f), maxBlockTime(0.5f), blockTime(0.5f), 
									   blockGrabMargin(0.25f), blockDirection(0)
{
}



void Block::start()
{
	blockTime = maxBlockTime;
	timeElapsed = 0;
}

void Block::update(float deltaTime)
{
	if (!isGrounded && isBlocking) {
		isBlocking = false;
		return;
	}

	//Recharge block
	if (!isBlocking && blockTime != maxBlockTime) {
		timeElapsed += deltaTime;
		if (timeElapsed > blockRegenTime) {
			blockTime = maxBlockTime;
			timeElapsed = 0;
			LOG("BLOCK RECHARGED\n");
		}
	}
	//Blocking
	else if (isBlocking && blockTime > 0 && isGrounded) {
		blockTime -= deltaTime;
		if (blockTime <= 0) {
			blockTime = 0;
			isBlocking = false;
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
			if (!(ss >> maxBlockTime))
				LOG("BLOCK: Invalid value for property %s", prop.first.c_str());
		}
		else if (prop.first == "blockRegenTime") {
			if(!(ss >> blockRegenTime))
				LOG("BLOCK: Invalid value for property %s", prop.first.c_str());
		}
		else if (prop.first == "blockGrabMargin") {
			if(!(ss >> blockGrabMargin))
				LOG("BLOCK: Invalid value for property %s", prop.first.c_str());

		}
		else
			LOG("BLOCK: Invalid property name %s", prop.first.c_str());

	}
}

void Block::block()
{
	if (!isBlocking && blockTime > 0 && isGrounded) {
		isBlocking = true;
		timeElapsed = 0;
		blockDirection = gameObject->getParent()->transform->getRotation().y;
		LOG("BLOCKING\n");
	}
}

void Block::unblock()
{
	isBlocking = false;
}

bool Block::blockAttack(float damage, Vector3 otherPosition)
{
	if (isBlocking && ((blockDirection > 0 && otherPosition.x > gameObject->getParent()->transform->getPosition().x) ||
		(blockDirection < 0 && otherPosition.x < gameObject->getParent()->transform->getPosition().x))) {
		blockTime -= 0.25f;
		LOG("Attack blocked\n");
		if (blockTime <= 0) 
			isBlocking = false;

		// Attack blocked animation
		PlayerAnimController* anim = gameObject->getParent()->getComponent<PlayerAnimController>();
		if (anim != nullptr) anim->blockedAttackAnimation();

		return true;
	}
	else {
		Health* health = gameObject->getParent()->getComponent<Health>();
		if (health != nullptr) health->receiveDamage(damage);
	}
	return false;
}

void Block::onObjectEnter(GameObject* other)
{
	if (other->getTag() == "suelo") {
		isGrounded = true;
	}
}

void Block::onObjectExit(GameObject* other)
{
	if (other->getTag() == "suelo") {
		isGrounded = false;
	}
}

bool Block::getGrabBlock() const
{
	if (isBlocking && blockTime > maxBlockTime - blockGrabMargin) return true;
	else return false;
}

bool Block::blocking() const
{
	return isBlocking;
}
