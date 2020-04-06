#pragma once

#ifndef GHOSTMOVE_H
#define GHOSTMOVE_H

#include <UserComponent.h>

class RigidBody;

class GhostMovement : public UserComponent
{
private:
	float maxSpeed;
	RigidBody* rigidbody;
public:
	GhostMovement(GameObject* g);

	virtual void start();
	void move(Vector3 dir);
	virtual void handleData(ComponentData* data);

	void setSpeed(float speed);
	float getSpeed() const;
};

#endif 