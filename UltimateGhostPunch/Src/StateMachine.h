#pragma once
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <UserComponent.h>

class StateAction;
enum class ActionInput;

class StateMachine : public UserComponent
{
protected:
	std::vector<ActionInput> actionInputs; // Input del jugador (puede ser varios)
	std::vector<StateAction*> stateActions; // Los estados
	StateAction* currentState;
public:
	StateMachine(GameObject* gameObject);
	virtual ~StateMachine();

	virtual void update(float deltaTime);

	void addStateAction(StateAction* stateAction);
	void addActionInput(ActionInput input);

private:
	virtual void processActionInput() = 0;

};

#endif