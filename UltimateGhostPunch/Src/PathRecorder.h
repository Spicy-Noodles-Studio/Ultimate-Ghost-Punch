#pragma once

#ifndef PATH_RECORDER_H
#define PATH_RECORDER_H

#include <UserComponent.h>
#include <stack>

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
	std::stack<int> lastPlatform;

	int controllerIndex, frame, currentPlatform, startDirection;
	float time;

	Vector3 iniPos, startVelocity, startForce;
	std::vector<Action> actions;

	void eraseLastLink();
	void eraseRecordedLinks();

public:
	PathRecorder(GameObject* gameObject);
	~PathRecorder();

	virtual void start();
	virtual void update(float deltaTime);

	virtual void onObjectEnter(GameObject* other);
	virtual void onObjectExit(GameObject* other);

	void saveState(const std::vector<Action>& actions);
	
	void stopRecording();
	void startRecording();

};

#endif 