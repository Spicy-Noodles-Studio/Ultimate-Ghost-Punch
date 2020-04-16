#include "PathRecorder.h"

#include <GameObject.h>
#include <InputSystem.h>
#include <RigidBody.h>
#include <ComponentRegister.h>

#include "PlatformGraph.h"
#include "PlatformNode.h"
#include "PlayerController.h"
#include "Health.h"
#include "GhostManager.h"
#include "Jump.h"
#include "GameManager.h"

REGISTER_FACTORY(PathRecorder);

PathRecorder::PathRecorder(GameObject* gameObject) : UserComponent(gameObject), recording(false), graph(nullptr), inputSystem(nullptr), frame(-1), lastPlatform(std::stack<int>()), states(std::vector<State>()),
													 currentPlatform(-1), actions(std::vector<Action>()), time(0.0f)
{

}

PathRecorder::~PathRecorder()
{
}

void PathRecorder::start()
{
#ifndef RECORD_PATH
	setActive(false);
#endif 

	frame = 0;
	time = 0.0f;

	GameObject* aux = findGameObjectWithName(GameManager::GetInstance()->getLastLevel());
	if (aux != nullptr) graph = aux->getComponent<PlatformGraph>();

	inputSystem = InputSystem::GetInstance();

	health = gameObject->getParent()->getComponent<Health>();
	ghostManager = gameObject->getParent()->getComponent<GhostManager>();
	controllerIndex = gameObject->getParent()->getComponent<PlayerController>()->getControllerIndex();

	std::vector<GameObject*> v = gameObject->findChildrenWithTag("groundSensor");
	if (v.size() > 0) jump = v[0]->getComponent<Jump>();
}

void PathRecorder::update(float deltaTime)
{
	if (graph != nullptr) {
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

	if (controllerIndex == 4) {
		//If it is an actual jump
		if (inputSystem->getKeyPress("Space") && (jump == nullptr || jump->canJump())) {
			startRecording();
			actions.push_back(Action::Jump);
		}
		if (recording && inputSystem->getKeyPress("LEFT SHIFT"))
			actions.push_back(Action::Dash);
		if (recording && inputSystem->isKeyPressed("A"))
			actions.push_back(Action::MoveLeft);
		if (recording && inputSystem->isKeyPressed("D"))
			actions.push_back(Action::MoveRight);

		if (recording && actions.size() != 0)
			saveState(actions);
	}
	//If we recived damage we stop recording
	if (health != nullptr && health->isInvencible())
		stopRecording();
	if (ghostManager != nullptr && ghostManager->isGhost())
		stopRecording();

	if (recording) {
		frame++;
		time += deltaTime;
	}

}

void PathRecorder::onObjectEnter(GameObject* other)
{
	if (controllerIndex == 4 && other->getTag() == "suelo" && recording) {
		Vector3 endPos = gameObject->transform->getWorldPosition();

		if (currentPlatform != -1) lastPlatform.push(currentPlatform);

		if (graph != nullptr)
			currentPlatform = graph->getIndex(endPos);

		if (currentPlatform != -1) {
			NavigationLink navLink = NavigationLink(states, iniPos, endPos, frame, time, currentPlatform);
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
	if (controllerIndex == 4 && other->getTag() == "suelo" && !recording) {
		saveState({ Action::None });
		startRecording();
	}
}

void PathRecorder::saveState(const std::vector<Action>& actions)
{
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
	iniPos = gameObject->transform->getWorldPosition();
	recording = true;
}

void PathRecorder::eraseLastLink()
{
	if (graph != nullptr && !lastPlatform.empty()) {
		graph->removeLastLink(lastPlatform.top());
		lastPlatform.pop();
	}
}

void PathRecorder::eraseRecordedLinks()
{
	while (!lastPlatform.empty()) {
		eraseLastLink();
	}
}

