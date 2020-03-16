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

	void move(Vector3 dir);
	
	virtual void start();
	virtual void handleData(ComponentData* data);
};

#endif