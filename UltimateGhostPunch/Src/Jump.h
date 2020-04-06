#pragma once
#ifndef JUMP_H
#define JUMP_H

#include <UserComponent.h>

class RigidBody;

class Jump : public UserComponent
{
private:
	RigidBody* rigidBody;

	float jumpForce;	// Force used to add impulse
	float jumpDecay;	// Rate of attenuation when jump is cancelled
	float coyoteTime;	// Extra time when leaving a platform so jumping is still able
	float coyoteTimer;

	bool grounded;		// Only true when sensor detects collision with floor
	bool jumping;		

public:
	Jump(GameObject* gameObject);
	virtual ~Jump();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void onObjectEnter(GameObject* other);
	virtual void onObjectExit(GameObject* other);
	virtual void handleData(ComponentData* data);

	void jump();
	void cancelJump();
	void setJumpForce(float force);
	void setCoyoteTime(float time);
	bool isGrounded();
	bool isJumping();
	bool canJump();
};

#endif