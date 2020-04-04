#include "UltimateGhostPunch.h"

#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>
#include <Scene.h>
#include <Camera.h>

#include "Health.h"
#include "GhostMovement.h"

REGISTER_FACTORY(UltimateGhostPunch);

UltimateGhostPunch::UltimateGhostPunch(GameObject* gameObject) :	UserComponent(gameObject), rigidBody(nullptr), ghostMovement(nullptr), 
																	direction(0.0, 0.0, 0.0), state(State::NONE), duration(0.0f), force(0.0f),
																	ghostSpeed(0.0f), chargeSpeedMult(0.0f)
																	
{
	
}

UltimateGhostPunch::~UltimateGhostPunch()
{

}

void UltimateGhostPunch::start()
{
	rigidBody = gameObject->getComponent<RigidBody>();
	ghostMovement = gameObject->getComponent<GhostMovement>();
	if(ghostMovement != nullptr)	ghostSpeed = ghostMovement->getSpeed();
	state = State::AVAILABLE;
}

void UltimateGhostPunch::update(float deltaTime)
{
	// Update the cooldown
	if (duration > 0.0f && state == State::PUNCHING)
		duration -= deltaTime;
	else if (duration <= 0.0f)
		state = State::USED;
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
		else if (prop.first == "chargeSpeedMult") {
			if (!(ss >> chargeSpeedMult))
				LOG("ULTIMATE GHOST PUNCH: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("ULTIMATE GHOST PUNCH: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void UltimateGhostPunch::charge()
{
	state = State::CHARGING;
	if (ghostMovement != nullptr) ghostMovement->setSpeed(ghostMovement->getSpeed() * chargeSpeedMult);
}

void UltimateGhostPunch::aim(double x, double y)
{
	direction = { x, y, 0.0 };
	direction.normalize();
}

void UltimateGhostPunch::ghostPunch()
{
	if (rigidBody != nullptr) rigidBody->addImpulse(direction * force);
	if (ghostMovement != nullptr) ghostMovement->setSpeed(ghostSpeed);
	state = State::PUNCHING;
}

const UltimateGhostPunch::State& UltimateGhostPunch::getState()
{
	return state;
}

const Vector3& UltimateGhostPunch::getDirection()
{
	return direction;
}
