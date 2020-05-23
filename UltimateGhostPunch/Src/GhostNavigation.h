#pragma once

#ifndef GHOST_NAVIGATION_H
#define GHOST_NAVIGATION_H

#include <vector>
#include <Vector3.h>

#include "StateAction.h"

class GameObject;
class UltimateGhostPunch;

class GhostNavigation : public StateAction
{
public:
	GhostNavigation(StateMachine* stateMachine);
	virtual ~GhostNavigation();

	void setTarget(GameObject* target);
	void setCharacter(GameObject* charcacter);

	Vector3 getDirection() const;
	UltimateGhostPunch* punch;

protected:
	virtual void update(float deltaTime);

private:
	GameObject* target;
	GameObject* character;
	Vector3 direction;
	// Distance for charging UGP
	float punchChargeDist;
	// Random charge time
	float chargeTime;
	// Punch Fail Factor
	int punchFailFactor;

	bool charging;
};

#endif
