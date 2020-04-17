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
	enum State { IDLE, GRABBING, GRABBED, BLOCKED };
	
	float grabDuration, remain, freezeDuration, throwForce,
			cooldown, grabTimer, grabVerticalOffset, dropHorizontalOffset;

	State state;

	PlayerController* controller;

	GameObject* enemy;
	PlayerController* enemyController;

	Vector3 enemyDiff;

	void resetEnemy();
	void grabEnemy();
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

	bool isGrabbing() const;
	bool isOnCooldown() const;
};

#endif