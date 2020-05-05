#pragma once
#ifndef PATH_RECORDER_H
#define PATH_RECORDER_H

#include <UserComponent.h>
#include <stack>

class InputSystem;
class RigidBody;
class PlatformGraph;
class State;
class Health;
class Jump;
class GhostManager;
enum class Action;

class PathRecorder : public UserComponent
{
private:
	InputSystem* inputSystem;
	PlatformGraph* graph;

	Health* health;
	Jump* jump;
	GhostManager* ghostManager;

	std::vector<State> states;
	std::stack<int> lastPlatform;

	bool recording;
	int controllerIndex;
	int frame;
	int currentPlatform;

	Vector3 iniPos;

	void eraseLastLink();
	void eraseRecordedLinks();

public:
	PathRecorder(GameObject* gameObject);
	virtual ~PathRecorder();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void onObjectEnter(GameObject* other);
	virtual void onObjectExit(GameObject* other);

	void saveState(Action action);

	void startRecording();
	void stopRecording();
};

#endif