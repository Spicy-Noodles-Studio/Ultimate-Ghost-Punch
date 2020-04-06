#include "GhostMovement.h"

#include <InputSystem.h>
#include <RigidBody.h>
#include <sstream>
#include <GameObject.h>

#include "ComponentRegister.h"

REGISTER_FACTORY(GhostMovement);

GhostMovement::GhostMovement(GameObject* g) :UserComponent(g), rigidbody(nullptr), maxSpeed(2.0f)
{
}

void GhostMovement::start()
{
	rigidbody = gameObject->getComponent<RigidBody>();
}

void GhostMovement::move(Vector3 dir)
{
	dir *= maxSpeed;
	if (rigidbody != nullptr) rigidbody->setLinearVelocity(dir);
}

void GhostMovement::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "maxSpeed") {
			if (!(ss >> maxSpeed))
				LOG("GHOST MOVEMENT: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("GHOST MOVEMENT: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void GhostMovement::setSpeed(float speed)
{
	maxSpeed = speed;
}

float GhostMovement::getSpeed() const
{
	return maxSpeed;
}
