#include "GhostMovement.h"
#include <ComponentRegister.h>
#include <InputSystem.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <SoundEmitter.h>
#include <sstream>

#include "PlayerState.h"

REGISTER_FACTORY(GhostMovement);

GhostMovement::GhostMovement(GameObject* gameObject) :UserComponent(gameObject), rigidBody(nullptr), maxSpeed(2.0f)
{

}

GhostMovement::~GhostMovement()
{
}

void GhostMovement::start()
{
	rigidBody = gameObject->getComponent<RigidBody>();
	checkNull(rigidBody);
}

void GhostMovement::handleData(ComponentData* data)
{
	checkNullAndBreak(data);
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "maxSpeed")
		{
			setFloat(maxSpeed);
		}
		else
			LOG("GHOST MOVEMENT: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void GhostMovement::move(Vector3 dir)
{
	PlayerState* aux = gameObject->getComponent<PlayerState>();
	if (notNull(aux) && aux->canGhostMove())
	{
		//Character rotation
		if (dir.x != 0)
		{
			double flippedY = (dir.x > 0) ? 1 : -1;
			if(notNull(gameObject->transform)) gameObject->transform->setRotation({ 0,90 * flippedY,0 });
		}

		dir *= maxSpeed;
		if (notNull(rigidBody)) rigidBody->setLinearVelocity(dir);
	}
}

void GhostMovement::stop()
{
	checkNullAndBreak(rigidBody);

	rigidBody->setLinearVelocity(Vector3::ZERO);
	rigidBody->clearForces();
}

void GhostMovement::setSpeed(float speed)
{
	maxSpeed = speed;
}

float GhostMovement::getSpeed() const
{
	return maxSpeed;
}

bool GhostMovement::isGhostMoving() const
{
	checkNullAndBreak(rigidBody, false);
	return std::abs(rigidBody->getLinearVelocity().x) > 0.3f || std::abs(rigidBody->getLinearVelocity().y) > 0.3f;
}