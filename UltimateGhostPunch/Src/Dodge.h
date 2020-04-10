#pragma once
#ifndef DODGE_H
#define DODGE_H

#include <UserComponent.h>

class RigidBody;
class GameObject;

class Dodge : public UserComponent
{
private:
	enum State { IDLE, CD };
	
	float cooldown, force, cd ;
	
	State state;

public:
	Dodge(GameObject* gameObject);

	virtual void update(float deltaTime);

	virtual void handleData(ComponentData* data);

	void dodge();
};

#endif