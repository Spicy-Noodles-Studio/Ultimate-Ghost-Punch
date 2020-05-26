#include "PathRecorder.h"

#include <ComponentRegister.h>
#include <InputSystem.h>
#include <GameObject.h>
#include <RigidBody.h>

#include "PlatformGraph.h"
#include "PlatformNode.h"
#include "PlayerController.h"
#include "Health.h"
#include "Jump.h"
#include "GameManager.h"
#include "GhostManager.h"


REGISTER_FACTORY(PathRecorder);

PathRecorder::PathRecorder(GameObject* gameObject) : UserComponent(gameObject), recording(false), graph(nullptr), inputSystem(nullptr), ghostManager(nullptr), health(nullptr),
jump(nullptr), parent(nullptr), controllerIndex(-1), frame(-1), lastPlatform(std::stack<int>()), states(std::vector<State>()),
currentPlatform(-1), actions(std::vector<Action>()), time(0.0f), startVelocity(Vector3::ZERO), iniPos(Vector3::ZERO),
startForce(Vector3::ZERO), startDirection(-1)
{

}

PathRecorder::~PathRecorder()
{
	graph = nullptr;
	inputSystem = nullptr;
	ghostManager = nullptr;
	health = nullptr;
	jump = nullptr;
	parent = nullptr;

	states.clear();
	actions.clear();
}

void PathRecorder::start()
{
#ifndef RECORD_PATH
	setActive(false);
#endif 

	frame = 0;
	time = 0.0f;

	GameObject* aux = findGameObjectWithName("LevelCollider");
	if (notNull(aux)) graph = aux->getComponent<PlatformGraph>();
	checkNull(graph);

	inputSystem = InputSystem::GetInstance();
	checkNull(inputSystem);

	controllerIndex = 4;

	checkNullAndBreak(gameObject);
	parent = gameObject->getParent();
	checkNullAndBreak(parent);

	jump = gameObject->getComponent<Jump>();
	ghostManager = parent->getComponent<GhostManager>();
	health = parent->getComponent<Health>();
	checkNull(jump);
	checkNull(health);
	checkNull(ghostManager);
}

void PathRecorder::update(float deltaTime)
{
	checkNullAndBreak(inputSystem);

	if (controllerIndex == 4)
	{
		//If it is an actual jump
		if (inputSystem->getKeyPress("Space") && (!notNull(jump) || jump->canJump())) {
			startRecording();
			actions.push_back(Action::Jump);
		}
		else if (inputSystem->getKeyRelease("Space"))
			actions.push_back(Action::CancelJump);

		if (recording && inputSystem->getKeyPress("LEFT SHIFT"))
			actions.push_back(Action::Dash);
		if (recording && inputSystem->isKeyPressed("A"))
			actions.push_back(Action::MoveLeft);
		if (recording && inputSystem->isKeyPressed("D"))
			actions.push_back(Action::MoveRight);

		if (recording && actions.size() != 0)
			saveState(actions);

		checkNullAndBreak(graph);
		//Saves the graph
		if (inputSystem->getKeyPress("O"))
			graph->saveGraph();

		//Removes last link of the last platform we have been at
		else if (inputSystem->getKeyPress("K"))
			graph->removeLastLink(currentPlatform);

		//Removes all links of the last platform we have been at
		else if (inputSystem->getKeyPress("L")) {
			graph->clearConnections(currentPlatform);
		}

		//Removes all connections
		else if (inputSystem->getKeyPress("P"))
			graph->clearAllConnections();

		//Removes last link created
		else if (inputSystem->getKeyPress("U"))
			eraseLastLink();

		//Removes all links created in this recording
		else if (inputSystem->getKeyPress("I"))
			eraseRecordedLinks();
	}

	//If we recived damage we stop recording
	if (notNull(health) && health->isInvencible())
		stopRecording();

	if (notNull(ghostManager) && ghostManager->isGhost())
		stopRecording();

	if (recording) {
		frame++;
		time += deltaTime;
	}

}

void PathRecorder::onObjectEnter(GameObject* other)
{
	if (controllerIndex == 4 && notNull(other) && other->getTag() == "suelo" && recording)
	{
		Vector3 endPos = notNull(gameObject) && notNull(gameObject->transform) ? gameObject->transform->getWorldPosition() : Vector3::ZERO;

		if (currentPlatform != -1)
			lastPlatform.push(currentPlatform);

		checkNullAndBreak(graph);

		currentPlatform = graph->getIndex(endPos);
		if (currentPlatform != -1) {
			NavigationLink navLink = NavigationLink(states, iniPos, endPos, startVelocity, startForce, frame, time, currentPlatform, startDirection);
			if (!lastPlatform.empty()) {
				graph->addLinkToPlatform(lastPlatform.top(), navLink);
			}
		}
		stopRecording();
	}
}

void PathRecorder::onObjectExit(GameObject* other)
{
	//Start recording
	if (controllerIndex == 4 && notNull(other) && other->getTag() == "suelo" && !recording) {
		saveState({ Action::None });
		startRecording();
	}
}

void PathRecorder::saveState(const std::vector<Action>& actions)
{
	if (!notNull(gameObject) && !notNull(gameObject->transform)) return;
	states.push_back(State(actions, frame, time, gameObject->transform->getWorldPosition()));
	this->actions.clear();
}

void PathRecorder::stopRecording()
{
	recording = false;
	frame = 0;
	time = 0.0f;
	states.clear();
}

void PathRecorder::startRecording()
{
	checkNullAndBreak(gameObject);
	checkNullAndBreak(gameObject->transform);

	iniPos = gameObject->transform->getWorldPosition();
	startDirection = (gameObject->transform->getRotation().y == 90) ? 1 : -1;
	recording = true;

	checkNullAndBreak(parent);

	RigidBody* rb = parent->getComponent<RigidBody>();
	checkNullAndBreak(rb);

	startVelocity = rb->getLinearVelocity();
	startForce = rb->getTotalForce();
}

void PathRecorder::eraseLastLink()
{
	if (notNull(graph) && !lastPlatform.empty())
	{
		graph->removeLastLink(lastPlatform.top());
		lastPlatform.pop();
	}
}

void PathRecorder::eraseRecordedLinks()
{
	while (!lastPlatform.empty())
		eraseLastLink();
}