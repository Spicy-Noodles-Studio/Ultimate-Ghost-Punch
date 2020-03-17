#pragma once
#ifndef JUMP_H
#define JUMP_H

#include <UserComponent.h>
#include <GameObject.h>
#include <RigidBody.h>

class Jump : public UserComponent
{
private:
	RigidBody* rigidBody;

	Vector3 jumpVector;
	float jumpForce;

	bool isGrounded;
	bool isJumping;

	int jumpMargin; ///Margen que se da para saltar aún cuando se está en caída, el "coyote time"

public:
	Jump(GameObject* gameObject);

	bool salta();
	
	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

	void setJumpForce(float force) { jumpForce = force; }
	void setJumpMargin(int margin) { jumpMargin = margin; }
};

#endif