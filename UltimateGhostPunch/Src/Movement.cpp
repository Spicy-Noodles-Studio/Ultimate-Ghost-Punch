#include "Movement.h"
#include <InputSystem.h>
#include <sstream>

Movement::Movement(GameObject* gameObject) : UserComponent(gameObject)
{
	rigidBody = gameObject->getComponent<RigidBody>();
}

void Movement::update(float deltaTime)
{
	UserComponent::update(deltaTime);

	Vector3 dir = Vector3(0, 0, 0);

	if (InputSystem::GetInstance()->isKeyPressed("A"))
		dir = Vector3(-1, 0, 0);
	else if (InputSystem::GetInstance()->isKeyPressed("D"))
		dir = Vector3(1, 0, 0);

	move(dir * force);
}

void Movement::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "force")
		{
			ss >> force;
		}
	}
}

void Movement::move(Vector3 force)
{
	rigidBody->addForce(force);
}