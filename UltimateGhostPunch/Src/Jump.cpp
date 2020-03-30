#include "Jump.h"

#include <sstream>
#include <GameObject.h>
#include <RigidBody.h>

#include "ComponentRegister.h"

REGISTER_FACTORY(Jump);

Jump::Jump(GameObject* gameObject) : UserComponent(gameObject)
{
	jumpVector = { 0,0,0 };
	jumpMargin = 2;
	isGrounded = false;
	isJumping = false;
}

bool Jump::salta()
{
	if (isGrounded && !isJumping) {
		isJumping = true;
		jumpForce = maxForce;
	}

	if (isJumping) jumpVector = { 0,1,0 };

	return isJumping;
}

void Jump::start()
{
	rigidBody = gameObject->getParent()->getComponent<RigidBody>();
}

void Jump::update(float deltaTime)
{
	if (isJumping) {
		if (jumpVector != Vector3(0, 0, 0)) {
			rigidBody->addImpulse(jumpVector * jumpForce);
			jumpForce -= jumpDecay;

			jumpVector = { 0,0,0 };
			if (jumpForce <= 0.0f) {
				jumpForce = 0.0f;
				isJumping = false;
			}
		}
		else {
			isJumping = false;
		}
	}
}

void Jump::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "maxForce")
		{
			if(!(ss >> maxForce))
				LOG("JUMP: wrong value for property %s.\n", prop.first.c_str());
		}
		else if (prop.first == "jumpDecay") {
			if(!(ss >> jumpDecay))
				LOG("JUMP: wrong value for property %s.\n", prop.first.c_str());
		}
	}
}

void Jump::onObjectEnter(GameObject* other)
{
	if (other->getTag() == "suelo") {
		isGrounded = true;
	}
}

void Jump::onObjectExit(GameObject* other)
{
	if (other->getTag() == "suelo") {
		isGrounded = false;
	}
}
