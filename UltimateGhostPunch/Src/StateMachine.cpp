#include "StateMachine.h"
#include "StateAction.h"

StateMachine::StateMachine(GameObject* gameObject) : UserComponent(gameObject) , currentState(nullptr)
{

}

StateMachine::~StateMachine()
{

}

void StateMachine::update(float deltaTime)
{
	//Process current state
	currentState->update(deltaTime);

	//Process input
	processActionInput();

	//Clear input
	actionInputs.clear();
}

void StateMachine::addStateAction(StateAction* stateAction)
{
	stateActions.push_back(stateAction);
}

void StateMachine::addActionInput(ActionInput input)
{
	actionInputs.push_back(input);
}
