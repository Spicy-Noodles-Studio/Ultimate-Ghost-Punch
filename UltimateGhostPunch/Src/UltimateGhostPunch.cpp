#include "UltimateGhostPunch.h"

#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <Scene.h>
#include <Camera.h>
#include <sstream>

#include "Health.h"
#include "GhostMovement.h"
#include "PlayerAnimController.h"

REGISTER_FACTORY(UltimateGhostPunch);

UltimateGhostPunch::UltimateGhostPunch(GameObject* gameObject) :	UserComponent(gameObject), rigidBody(nullptr), ghostMovement(nullptr), 
																	direction(0.0, 0.0, 0.0), state(State::NONE), duration(0.0f), force(0.0f),
																	ghostSpeed(0.0f), chargeSpeedMult(0.0f), anim(nullptr)
																	
{
	
}

UltimateGhostPunch::~UltimateGhostPunch()
{

}

void UltimateGhostPunch::start()
{
	rigidBody = gameObject->getComponent<RigidBody>();
	ghostMovement = gameObject->getComponent<GhostMovement>();
	anim = gameObject->getComponent<PlayerAnimController>();
	if(ghostMovement != nullptr)	ghostSpeed = ghostMovement->getSpeed();
	state = State::AVAILABLE;
}

void UltimateGhostPunch::preUpdate(float deltaTime)
{
	if (state == State::SUCCESS) return;

	// Update the cooldown
	if (duration > 0.0f && state == State::PUNCHING)
		duration -= deltaTime;
	else if (state != State::USED && duration <= 0.0f)
	{
		state = State::USED;
		if (anim != nullptr)  anim->notLoopAnimation("UGPFail");
	}
}

void UltimateGhostPunch::update(float deltaTime)
{
	
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
	if (anim != nullptr)  anim->chargingGhostAnimation();
}

void UltimateGhostPunch::aim(double x, double y)
{
	direction = { x, y, 0.0 };
	direction.normalize();
	if (direction.x != 0)
		gameObject->transform->setRotation({ 0,90 * double(direction.x >= 0 ? 1 : -1),0 });
}

void UltimateGhostPunch::ghostPunch()
{
	if (rigidBody != nullptr) rigidBody->addImpulse(direction * force);
	if (ghostMovement != nullptr) ghostMovement->setSpeed(ghostSpeed);
	state = State::PUNCHING;
	if (anim != nullptr)  anim->punchingGhostAnimation();
}

const UltimateGhostPunch::State& UltimateGhostPunch::getState()
{
	return state;
}

const Vector3& UltimateGhostPunch::getDirection()
{
	return direction;
}

bool UltimateGhostPunch::isPunching()
{
	return state == State::PUNCHING;
}

void UltimateGhostPunch::punchSucceeded()
{
	state = State::SUCCESS;
}
