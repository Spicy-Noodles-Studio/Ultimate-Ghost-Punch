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
	bool recording;
	InputSystem* inputSystem;
	PlatformGraph* graph;

	Health* health;
	Jump* jump;
	GhostManager* ghostManager;

	GameObject* parent;

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
	virtual ~PathRecorder();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void onObjectEnter(GameObject* other);
	virtual void onObjectExit(GameObject* other);

	void saveState(const std::vector<Action>& actions);
	
	void stopRecording();
	void startRecording();
};

#endif