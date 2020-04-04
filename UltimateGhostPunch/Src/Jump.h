#pragma once
#ifndef JUMP_H
#define JUMP_H

#include <UserComponent.h>

class RigidBody;

class Jump : public UserComponent
{
private:
	RigidBody* rigidBody;

	Vector3 jumpVector;
	float jumpForce;
	float maxForce;
	float jumpDecay;

	bool grounded;
	bool jumping;

	float coyoteTime; // Margen que se da para saltar aún cuando se está en caída, el "coyote time"

public:
	Jump(GameObject* gameObject);
	virtual ~Jump();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void fixedUpdate(float deltaTime);
	virtual void handleData(ComponentData* data);
	virtual void onObjectEnter(GameObject* other);
	virtual void onObjectExit(GameObject* other);

	bool jump();
	void cancelJump();
	void setJumpForce(float force);
	void setCoyoteTime(float time);
	bool isGrounded();
	bool isJumping();
	bool canJump();
};

#endif