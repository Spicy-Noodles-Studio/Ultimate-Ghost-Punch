#include "GhostMovement.h"
#include <InputSystem.h>
#include <sstream>

GhostMovement::GhostMovement(GameObject* g) :UserComponent(g)
{
}

void GhostMovement::awake()
{
	body = gameObject->getComponent<RigidBody>();
}

void GhostMovement::move(Vector3 dir)
{
	dir *= maxSpeed;
	body->setLinearVelocity(dir);
}

void GhostMovement::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "maxSpeed") {
			float speed; ss >> speed;
			maxSpeed=speed;
		}
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
/*
void GhostMovement::update(float deltaTime)
{
	//UserComponent::update(deltaTime);
	

	Vector3 mov = Vector3(0, 0, 0);
	if (InputSystem::GetInstance()->isKeyPressed("W")) {
		mov += Vector3(0, 1, 0);
	}
	if (InputSystem::GetInstance()->isKeyPressed("A")) {
		mov+=Vector3(-1, 0, 0);
	}
	if (InputSystem::GetInstance()->isKeyPressed("S"))
	{
		mov+=Vector3(0, -1, 0);
	}
	if (InputSystem::GetInstance()->isKeyPressed("D"))
	{
		mov+=Vector3(1, 0, 0);
	}
		move(mov);
}
*/