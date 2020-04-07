#include "Dodge.h"

#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>
#include "Movement.h"
#include "ComponentRegister.h"

REGISTER_FACTORY(Dodge);

Dodge::Dodge(GameObject* gameObject) : UserComponent(gameObject)
{
	COOLDOWN = 1.0f;
	force = 3000.0f;

	cd = 0.0f;


	state = IDLE;

	vIzq = { -1,0,0 };
	vDer = { 1,0,0 };

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


	if (cd <= 0.0f) {
		state = IDLE;
	}

}


void Dodge::dodgeR()
{
	if (state == IDLE) { 
		gameObject->getComponent<RigidBody>()->addForce(vDer * force);
		cd = COOLDOWN;
		state = CD;
	}
}

void Dodge::dodgeL()
{
	if (state == IDLE) { 
		gameObject->getComponent<RigidBody>()->addForce(vIzq * force);
		cd = COOLDOWN;
		state = CD;
	}
}




void Dodge::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		
	}
}


