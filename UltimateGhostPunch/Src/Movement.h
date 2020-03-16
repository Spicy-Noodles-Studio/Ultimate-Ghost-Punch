#pragma once
#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <UserComponent.h>
#include <GameObject.h>
#include <RigidBody.h>

class Movement : public UserComponent
{
private:
	RigidBody* rigidBody;
	int force;

public:
	Movement(GameObject* gameObject);

	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

	void move(Vector3 force);
};

#endif