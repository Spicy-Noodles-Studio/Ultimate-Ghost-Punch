#pragma once
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <UserComponent.h>

class StateAction;
enum class ActionInput;

class StateMachine : public UserComponent
{
protected:
	std::vector<ActionInput> actionInputs; // Input to simulate
	std::vector<StateAction*> stateActions; // AI different states
	StateAction* currentState;

protected:
	virtual void update(float deltaTime);

public:
	StateMachine(GameObject* gameObject);
	virtual ~StateMachine();

	void addStateAction(StateAction* stateAction);
	void addActionInput(ActionInput input);

private:
	virtual void processActionInput() = 0;

};

#endif