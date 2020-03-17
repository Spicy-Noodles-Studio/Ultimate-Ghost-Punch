#include "Jump.h"
#include <sstream>

Jump::Jump(GameObject* gameObject) : UserComponent(gameObject)
{
	jumpVector = { 0,0,0 };
	jumpMargin = 2;
	isGrounded = true;
	isJumping = false;
}

bool Jump::salta()
{
	if (isGrounded && !isJumping) {
		isGrounded = false;
		isJumping = true;
		jumpForce = maxForce;
	}

	if(isJumping) jumpVector = { 0,1,0 };

	return isJumping;
}

void Jump::start()
{
	rigidBody = gameObject->getComponent<RigidBody>();
}

void Jump::update(float deltaTime)
{
	if (!isGrounded && rigidBody->getTotalForce().y == 0) isGrounded = true;
	if (isGrounded && rigidBody->getTotalForce().y <= jumpMargin) isGrounded = false;

	if (isJumping && jumpVector != Vector3(0,0,0)) {
		rigidBody->addForce(jumpVector * jumpForce);
		jumpForce -= 0.5f;
		jumpVector = { 0,0,0 };
		if (jumpForce <= 0.0f) {
			jumpForce = 0.0f;
			isJumping = false;
		}
	}
}

void Jump::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "maxJump")
		{
			ss >> maxForce;
		}
	}
}