#pragma once
#ifndef STATE_ACTION_H
#define STATE_ACTION_H

class StateMachine;

class StateAction {
	friend class StateMachine;
public:
	StateAction(StateMachine* stateMachine);
	~StateAction();

protected:
	virtual void update(float deltaTime) = 0;

	/* 
		My State Machine pointer
		Each StateAction will inject its input through stateMachine pointer	
	
		TODO: pensar en la transicion de estados
	*/
	StateMachine* stateMachine;
};

#endif 