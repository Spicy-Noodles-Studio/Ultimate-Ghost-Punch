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
	struct PathNode {
		PlatformNode platform;
		int index;
	};

public:
	PlatformNavigation(StateMachine* stateMachine);
	~PlatformNavigation();

	void setPlatformGraph(PlatformGraph* platformGraph);
	void setCharacter(GameObject* character);

	void setTarget(const Vector3& position);
	void setTarget(GameObject* target);
	void setTarget(const PlatformNode& node);

	/* Returns true if character is over target */
	bool hasArrived() const;

private:

	std::vector<PathNode> getShortestPath(); // Dijkstra

	void moveToStartingPoint(const PathNode& node);
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
