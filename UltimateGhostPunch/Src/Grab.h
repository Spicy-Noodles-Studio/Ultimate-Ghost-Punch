#pragma once
#ifndef GRAB_H
#define GRAB_H

#include <UserComponent.h>

class RigidBody;
class GameObject;

class Grab : public UserComponent
{
private:
	float duration = 1.5f;
	float remain = 0.0f;

	float force = 1500.0f;

	bool isGrounded = true;

	enum State {
		IDLE, GRABBING, GRABBED
	};

	State state = IDLE;

	RigidBody* attackTrigger;

	GameObject* enemy = nullptr;


	Vector3 vIzq = { -1,0,0 };
	Vector3 vDer = { 1,0,0 };

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