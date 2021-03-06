#pragma once
#ifndef DODGE_H
#define DODGE_H

#include <UserComponent.h>

class GameObject;
class RigidBody;
class Movement;

class Dodge : public UserComponent
{
private:
	enum State { IDLE, DODGING, CD, };

	RigidBody* rigidBody;

	State state;
	Vector3 playerGravity;

	float cooldown;
	float force;
	float time;
	float duration;
	float atenuation;

protected:
	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

public:
	Dodge(GameObject* gameObject);
	virtual ~Dodge();

	void dodge();
	void endDodge();
	bool isDodging();
	bool isOnCooldown();
};

#endif