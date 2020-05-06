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
}

void GhostMovement::handleData(ComponentData* data)
{
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
	if (aux != nullptr && aux->canGhostMove()) {
		//Character rotation
		if (dir.x != 0)
			gameObject->transform->setRotation({ 0,90 * dir.x,0 });

		dir *= maxSpeed;
		if (rigidBody != nullptr)
			rigidBody->setLinearVelocity(dir);

		//SoundEmitter* soundEmitter = gameObject->getComponent<SoundEmitter>();
		//if (soundEmitter != nullptr) soundEmitter->playSound("heartBeat");
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