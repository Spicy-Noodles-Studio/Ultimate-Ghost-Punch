#include "GhostMovement.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <InputSystem.h>
#include <RigidBody.h>
#include <sstream>

REGISTER_FACTORY(GhostMovement);

GhostMovement::GhostMovement(GameObject* g) :UserComponent(g)
{

}

GhostMovement::~GhostMovement()
{

}

void GhostMovement::start()
{
	rigidBody = gameObject->getComponent<RigidBody>();
}

void GhostMovement::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "maxSpeed")
		{
			if (!(ss >> maxSpeed))
				LOG("GHOST MOVEMENT: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("GHOST MOVEMENT: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void GhostMovement::move(Vector3 dir)
{
	dir *= maxSpeed;
	if (rigidBody != nullptr) rigidBody->setLinearVelocity(dir);
}

void GhostMovement::setSpeed(float speed)
{
	maxSpeed = speed;
}

float GhostMovement::getSpeed() const
{
	return maxSpeed;
}