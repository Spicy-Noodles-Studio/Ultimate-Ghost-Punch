#include "Jump.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <SoundEmitter.h>
#include <sstream>

#include "PlayerState.h"

REGISTER_FACTORY(Jump);

Jump::Jump(GameObject* gameObject) : UserComponent(gameObject), jumpForce(0), jumpDecay(0), coyoteTime(0.5f), coyoteTimer(0.0f),
									 playersBelow(0), grounded(false), jumping(false), rigidBody(nullptr), parent(nullptr), landed(0), jumped(0)
{

}

Jump::~Jump()
{

}

void Jump::start()
{
	landed = false;
	parent = gameObject->getParent();
	checkNullAndBreak(parent);
	
	rigidBody = parent->getComponent<RigidBody>();
	checkNull(rigidBody);
}

void Jump::update(float deltaTime)
{
	// Manage coyote time so jumping is possible when not grounded for a certain time
	if (coyoteTimer >= 0.0f)
		coyoteTimer -= deltaTime;
}

void Jump::postUpdate(float deltaTime)
{
	if (landed > 0)landed--;;
	if (jumped > 0)jumped--;
}

void Jump::onObjectEnter(GameObject* other)
{
	bool isFloor = other != nullptr && other->getTag() == "suelo";
	bool isPlayer = other != nullptr && other->getTag() == "Player" && parent != nullptr && other != parent;

	if (isFloor || isPlayer)
	{
		if (isFloor) {
			grounded = true;
			landed = 2;
		}

		coyoteTimer = 0.0f;
		jumping = false; // Cannot be jumping if is on floor

		if (isPlayer) {
			if (!playersBelow) landed = 2;
			playersBelow++;
		}
	}
}

void Jump::onObjectExit(GameObject* other)
{
	bool isFloor = other != nullptr && other->getTag() == "suelo";
	bool isPlayer = other != nullptr && other->getTag() == "Player" && parent != nullptr && other != parent;

	if (isFloor || isPlayer)
	{
		if (isFloor)
			grounded = false;

		if (!jumping)
			coyoteTimer = coyoteTime;

		if (isPlayer)
			playersBelow--;
	}
}

void Jump::handleData(ComponentData* data)
{
	if (data == nullptr) return;
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "maxForce")
		{
			setFloat(jumpForce);
		}
		else if (prop.first == "jumpDecay")
		{
			setFloat(jumpDecay);
		}
		else if (prop.first == "coyoteTime")
		{
			setFloat(coyoteTime);
		}
	}
}

void Jump::jump()
{
	if (parent == nullptr) return;
	PlayerState* aux = parent->getComponent<PlayerState>();

	if (jumping || !canJump() || rigidBody == nullptr || (aux != nullptr && !aux->canJump())) return;

	// Cancel vertical velocity so impulse doesnt lose strenght
	rigidBody->setLinearVelocity(rigidBody->getLinearVelocity() * Vector3(1.0, 0.0, 1.0));
	rigidBody->addImpulse(Vector3(0.0, 1.0, 0.0) * jumpForce);
	jumping = true;
	jumped = 2;
	coyoteTimer = 0.0f;
}

void Jump::cancelJump()
{
	if (!jumping || rigidBody == nullptr) return;
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

bool Jump::isGrounded() const
{
	return grounded || playersBelow;
}

bool Jump::isAbovePlayer() const
{
	return playersBelow > 0;
}

bool Jump::isJumping() const
{
	return jumping;
}

bool Jump::isFalling() const
{
	return rigidBody != nullptr && rigidBody->getLinearVelocity().y <= -1.0f;
}

bool Jump::canJump() const
{
	return grounded || playersBelow || coyoteTimer > 0.0f;
}

bool Jump::hasLanded() const
{
	return landed > 0;
}

bool Jump::hasJumped() const
{
	return jumped > 0;
}