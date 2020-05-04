#include "Jump.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>

#include "PlayerAnimController.h"

#include <sstream>

REGISTER_FACTORY(Jump);

Jump::Jump(GameObject* gameObject) : UserComponent(gameObject), coyoteTime(0.5f), coyoteTimer(0.0f), grounded(false), jumping(false), rigidBody(nullptr)
{
}

Jump::~Jump()
{
}

void Jump::start()
{
	rigidBody = gameObject->getParent()->getComponent<RigidBody>();
}

void Jump::update(float deltaTime)
{
	// Manage coyote time so jumping is possible when not grounded for a certain time
	if (coyoteTimer >= 0.0f) coyoteTimer -= deltaTime;
}

void Jump::onObjectEnter(GameObject* other)
{
	bool overPlayer = other->getTag() == "Player" && other != gameObject->getParent();
	if (other->getTag() == "suelo" || overPlayer) {
		grounded = true;
		coyoteTimer = 0.0f;
		jumping = false; // Cannot be jumping if is on floor
		if (overPlayer) playersBelow++;
	}
}

void Jump::onObjectExit(GameObject* other)
{
	bool overPlayer = other->getTag() == "Player" && other != gameObject->getParent();
	if (other->getTag() == "suelo" || overPlayer) {
		grounded = false;
		if(!jumping)
			coyoteTimer = coyoteTime;
		if (overPlayer) playersBelow--;
	}
}

void Jump::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "maxForce")
		{
			setFloat(jumpForce);
		}
		else if (prop.first == "jumpDecay") {
			setFloat(jumpDecay);
		}
		else if (prop.first == "coyoteTime") {
			setFloat(coyoteTime);
		}
	}
}

void Jump::jump()
{
	if (jumping || !canJump()) return;

	// Cancel vertical velocity so impulse doesnt lose strenght
	rigidBody->setLinearVelocity(rigidBody->getLinearVelocity() * Vector3(1.0, 0.0, 1.0));
	rigidBody->addImpulse(Vector3(0.0, 1.0, 0.0) * jumpForce);
	jumping = true;
	coyoteTimer = 0.0f;

	auto animController = gameObject->getParent()->getComponent<PlayerAnimController>();
	if(animController != nullptr) animController->jumpAnimation();
}

void Jump::cancelJump()
{
	if (!jumping) return;
	Vector3 velocity = rigidBody->getLinearVelocity();
	if (velocity.y < 0.0) return; // Is falling
	velocity.y *= jumpDecay;

	rigidBody->setLinearVelocity(velocity);
	jumping = false;
}

void Jump::setJumpForce(float force)
{
	jumpForce = force;
}

void Jump::setCoyoteTime(float time)
{
	coyoteTime = time;
}

bool Jump::isGrounded()
{
	return grounded || playersBelow;
}

bool Jump::isJumping()
{
	return jumping;
}

bool Jump::canJump()
{
	return isGrounded() || coyoteTimer > 0.0f;
}