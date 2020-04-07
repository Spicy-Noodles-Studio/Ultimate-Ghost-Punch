#include "Block.h"
#include <sstream>

#include "Health.h"

#include "ComponentRegister.h"

REGISTER_FACTORY(Block);

Block::Block(GameObject* gameObject) : UserComponent(gameObject)
{
}

bool Block::block()
{
	if (!isBlocking && blockTime > 0 && isGrounded) {
		isBlocking = true;
		timeElapsed = 0;
		blockDirection = gameObject->getParent()->transform->getRotation().y;
		LOG("BLOCKING\n");
	}
	return isBlocking;
}

bool Block::unblock()
{
	isBlocking = false;
	return isBlocking;
}

bool Block::blockAttack(float damage, Vector3 otherPosition)
{
	if (isBlocking && ((blockDirection > 0 && otherPosition.x > gameObject->getParent()->transform->getPosition().x) ||
		(blockDirection < 0 && otherPosition.x < gameObject->getParent()->transform->getPosition().x))) {
		blockTime -= 0.25f;
		LOG("Attack blocked\n");
		if (blockTime <= 0) {
			LOG("BLOCK ENDED\n");
			isBlocking = false;
		}
		return true;
	}
	else {
		Health* health = gameObject->getParent()->getComponent<Health>();
		if (health != nullptr) health->receiveDamage(damage);
		LOG("Attack received\n");
	}
	return false;
}

void Block::start()
{
	rigidBody = gameObject->getParent()->getComponent<RigidBody>();

	isBlocking = false;
	blockTime = maxBlockTime;
	timeElapsed = 0;
}

void Block::update(float deltaTime)
{
	if (!isGrounded && isBlocking) {
		isBlocking = false;
		return;
	}

	if (!isBlocking && blockTime != maxBlockTime) {
		timeElapsed += deltaTime;
		if (timeElapsed > blockRegenTime) {
			blockTime = maxBlockTime;
			timeElapsed = 0;
			LOG("BLOCK RECHARGED\n");
		}
	}
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
			ss >> maxBlockTime;
		}
		if (prop.first == "blockRegenTime") {
			ss >> blockRegenTime;
		}
		if (prop.first == "blockGrabMargin") {
			ss >> blockGrabMargin;
		}
	}
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

bool Block::getGrabBlock()
{
	if (isBlocking && blockTime > maxBlockTime - blockGrabMargin) return true;
	else return false;
}
