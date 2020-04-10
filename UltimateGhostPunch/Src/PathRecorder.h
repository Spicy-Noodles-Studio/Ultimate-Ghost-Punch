#pragma once

#ifndef PATH_RECORDER_H
#define PATH_RECORDER_H

#include <UserComponent.h>

class RigidBody;
class PlatformGraph;
class InputSystem;
class State;
enum class Action;

class PathRecorder : public UserComponent
{
private:
	bool record, recording;
	RigidBody* rigidBody;
	PlatformGraph* graph;
	InputSystem* inputSystem;
	std::vector<State> states;

	int controllerIndex;

	Vector3 iniPos;
public:
	PathRecorder(GameObject* gameObject);
	~PathRecorder();

	virtual void start();
	virtual void update(float deltaTime);

	virtual void onCollisionEnter(GameObject* other);
	virtual void onCollisionExit(GameObject* other);

	void saveState(Action action);
};

#endif 