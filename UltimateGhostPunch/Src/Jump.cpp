#include "Jump.h"

#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>

#include "PlayerState.h"

REGISTER_FACTORY(Jump);

Jump::Jump(GameObject* gameObject) : UserComponent(gameObject), jumpForce(0), jumpDecay(0), coyoteTime(0.5f), coyoteTimer(0.0f),
playersBelow(0), grounded(false), jumping(false), rigidBody(nullptr), parent(nullptr), landed(0), jumped(0)
{

}

Jump::~Jump()
{
	rigidBody = nullptr;
	parent = nullptr;
}

void Jump::start()
{
	checkNullAndBreak(gameObject);

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
	if (landed > 0) landed--;;
	if (jumped > 0) jumped--;
	if (grounded) jumping = false;
}

void Jump::onObjectEnter(GameObject* other)
{
	checkNullAndBreak(other);

	bool isFloor = other->getTag() == "suelo";
	bool isPlayer = other->getTag() == "Player" && parent != nullptr && other != parent;

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
	checkNullAndBreak(other);

	bool isFloor = other->getTag() == "suelo";
	bool isPlayer = other->getTag() == "Player" && notNull(parent) && other != parent;

	if (isFloor || isPlayer)
	{
		if (isFloor)
			grounded = false;

		if (!jumping)
			coyoteTimer = coyoteTime;

		if (isPlayer)
		{
			playersBelow--;
			if (playersBelow < 0)
				playersBelow = 0;
		}
	}
}

void Jump::handleData(ComponentData* data)
{
	checkNullAndBreak(data);
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
	checkNullAndBreak(parent);
	PlayerState* aux = parent->getComponent<PlayerState>();

	if (jumping || !canJump() || !notNull(rigidBody) || (notNull(aux) && !aux->canJump())) return;

	// Cancel vertical velocity so impulse doesnt lose strenght
	rigidBody->setLinearVelocity(rigidBody->getLinearVelocity() * Vector3(1.0, 0.0, 1.0));
	rigidBody->addImpulse(Vector3::UP * jumpForce);
	jumping = true;
	jumped = 2;
	coyoteTimer = 0.0f;
}

void Jump::cancelJump()
{
	if (!jumping || !notNull(rigidBody)) return;
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
	return notNull(rigidBody) && rigidBody->getLinearVelocity().y <= -1.0f;
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