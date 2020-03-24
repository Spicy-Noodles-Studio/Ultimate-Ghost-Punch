#include "Dodge.h"

#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>
#include "Movement.h"


Dodge::Dodge(GameObject* gameObject) : UserComponent(gameObject)
{
}

void Dodge::start()
{
	movement = gameObject->getComponent<Movement>();
}

void Dodge::update(float deltaTime)
{
	// Update the cooldown
	if (cd > 0.0f)
	{
		cd -= deltaTime;
	}

	if (state == DODGING_R) {
		duration += deltaTime;
		movement->move(Vector3(VELOCITY,0,0));
	}else if (state == DODGING_L) {
		duration += deltaTime;
		movement->move(Vector3(-VELOCITY, 0, 0));
	}


	if (duration >= DURATION) {
		duration = 0.0f;
		state = CD;
	}

	if (cd <= 0.0f) {
		state = IDLE;
	}

}


void Dodge::dodgeR()
{
	if (state == IDLE) { 
		state = DODGING_R; 
		cd = COOLDOWN + DURATION;
	}
}

void Dodge::dodgeL()
{
	if (state == IDLE) { 
		state = DODGING_L; 
		cd = COOLDOWN + DURATION;
	}
}




void Dodge::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		/*if (prop.first == "quickCooldown") {
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
			LOG("ATTACK: Invalid property name \"%s\"", prop.first.c_str());*/
	}
}


