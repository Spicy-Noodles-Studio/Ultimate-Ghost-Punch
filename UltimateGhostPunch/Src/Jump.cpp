#include "Jump.h"
#include <sstream>

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
		printf("salta\n");
		jumpForce = maxForce;
	}

	if(isJumping) jumpVector = { 0,1,0 };

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
			ss >> maxForce;
		}
		if (prop.first == "jumpDecay") {
			ss >> jumpDecay;
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
