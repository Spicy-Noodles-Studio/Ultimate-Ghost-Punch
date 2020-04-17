#include "Dodge.h"

#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>
#include "Movement.h"
#include "ComponentRegister.h"

REGISTER_FACTORY(Dodge);

Dodge::Dodge(GameObject* gameObject) : UserComponent(gameObject), cooldown(1.0f), force(10.0f), cd(0.0f)
{
}

void Dodge::update(float deltaTime)
{
	// Update the cooldown
	if (cd > 0.0f)
		cd -= deltaTime;

	if (cd <= 0.0f)
		state = IDLE;

}

void Dodge::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);
		
		if (prop.first == "force") {
			if (!(ss >> force))
				LOG("DODGE: Invalid value for property %s", prop.first.c_str());
		}
		else if (prop.first == "cooldown") {
			if (!(ss >> cooldown))
				LOG("DODGE: Invalid value for property %s", prop.first.c_str());
		}
		else
			LOG("DODGE: Invalid property name %s", prop.first.c_str());
	}
}

bool Dodge::dodge()
{
	if (state == IDLE) {
		Vector3 dir = Vector3();
		dir.x = (gameObject->transform->getRotation().y > 0) ? 1 : -1;

		RigidBody* rb = gameObject->getComponent<RigidBody>();
		if(rb!=nullptr) rb->addImpulse(dir * force);

		cd = cooldown;
		state = CD;

		return true;
	}
	return false;
}


