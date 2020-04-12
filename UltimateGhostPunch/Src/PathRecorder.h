#pragma once

#ifndef PATH_RECORDER_H
#define PATH_RECORDER_H

#include <UserComponent.h>

class RigidBody;
class PlatformGraph;
class InputSystem;
class State;
class Health;
class GhostManager;
class Jump;
enum class Action;

class PathRecorder : public UserComponent
{
private:
	bool recording;
	PlatformGraph* graph;
	InputSystem* inputSystem;
	Health* health;
	GhostManager* ghostManager;
	Jump* jump;
	std::vector<State> states;

	int controllerIndex, frame;

	Vector3 iniPos;
public:
	PathRecorder(GameObject* gameObject);
	~PathRecorder();

	virtual void start();
	virtual void update(float deltaTime);

	virtual void onObjectEnter(GameObject* other);
	virtual void onObjectExit(GameObject* other);

	void saveState(Action action);
	
	void stopRecording();
	void startRecording();
};

#endif 