#pragma once
#ifndef PLATFORM_NAVIGATION_H
#define PLATFORM_NAVIGATION_H
#include "StateAction.h"


class PlatformNavigation : public StateAction
{
public:
	enum class NavigationState { MOVING_RIGHT, MOVING_LEFT, JUMPING, CANCEL_JUMP, DODGE };
public:
	PlatformNavigation(StateMachine* stateMachine);
	~PlatformNavigation();

protected:
	virtual void update(float deltaTime);

private:
	/* GRAPH INFO */

};

#endif
