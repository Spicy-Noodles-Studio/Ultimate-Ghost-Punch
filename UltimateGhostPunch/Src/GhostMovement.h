#pragma once

#ifndef GHOSTMOVE_H
#define GHOSTMOVE_H

#include <UserComponent.h>

class RigidBody;

class GhostMovement : public UserComponent
{
private:
	float maxSpeed=0;
	RigidBody* body;
public:
	GhostMovement(GameObject* g);

	virtual void start();
	void move(Vector3 dir);
	virtual void handleData(ComponentData* data);

	void setSpeed(float speed);

};

#endif 