#include "UltimateGhostPunch.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <Scene.h>
#include <Camera.h>
#include <MathUtils.h>
#include <sstream>

#include "Health.h"
#include "GhostMovement.h"
#include "PlayerAnimController.h"

REGISTER_FACTORY(UltimateGhostPunch);

UltimateGhostPunch::UltimateGhostPunch(GameObject* gameObject) : UserComponent(gameObject), rigidBody(nullptr), ghostMovement(nullptr), anim(nullptr),
direction(Vector3::ZERO), state(State::NONE), used(false), duration(0.0f), force(0.0f), ghostSpeed(0.0f), chargeSpeedMult(0.0f)
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

	if (ghostMovement != nullptr)
		ghostSpeed = ghostMovement->getSpeed();

	state = State::AVAILABLE;
}

void UltimateGhostPunch::preUpdate(float deltaTime)
{
	if (state == State::SUCCESS || state == State::USED)
	{
		Vector3 rotation = gameObject->transform->getRotation();
		rotation.z = 0.0;
		gameObject->transform->setRotation(rotation);
		return;
	}

	// Update the cooldown
	if (duration > 0.0f && state == State::PUNCHING)
		duration -= deltaTime;
	else if (state != State::USED && duration <= 0.0f)
	{
		state = State::USED;
		if (anim != nullptr)  anim->notLoopAnimation("UGPFail");
	}
}

void UltimateGhostPunch::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "force")
		{
			setFloat(force);
		}
		else if (prop.first == "duration")
		{
			setFloat(duration);
		}
		else if (prop.first == "chargeSpeedMult")
		{
			setFloat(chargeSpeedMult);
		}
		else
			LOG("ULTIMATE GHOST PUNCH: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void UltimateGhostPunch::charge()
{
	if (state == State::AVAILABLE)
		state = State::CHARGING;
	if (ghostMovement != nullptr)
		ghostMovement->setSpeed(ghostMovement->getSpeed() * chargeSpeedMult);

	if (anim != nullptr)
		anim->chargingGhostAnimation();
}

void UltimateGhostPunch::aim(double x, double y)
{
	if (x == 0 && y == 0) return;
	if (state != State::CHARGING) return;

	direction = { x, y, 0.0 };
	direction.normalize();

	float flippedX = direction.x >= 0 ? 1.0f : -1.0f;
	float flippedY = direction.y >= 0 ? 1.0f : -1.0f;

	if (direction.x != 0)
	{
		float angle = acos(direction.dot(Vector3::RIGHT * flippedX));
		Vector3 finalDirection = Vector3(0.0, 90.0f * flippedX, angle * RAD_TO_DEG * flippedX * flippedY);
		gameObject->transform->setRotation(finalDirection);
	}
}

void UltimateGhostPunch::ghostPunch()
{
	if (state != State::CHARGING) return;

	if (rigidBody != nullptr) rigidBody->addImpulse(direction * force);

	if (ghostMovement != nullptr) ghostMovement->setSpeed(ghostSpeed);

	state = State::PUNCHING;
	used = true;

	if (anim != nullptr) anim->punchingGhostAnimation();
}

const UltimateGhostPunch::State& UltimateGhostPunch::getState()
{
	return state;
}

const Vector3& UltimateGhostPunch::getDirection()
{
	return direction;
}

bool UltimateGhostPunch::isPunching() const
{
	return state == State::PUNCHING;
}

bool UltimateGhostPunch::isAiming() const
{
	return state == State::CHARGING;
}

bool UltimateGhostPunch::isUsed() const
{
	return used;
}

bool UltimateGhostPunch::punchSuccess() const
{
	return state == State::SUCCESS;
}

void UltimateGhostPunch::punchSucceeded()
{
	state = State::SUCCESS;
}