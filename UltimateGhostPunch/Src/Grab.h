#pragma once
#ifndef GRAB_H
#define GRAB_H

#include <UserComponent.h>

class GameObject;
class RigidBody;
class PlayerState;
class Score;

class Grab : public UserComponent
{
private:
	enum State { IDLE, CHARGE, GRABBED, BLOCKED };

	int id;

	float grabDuration;
	float freezeDuration;

	float throwForce;

	float remain;
	float cooldown;
	float grabTimer;

	float grabVerticalOffset;
	float dropHorizontalOffset;

	int charging;

	int dropped;
	int missed;

	State state;

	GameObject* parent;

	GameObject* enemy;

	Score* score;
	PlayerState* playerState;

	Vector3 enemyDiff;
	bool enemyFollowing;

	Vector3 grabbedPosition;
	int prevOrientation;
	float enemyFollowingThreshold;

	void resetEnemy();
	void grabEnemy();

protected:
	virtual void start();
	virtual void update(float deltaTime);
	virtual void postUpdate(float deltaTime);

	virtual void onObjectStay(GameObject* other);
	virtual void onObjectEnter(GameObject* other);
	virtual void onObjectExit(GameObject* other);
	virtual void handleData(ComponentData* data);

public:
	Grab(GameObject* gameObject);
	virtual ~Grab();

	void chargeGrab();
	void grab();
	void drop();
	void grabMissed();

	bool isGrabbing() const;
	bool isCharging() const;
	bool isOnCooldown() const;
	bool isStunned() const;

	bool hasMissed() const;
	bool hasDropped() const;
};

#endif