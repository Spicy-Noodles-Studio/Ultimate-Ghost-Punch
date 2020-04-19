#pragma once
#ifndef PLATFORM_NAVIGATION_H
#define PLATFORM_NAVIGATION_H

#include "StateAction.h"
#include "PlatformGraph.h"
#include "PlatformNode.h"


typedef std::pair<int, int> ii;

class PlatformNavigation : public StateAction
{
public:
	//enum class NavigationState { MOVING_RIGHT, MOVING_LEFT, JUMPING, CANCEL_JUMP, DODGE };

	struct pathNode {
		PlatformNode platform;
		int index;
	};

public:
	PlatformNavigation(StateMachine* stateMachine);
	~PlatformNavigation();

	void setPlatformGraph(PlatformGraph* platformGraph);
	void setCharacter(GameObject* character);

private:

	std::vector<pathNode> getShortestPath(); //Dijkstra es suficiente

	void moveToStartingPoint(const pathNode& node);
	void moveToPlatform();

protected:
	virtual void update(float deltaTime);

private:
	/* GRAPH INFO */
	PlatformGraph* platformGraph;
	PlatformNode target;	// Target Platform

	GameObject* character;	// Source

	bool movingThroughLink;
	NavigationLink linkInUse;
	double time;
	int lastState;

};

#endif
