#pragma once
#ifndef PLATFORM_NAVIGATION_H
#define PLATFORM_NAVIGATION_H
#include "StateAction.h"
#include "PlatformGraph.h"
#include "PlatformNode.h"


class PlatformNavigation : public StateAction
{
public:
	enum class NavigationState { MOVING_RIGHT, MOVING_LEFT, JUMPING, CANCEL_JUMP, DODGE };
public:
	PlatformNavigation(StateMachine* stateMachine);
	~PlatformNavigation();

	void setPlatformGraph(PlatformGraph* platformGraph);
	void setCharacter(GameObject* character);

private:
	std::vector<PlatformNode> getShortestPath(); //Dijkstra es suficiente

protected:
	virtual void update(float deltaTime);

private:
	/* GRAPH INFO */
	PlatformGraph* platformGraph;
	PlatformNode target;	// Target Platform

	GameObject* character;	// Source

};

#endif
