#pragma once
#include "StateAction.h"
class FightingState :
	public StateAction
{
private:


public:
	FightingState(StateMachine* stateMachine);
	virtual ~FightingState();

};

