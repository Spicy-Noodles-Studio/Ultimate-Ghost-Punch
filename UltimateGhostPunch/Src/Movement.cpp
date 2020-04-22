#include "Movement.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>
#include <MathUtils.h>

REGISTER_FACTORY(Movement);

Movement::Movement(GameObject* gameObject) : UserComponent(gameObject), maxVelocity(10.0f), speed(75.0f)
{

}

Movement::~Movement()
{

}

void Movement::move(Vector3 dir)
{
	if(rigidBody != nullptr &&  std::abs(rigidBody->getLinearVelocity().x) < maxVelocity)
		rigidBody->addForce(dir * speed);

	//Character rotation
	if (dir.x != 0)
		gameObject->transform->setRotation({ 0,90 * dir.x,0 });
}

void Movement::stop()
{
	if (rigidBody != nullptr)
	{
		rigidBody->setLinearVelocity({0,rigidBody->getLinearVelocity().y,0});
		rigidBody->clearForces();
	}
}

void Movement::start()
{
	rigidBody = gameObject->getComponent<RigidBody>();
}

void Movement::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "speed")
		{
			if(!(ss >> speed))
				LOG("MOVEMENT: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "maxVelocity")
		{
			if (!(ss >> maxVelocity))
				LOG("MOVEMENT: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("MOVEMENT: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void Movement::setSpeed(float spd)
{
	speed = spd;
}

float Movement::getSpeed() const
{
	return speed;
}