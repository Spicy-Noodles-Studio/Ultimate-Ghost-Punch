#pragma once
#ifndef ULTIMATE_GHOST_PUNCH_H
#define ULTIMATE_GHOST_PUNCH_H

#include <UserComponent.h>

class RigidBody;
class GhostMovement;
class PlayerAnimController;

class UltimateGhostPunch : public UserComponent
{
private:
	enum State { NONE, CHARGING, PUNCHING, AVAILABLE, USED, SUCCESS, FAIL };

	RigidBody* rigidBody;
	GhostMovement* ghostMovement;
	PlayerAnimController* anim;

	Vector3 direction;
	State state;

	float duration;
	float force;
	float ghostSpeed;

	// Speed multiplayer for punch charging speed (from 0.0 to 1.0)
	float chargeSpeed;

public:
	UltimateGhostPunch(GameObject* gameObject);
	virtual ~UltimateGhostPunch();

	virtual void start();
	virtual void preUpdate(float deltaTime);
	virtual void postUpdate(float deltaTime);
	virtual void handleData(ComponentData* data);

	void charge();
	void aim(double x, double y);

	void ghostPunch();
	void punchSucceeded();

	bool isUsed() const;
	bool isAiming() const;
	bool isPunching() const;
	bool punchSuccess() const;
	bool punchFail() const;
};

#endif