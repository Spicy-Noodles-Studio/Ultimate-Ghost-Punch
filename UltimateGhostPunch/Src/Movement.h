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

protected:
	virtual void start();
	virtual void handleData(ComponentData* data);

public:
	Movement(GameObject* gameObject);
	virtual ~Movement();

	void move(Vector3 dir);
	void stop(); // Stops the object's physic body movement and clear its forces
	void stopHorizontal(); // Stops the player from moving in the X axis, used for the AI

	void setSpeed(float speed);
	float getSpeed() const;

	bool isMoving() const;
};

#endif