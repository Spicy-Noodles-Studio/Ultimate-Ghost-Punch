#pragma once

#ifndef PLATFORM_NODE_H
#define PLATFORM_NODE_H

#include <Vector3.h>
#include <vector>

enum class Action { Dash, Jump, MoveLeft, MoveRight, None };

class State {
private:
	Vector3 pos;
	Vector3 velocity;
	Vector3 force;
	Action action;

public:
	State();
	State(const Vector3& pos, const Vector3& velocity, const Vector3& force, Action action);

	Vector3 getPos() const;
};

class NavigationLink {
private:
	std::vector<State> linkStates;
	int connection;

public:
	NavigationLink(const std::vector<State>& states, int connection);

	int getConnection() const;
	std::vector<State> getStates() const;
};

class PlatformNode
{
private:
	Vector3 iniPos, endPos;
	std::vector<NavigationLink> edges;

public:
	PlatformNode();
	PlatformNode(const Vector3& iniPos);
	PlatformNode(const Vector3& iniPos, const Vector3& endPos);
	~PlatformNode();

	void setEnd(const Vector3& endPos);
	void setBegining(const Vector3& iniPos);

	Vector3 getBegining() const;
	Vector3 getEnd() const;

	void addEdge(const NavigationLink& link);

	std::vector<NavigationLink> getEdges();
};

#endif