#pragma once
#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <UserComponent.h>

class RigidBody;
class GameObject;

class Movement : public UserComponent
{
private:
	RigidBody* rigidBody;
	int force;

public:
	Movement(GameObject* gameObject);

	void move(Vector3 dir);
	// Stops the object's physic body's movement and clear its forces
	void stop();
	
	virtual void start();
	virtual void handleData(ComponentData* data);
};

#endif