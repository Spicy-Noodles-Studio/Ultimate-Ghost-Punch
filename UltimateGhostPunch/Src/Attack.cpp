#include "Attack.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>

#include "Health.h"
#include "Block.h"
#include "ComponentRegister.h"
#include "GameManager.h"
#include "Score.h"
#include "PlayerIndex.h"

REGISTER_FACTORY(Attack);

Attack::Attack(GameObject* gameObject) : UserComponent(gameObject), attackTrigger(nullptr),currentAttack(NONE),state(NOT_ATTACKING),activeTime(0.0f), attackDuration(0.5f),
										 strongAttackDamage(2), quickAttackDamage(1), chargeTime(0), strongChargeTime(0.75f), quickChargeTime(0.5f), strongAttackCooldown(2.0f),
										 quickAttackCooldown(0.5f),cooldown(0.0f)
{

}

Attack::~Attack()
{

}

void Attack::start()
{
	attackTrigger = gameObject->getComponent<RigidBody>();

	// Deactivate the trigger until the attack is used
	if (attackTrigger != nullptr) attackTrigger->setActive(false);
}

void Attack::update(float deltaTime)
{
	// Update the cooldown
	if (cooldown > 0.0f)
		cooldown -= deltaTime;

	//Attack charge time
	if (chargeTime > 0.0f)
		chargeTime -= deltaTime;
	else if (state == CHARGING)
		attack();

	// Attack active time
	if (activeTime > 0.0f)
		activeTime -= deltaTime;
	else if (state == ATTACKING)
	{
		// Deactivate the trigger until the next attack is used
		attackTrigger->setActive(false);

		// Reset the current attack state
		state = NOT_ATTACKING;
	}
}

void Attack::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "quickCooldown") {
			if (!(ss >> quickAttackCooldown))
				LOG("ATTACK: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "strongCooldown") {
			if (!(ss >> strongAttackCooldown))
				LOG("ATTACK: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "quickDamage") {
			if (!(ss >> quickAttackDamage))
				LOG("ATTACK: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "strongDamage") {
			if (!(ss >> strongAttackDamage))
				LOG("ATTACK: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "attackDuration") {
			if (!(ss >> attackDuration))
				LOG("ATTACK: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "quickCharge") {
			if (!(ss >> quickChargeTime))
				LOG("ATTACK: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "strongCharge") {
			if (!(ss >> strongChargeTime))
				LOG("ATTACK: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("ATTACK: Invalid property name \"%s\"", prop.first.c_str());
	}
}


void Attack::onObjectStay(GameObject* other)
{
	if (other->getTag() == "Player" && other != gameObject->getParent() && state == ATTACKING)//If it hits a player different than myself
	{
		Score* score = GameManager::GetInstance()->getScore();
		LOG("You hit player %s!\n", other->getName().c_str());
		float damage = 0;

		switch (currentAttack)
		{
		case Attack::QUICK:
			damage = quickAttackDamage;
			break;
		case Attack::STRONG:
			damage = strongAttackDamage;
			break;
		}

		std::vector<GameObject*> aux = other->findChildrenWithTag("groundSensor");
		Block* enemyBlock = nullptr;
		if (aux.size() > 0) enemyBlock = aux[0]->getComponent<Block>();
		if (enemyBlock != nullptr) {
			enemyBlock->blockAttack(damage, gameObject->getParent()->transform->getPosition());

			// Deactivate the trigger until the next attack is used
			attackTrigger->setActive(false);

			// Reset the current attack state
			state = NOT_ATTACKING;

		}
		else {
			Health* enemyHealth = other->getComponent<Health>();
			int health = enemyHealth->getHealth();
			if (enemyHealth != nullptr) enemyHealth->receiveDamage(damage);
			score->receiveHitFrom(other->getComponent<PlayerIndex>()->getIndex(), gameObject->getParent()->getComponent<PlayerIndex>()->getIndex());
			if(health!= enemyHealth->getHealth())
				score->damageRecivedFrom(other->getComponent<PlayerIndex>()->getIndex(), gameObject->getParent()->getComponent<PlayerIndex>()->getIndex(), damage);
			if (!enemyHealth->isAlive())
			{
				score->killedBy(other->getComponent<PlayerIndex>()->getIndex(), gameObject->getParent()->getComponent<PlayerIndex>()->getIndex());
			}
		}
	}
}

void Attack::charge(float newCooldown, float newChargeTime)
{
	state = CHARGING;
	chargeTime = newChargeTime;
	cooldown = newCooldown + chargeTime;
	LOG("Attack charging...\n");
}

void Attack::attack()
{
	state = ATTACKING;
	activeTime = attackDuration;
	attackTrigger->setActive(true);
	GameManager::GetInstance()->getScore()->attackDone(gameObject->getParent()->getComponent<PlayerIndex>()->getIndex(), false);
	LOG("Attack!\n");
}


void Attack::quickAttack()
{
	if (cooldown <= 0.0f)
	{
		currentAttack = QUICK;
		charge(quickAttackCooldown, quickChargeTime);
	}
	else
		LOG("Attack on CD...\n");
}

void Attack::strongAttack()
{
	if (cooldown <= 0.0f)
	{
		currentAttack = STRONG;
		charge(strongAttackCooldown, strongChargeTime);
	}
	else
		LOG("Attack on CD...\n");
}
