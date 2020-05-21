#include "PathRecorder.h"
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


#include <ComponentRegister.h>

REGISTER_FACTORY(PathRecorder);

PathRecorder::PathRecorder(GameObject* gameObject) : UserComponent(gameObject), recording(false), graph(nullptr), inputSystem(nullptr), ghostManager(nullptr), health(nullptr),
													 jump(nullptr), parent(nullptr), controllerIndex(-1), frame(-1), lastPlatform(std::stack<int>()), states(std::vector<State>()), 
													 currentPlatform(-1), actions(std::vector<Action>()), time(0.0f), startVelocity(Vector3::ZERO), iniPos(Vector3::ZERO), 
													 startForce(Vector3::ZERO), startDirection(-1)
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

	GameObject* aux = findGameObjectWithName("LevelCollider");
	if (aux != nullptr) graph = aux->getComponent<PlatformGraph>();

	inputSystem = InputSystem::GetInstance();
	
	parent = gameObject->getParent();
	if (parent != nullptr) {
		health = parent->getComponent<Health>();
		ghostManager = parent->getComponent<GhostManager>();
	}
	// TODO: devolver a como estaba antes, esto petaba
	controllerIndex = 4; // gameObject->getParent()->getComponent<PlayerController>()->getControllerIndex();

	std::vector<GameObject*> v = gameObject->findChildrenWithTag("groundSensor");
	if (v.size() > 0)
		jump = v[0]->getComponent<Jump>();
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

	if (controllerIndex == 4)
	{
		//If it is an actual jump
		if (inputSystem->getKeyPress("Space") && (jump == nullptr || jump->canJump())) {
			startRecording();
			actions.push_back(Action::Jump);
		}
		else if (inputSystem->getKeyRelease("Space") && (jump == nullptr || jump->isJumping()))
			actions.push_back(Action::CancelJump);

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
	if (controllerIndex == 4 && other != nullptr && other->getTag() == "suelo" && recording)
	{
		Vector3 endPos = gameObject->transform != nullptr ? gameObject->transform->getWorldPosition(): Vector3::ZERO;

		if (currentPlatform != -1)
			lastPlatform.push(currentPlatform);

		if (graph != nullptr)
			currentPlatform = graph->getIndex(endPos);

		if (currentPlatform != -1) {			
			NavigationLink navLink = NavigationLink(states, iniPos, endPos, startVelocity, startForce, frame, time, currentPlatform, startDirection);
			if (!lastPlatform.empty() && graph != nullptr) {
				graph->addLinkToPlatform(lastPlatform.top(), navLink);
			}
		}
		stopRecording();
	}
}

void PathRecorder::onObjectExit(GameObject* other)
{
	//Start recording
	if (controllerIndex == 4 && other != nullptr && other->getTag() == "suelo" && !recording) {
		saveState({ Action::None });
		startRecording();
	}
}

void PathRecorder::saveState(const std::vector<Action>& actions)
{
	if (gameObject->transform == nullptr) return;
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
	if (gameObject->transform == nullptr) return;

	iniPos = gameObject->transform->getWorldPosition();
	startDirection = (gameObject->transform->getRotation().y == 90) ? 1 : -1;
	recording = true;

	if (parent != nullptr) {
		RigidBody* rb = parent->getComponent<RigidBody>();
		if (rb != nullptr) {
			startVelocity = rb->getLinearVelocity();
			startForce = rb->getTotalForce();
		}
	}
}

void PathRecorder::eraseLastLink()
{
	if (graph != nullptr && !lastPlatform.empty())
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