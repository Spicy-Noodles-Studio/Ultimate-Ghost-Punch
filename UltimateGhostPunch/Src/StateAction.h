#pragma once
#ifndef STATE_ACTION_H
#define STATE_ACTION_H

#include <ErrorManagement.h>

class StateMachine;

class StateAction {
	friend class StateMachine;
protected:
	virtual void update(float deltaTime) = 0;

public:
	StateAction(StateMachine* stateMachine);
	~StateAction();

	/* My State Machine pointer
		Each StateAction will inject its input through stateMachine pointer	*/
	StateMachine* stateMachine;
};

#endif 