#include "Jump.h"

#include <sstream>
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>


REGISTER_FACTORY(Jump);

Jump::Jump(GameObject* gameObject) : UserComponent(gameObject)
{
	jumpVector = { 0,0,0 };
	coyoteTime = 2;
	grounded = false;
	jumping = false;
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
	if (coyoteTime >= 0.0f) coyoteTime -= deltaTime;
}

void Jump::fixedUpdate(float deltaTime)
{
	if (jumping) {
		if (jumpVector != Vector3(0, 0, 0)) {
			rigidBody->addImpulse(jumpVector * jumpForce);
			jumpForce -= jumpDecay;

			jumpVector = { 0,0,0 };
			if (jumpForce <= 0.0f) {
				jumpForce = 0.0f;
				jumping = false;
			}
		}
		else {
			jumping = false;
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
		grounded = true;
	}
}

void Jump::onObjectExit(GameObject* other)
{
	if (other->getTag() == "suelo") {
		grounded = false;
	}
}

bool Jump::jump()
{
	// TODO: cambiar para que aplique el impulso aqui
	if (grounded && !jumping) {
		jumping = true;
		jumpForce = maxForce;
	}

	if (jumping) jumpVector = { 0,1,0 };

	return jumping;
}

void Jump::cancelJump()
{
	if (!jumping) return;
	float jumpAttenuation = 0.5f;
	Vector3 velocity = rigidBody->getLinearVelocity();
	velocity.y *= jumpAttenuation;

	rigidBody->setLinearVelocity(velocity);
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
	return grounded;
}

bool Jump::isJumping()
{
	return jumping;
}

bool Jump::canJump()
{
	return grounded || coyoteTime > 0.0f;
}
