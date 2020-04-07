#pragma once
#ifndef GRAB_H
#define GRAB_H

#include <UserComponent.h>
#include "PlayerController.h"

class RigidBody;
class GameObject;

class Grab : public UserComponent
{
private:
	float duration;
	float remain;
	float freezeDuration;

	float force;

	bool isGrounded;

	enum State {
		IDLE, GRABBING, GRABBED, BLOCKED
	};

	State state;
	State last;

	RigidBody* attackTrigger;
	PlayerController* controller;

	GameObject* enemy;


	Vector3 vIzq;
	Vector3 vDer;

	Vector3 enemyDiff;
public:
	Grab(GameObject* gameObject);

	virtual void start();
	virtual void update(float deltaTime);
	virtual void onObjectStay(GameObject* other);
	virtual void onObjectEnter(GameObject* other);
	virtual void onObjectExit(GameObject* other);

	virtual void handleData(ComponentData* data);

	void grab();
	void drop();
};

#endif