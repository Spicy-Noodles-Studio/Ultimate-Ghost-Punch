#pragma once
#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <UserComponent.h>

class RigidBody;

class Movement : public UserComponent
{
private:
	RigidBody* rigidBody;
	float speed, maxVelocity;

public:
	Movement(GameObject* gameObject);
	virtual ~Movement();

	virtual void start();
	virtual void handleData(ComponentData* data);

	void move(Vector3 dir);
	void stop(); // Stops the object's physic body's movement and clear its forces

	void setSpeed(float speed);
	float getSpeed() const;
};

#endif