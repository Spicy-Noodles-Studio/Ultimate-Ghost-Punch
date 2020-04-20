#include "PlayerFX.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <sstream>

REGISTER_FACTORY(PlayerFX);

PlayerFX::PlayerFX(GameObject* gameObject) : UserComponent(gameObject)
{

}

PlayerFX::~PlayerFX()
{

}

void PlayerFX::start()
{

}

void PlayerFX::update(float deltaTime)
{
	// Update the cooldown
	if (cooldown > 0.0f)
		cooldown -= deltaTime;

	/*//Attack charge time
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
	}*/
}

void PlayerFX::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		/*if (prop.first == "quickCooldown") {
			if (!(ss >> quickAttackCooldown))
				LOG("ATTACK: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "strongCooldown") {
			if (!(ss >> strongAttackCooldown))
				LOG("ATTACK: Invalid property with name \"%s\"", prop.first.c_str());
		}*/
	}
}
/*
bool Attack::strongAttack()
{
	if (cooldown <= 0.0f)
	{
		currentAttack = STRONG;
		charge(strongAttackCooldown, strongChargeTime);
		return true;
	}
	else
		LOG("Attack on CD...\n");
	return false;
}
*/