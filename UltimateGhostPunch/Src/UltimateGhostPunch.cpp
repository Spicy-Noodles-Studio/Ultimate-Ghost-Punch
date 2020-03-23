#include "UltimateGhostPunch.h"

#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>
#include <Scene.h>
#include <Camera.h>
#include "Health.h"


UltimateGhostPunch::UltimateGhostPunch(GameObject* gameObject) : UserComponent(gameObject)
{
}

void UltimateGhostPunch::start()
{
	body = gameObject->getComponent<RigidBody>();
}

void UltimateGhostPunch::ghostPunch(const Vector3 & dir)
{
	
	Vector3 dirr = dir * force;
	if (body != nullptr)body->addImpulse(dirr);
	punching = true;
	
	available = false;
}

void UltimateGhostPunch::update(float deltaTime)
{
	// Update the cooldown
	if (duration > 0.0f && punching)
	{
		duration -= deltaTime;
	}
	else {
		
		punching = false;
	}

	//Attack charge time
	

	

}

bool UltimateGhostPunch::isAvailable()
{
	return available;
}

bool UltimateGhostPunch::isPunching()
{
	return punching;
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
				LOG("ATTACK: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("ULTIMATE GHOST PUNCH: Invalid property name \"%s\"", prop.first.c_str());
	}
}
