#include "Movement.h"
#include <sstream>

Movement::Movement(GameObject* gameObject) : UserComponent(gameObject)
{

}

void Movement::move(Vector3 dir)
{
	rigidBody->addForce(dir * force);
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

		if (prop.first == "force")
		{
			ss >> force;
		}
	}
}