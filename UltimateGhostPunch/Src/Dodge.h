#pragma once
#ifndef DODGE_H
#define DODGE_H

#include <UserComponent.h>

class RigidBody;
class GameObject;
class Movement;

class Dodge : public UserComponent
{
private:
	enum class State { IDLE, DODGING, CD, };

	float cooldown, force, time, duration, atenuation;
	Vector3 playerGravity;

	State state;
	RigidBody* rigidBody;

public:
	Dodge(GameObject* gameObject);

	virtual void update(float deltaTime);
	virtual void start();

	virtual void handleData(ComponentData* data);

	bool dodge();
	void endDodge();
	bool isDodging();
};

#endif