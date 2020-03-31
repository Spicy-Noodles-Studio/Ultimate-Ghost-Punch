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

	bool isGrounded = true;

	enum State {
		IDLE, GRABBING, GRABBED
	};

	State state = IDLE;

	RigidBody* attackTrigger;

	GameObject* enemy = nullptr;
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