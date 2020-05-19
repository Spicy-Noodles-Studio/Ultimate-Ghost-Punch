#include "Movement.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>

#include "PlayerState.h"

REGISTER_FACTORY(Movement);

Movement::Movement(GameObject* gameObject) : UserComponent(gameObject), rigidBody(nullptr), speed(0)
{

}

Movement::~Movement()
{

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
			setFloat(speed);
		}
		else
			LOG("MOVEMENT: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void Movement::move(Vector3 dir)
{
	PlayerState* aux = gameObject->getComponent<PlayerState>();
	if (aux != nullptr && aux->canMove()) {
		if (rigidBody != nullptr)
			rigidBody->addForce(dir * speed);

		//Character rotation
		if (dir.x != 0)
			gameObject->transform->setRotation({ 0,90 * dir.x,0 });
	}
}

void Movement::stop()
{
	if (rigidBody != nullptr)
	{
		rigidBody->setLinearVelocity({0,0,0});
		rigidBody->clearForces();
	}
}

void Movement::setSpeed(float speed)
{
	this->speed = speed;
}

float Movement::getSpeed() const
{
	return speed;
}

bool Movement::isMoving() const
{
	if (rigidBody == nullptr) return false;
	return std::abs(rigidBody->getLinearVelocity().x) > 0.3f;
}
