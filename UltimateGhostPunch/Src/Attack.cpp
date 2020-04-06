#include "Attack.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>

#include "Health.h"

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
	if (other->getTag() == "Player" && other != gameObject->getParent() && state == ATTACKING) // If it hits a player different than myself
	{
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

		Health* enemyHealth = other->getComponent<Health>();
		if (enemyHealth != nullptr) enemyHealth->receiveDamage(damage);
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