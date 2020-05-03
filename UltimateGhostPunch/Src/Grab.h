#pragma once
#ifndef GRAB_H
#define GRAB_H

#include <UserComponent.h>
#include "PlayerController.h"

class RigidBody;
class GameObject;
class Score;
class Block;
class Dodge;
class Attack;
class PlayerAnimController;

class Grab : public UserComponent
{
private:
	enum State { IDLE, GRABBED, BLOCKED };
	
	float grabDuration, remain, freezeDuration, throwForce,
			cooldown, grabTimer, grabVerticalOffset, dropHorizontalOffset;

	State state;

	GameObject* parent;
	PlayerController* controller;
	PlayerAnimController* myAnim;

	GameObject* enemy;
	PlayerController* enemyController;
	PlayerAnimController* enemyAnim;

	Dodge* dodge;
	Block* block;
	Attack* attack;

	Vector3 enemyDiff;
	bool enemyFollowing;
	Vector3 grabbedPosition;
	int prevOrientation;
	float enemyFollowingThreshold;
	int id;
	Score* score;

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
	void grabMissed();

	bool isGrabbing() const;
	bool isOnCooldown() const;
	bool canGrab() const;
};

#endif