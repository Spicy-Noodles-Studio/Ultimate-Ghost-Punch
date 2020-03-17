#include "GhostMovement.h"
#include <InputSystem.h>
#include <RigidBody.h>
#include <sstream>
#include <GameObject.h>

GhostMovement::GhostMovement(GameObject* g) :UserComponent(g)
{
}

void GhostMovement::start()
{
	body = gameObject->getComponent<RigidBody>();
}

void GhostMovement::move(Vector3 dir)
{
	dir *= maxSpeed;
	if (body != nullptr) body->setLinearVelocity(dir);
}

void GhostMovement::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "maxSpeed")
			ss >> maxSpeed;
		else
		{
			printf("GHOST MOVEMENT: Invalid property name \"%s\"", prop.first.c_str());
		}
	}
}

void GhostMovement::setSpeed(float speed)
{
	maxSpeed = speed;
}