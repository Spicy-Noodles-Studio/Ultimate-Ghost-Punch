#pragma once
#ifndef PLATFORM_NODE_H
#define PLATFORM_NODE_H

#include <Vector3.h>
#include <vector>

enum class Action { MoveRight, MoveLeft, Jump, CancelJump, Dash, None };

class GaiaData;

class State
{
private:
	std::vector<Action> actions;
	int frame;//The frame of the trajectory were we�re at
	float time;//Time that has passed since the begining of the recording
	Vector3 pos;

public:
	State();
	State(const std::vector<Action>& actions, int frame, float time, const Vector3& pos);

	Vector3 getPos() const;
	int getFrame() const;
	float getTime();
	std::vector<Action> getActions();

	void addAction(Action action);

	GaiaData saveState();
	void loadState(const GaiaData& data);
};

class NavigationLink
{
private:
	std::vector<State> linkStates;
	Vector3 iniPos, endPos, startVelocity, startForce;
	int connection, frames, startDirection;
	float duration;

public:
	NavigationLink();
	NavigationLink(const std::vector<State>& states, const Vector3& iniPos, const Vector3& endPos, const Vector3& startVelocity, const Vector3& startForce, int frames, float duration, int connection, int startDirection);

	int getConnection() const;
	std::vector<State> getStates() const;
	Vector3 getIniPos() const;
	Vector3 getEndPos() const;
	Vector3 getStartVelocity() const;
	Vector3 getStartForce() const;
	int getFrames() const;
	int getDirection() const;
	float getDuration() const;

	GaiaData saveLink();
	void loadLink(const GaiaData& data);
};

class PlatformNode
{
private:
	Vector3 iniPos;
	Vector3 endPos;
	Vector3 midPos;
	int index;
	std::vector<NavigationLink> edges;

public:
	PlatformNode();
	PlatformNode(const Vector3& iniPos, int index);
	PlatformNode(const Vector3& iniPos, const Vector3& endPos, int index);
	~PlatformNode();

	void setEnd(const Vector3& endPos);
	void setBegining(const Vector3& iniPos);

	Vector3 getBegining() const;
	Vector3 getEnd() const;
	Vector3 getMiddle() const;
	int getIndex() const;
	std::vector<NavigationLink> getEdges() const;
	NavigationLink getEdge(int index) const;

	void addEdge(const NavigationLink& link);
	void removeLastEdge();
	void removeAllEdges();

	GaiaData savePlatform();
	void loadPlatform(const GaiaData& data);

	std::vector<NavigationLink>& getEdges();
};

#endif