#pragma once

#ifndef GHOSTMOVE_H
#define GHOSTMOVE_H

#include <UserComponent.h>
#include <GameObject.h>
#include <RigidBody.h>

class GhostMovement : public UserComponent
{
private:
	float maxSpeed=0;
	RigidBody* body;
public:
	GhostMovement(GameObject* g);

	virtual void awake();
	void move(Vector3 dir);
	virtual void handleData(ComponentData* data);

	void setSpeed(float speed);

};

#endif 