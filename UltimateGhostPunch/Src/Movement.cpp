#include "Movement.h"
#include <sstream>
#include <RigidBody.h>
#include <GameObject.h>

Movement::Movement(GameObject* gameObject) : UserComponent(gameObject)
{

}

void Movement::move(Vector3 dir)
{
	if(rigidBody != nullptr) rigidBody->addForce(dir * force);
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