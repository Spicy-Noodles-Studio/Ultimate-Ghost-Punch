#include "UltimateGhostPunch.h"

#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>
#include <Scene.h>
#include <Camera.h>

#include "Health.h"
#include "ComponentRegister.h"

REGISTER_FACTORY(UltimateGhostPunch);

UltimateGhostPunch::UltimateGhostPunch(GameObject* gameObject) : UserComponent(gameObject), body(nullptr)
{
}

void UltimateGhostPunch::start()
{
	body = gameObject->getComponent<RigidBody>();
	state = AVAILABLE;
}

void UltimateGhostPunch::charge()
{
	state = CHARGING;
	if (body != nullptr) body->setLinearVelocity({ 0,0,0 });
}

void UltimateGhostPunch::aim(double x, double y)
{
	dir = { x, y, 0 };
	dir.normalize();
}

void UltimateGhostPunch::ghostPunch()
{
	if (body != nullptr) body->addImpulse(dir * force);
	state = PUNCHING;
}

void UltimateGhostPunch::update(float deltaTime)
{
	// Update the cooldown
	if (duration > 0.0f && state == PUNCHING)
		duration -= deltaTime;
	else if (duration <= 0.0f)
		state = USED;
}

State UltimateGhostPunch::getState()
{
	return state;
}

const Vector3& UltimateGhostPunch::getDir()
{
	return dir;
}

void UltimateGhostPunch::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "force") {
			if (!(ss >> force))
				LOG("ULTIMATE GHOST PUNCH: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "duration") {
			if (!(ss >> duration))
				LOG("ULTIMATE GHOST PUNCH: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("ULTIMATE GHOST PUNCH: Invalid property name \"%s\"", prop.first.c_str());
	}
}
