#pragma once

#ifndef GHOST_NAVIGATION_H
#define GHOST_NAVIGATION_H

#include <vector>
#include <Vector3.h>

#include "StateAction.h"

class GameObject;

class GhostNavigation : public StateAction
{
public:
	GhostNavigation(StateMachine* stateMachine);
	~GhostNavigation();

	void setTarget(GameObject* target);
	void setCharacter(GameObject* charcacter);

	Vector3 getDirection() const;

protected:
	virtual void update(float deltaTime);

private:
	GameObject* target;
	GameObject* character;
	Vector3 direction;
};

#endif
