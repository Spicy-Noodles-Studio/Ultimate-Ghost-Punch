#include "StateAction.h"

StateAction::StateAction(StateMachine* stateMachine) : stateMachine(stateMachine)
{
	checkNull(stateMachine);
}

StateAction::~StateAction()
{

}
