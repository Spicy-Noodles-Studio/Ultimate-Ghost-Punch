#include "Dodge.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>

#include "PlayerState.h"
#include "PlayerAnimController.h"

REGISTER_FACTORY(Dodge);

Dodge::Dodge(GameObject* gameObject) : UserComponent(gameObject), rigidBody(nullptr), state(IDLE), playerGravity(0, -10, 0), cooldown(1.0f), force(10.0f), time(0.0f), duration(0.25f), atenuation(0.3f)
{

}

Dodge::~Dodge()
{

}

void Dodge::start()
{
	rigidBody = gameObject->getComponent<RigidBody>();

	if (rigidBody != nullptr)
		playerGravity = rigidBody->getGravity();
}

void Dodge::update(float deltaTime)
{
	if (state != State::IDLE)
	{
		if (time > 0.0f)
			time -= deltaTime;

		if (time <= 0.0f)
		{
			if (state == State::DODGING)
				endDodge();
			else
				state = State::IDLE;
		}
	}
}

void Dodge::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "force")
		{
			if (!(ss >> force))
				LOG("DODGE: Invalid value for property %s", prop.first.c_str());
		}
		else if (prop.first == "cooldown")
		{
			if (!(ss >> cooldown))
				LOG("DODGE: Invalid value for property %s", prop.first.c_str());
		}
		else if (prop.first == "duration")
		{
			if (!(ss >> duration))
				LOG("DODGE: Invalid value for property %s", prop.first.c_str());
		}
		else if (prop.first == "atenuation")
		{
			if (!(ss >> atenuation))
				LOG("DODGE: Invalid value for property %s", prop.first.c_str());
		}
		else
			LOG("DODGE: Invalid property name %s", prop.first.c_str());
	}
}

void Dodge::dodge()
{
	PlayerState* aux = gameObject->getComponent<PlayerState>();

	if (state == State::IDLE && aux->canDodge())
	{
		Vector3 dir = Vector3::ZERO;
		dir.x = (gameObject->transform->getRotation().y > 0) ? 1 : -1;

		if (rigidBody != nullptr)
		{
			rigidBody->setGravity({ 0,0,0 });
			rigidBody->setLinearVelocity({ 0,0,0 });
			rigidBody->addImpulse(dir * force);
		}

		state = State::DODGING;
		time = duration;

		PlayerAnimController* anim = gameObject->getComponent<PlayerAnimController>();
		if (anim != nullptr) anim->dashAnimation();
	}
}

void Dodge::endDodge()
{
	state = State::CD;
	time = cooldown;

	if (rigidBody != nullptr)
	{
		rigidBody->setLinearVelocity(rigidBody->getLinearVelocity() * atenuation);
		rigidBody->setGravity(playerGravity);
	}
}

bool Dodge::isDodging()
{
	return state == State::DODGING;
}