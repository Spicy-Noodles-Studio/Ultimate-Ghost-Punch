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
#include "GhostManager.h"

REGISTER_FACTORY(PathRecorder);

PathRecorder::PathRecorder(GameObject* gameObject) : UserComponent(gameObject), inputSystem(nullptr), graph(nullptr), health(nullptr), jump(nullptr), ghostManager(nullptr),
states(std::vector<State>()), lastPlatform(std::stack<int>()), recording(false), controllerIndex(0), frame(-1), currentPlatform(-1)
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

	GameObject* aux = findGameObjectWithName("Level1");
	if (aux != nullptr)
		graph = aux->getComponent<PlatformGraph>();

	inputSystem = InputSystem::GetInstance();

	health = gameObject->getParent()->getComponent<Health>();
	ghostManager = gameObject->getParent()->getComponent<GhostManager>();
	controllerIndex = gameObject->getParent()->getComponent<PlayerController>()->getControllerIndex();

	std::vector<GameObject*> v = gameObject->findChildrenWithTag("groundSensor");
	if (v.size() > 0)
		jump = v[0]->getComponent<Jump>();
}

void PathRecorder::update(float deltaTime)
{
	//Saves the graph
	if (inputSystem->getKeyPress("O"))
		graph->saveGraph();

	//Removes last link of the last platform we have been at
	else if (inputSystem->getKeyPress("K"))
		graph->removeLastLink(currentPlatform);

	//Removes all links of the last platform we have been at
	else if (inputSystem->getKeyPress("L"))
		graph->clearConnections(currentPlatform);

	//Removes all connections
	else if (inputSystem->getKeyPress("P"))
		graph->clearAllConnections();

	//Removes last link created
	else if (inputSystem->getKeyPress("U"))
		eraseLastLink();

	//Removes all links created in this recording
	else if (inputSystem->getKeyPress("I"))
		eraseRecordedLinks();

	if (controllerIndex == 4)
	{
		//If it is an actual jump
		if (inputSystem->getKeyPress("Space") && (jump == nullptr || jump->canJump()))
		{
			saveState(Action::Jump);
			startRecording();
		}
		else if (recording && inputSystem->getKeyPress("LEFT SHIFT"))
			saveState(Action::Dash);
		else if (recording && inputSystem->isKeyPressed("A"))
			saveState(Action::MoveLeft);
		else if (recording && inputSystem->isKeyPressed("D"))
			saveState(Action::MoveRight);
	}

	//If we recived damage we stop recording
	if (health != nullptr && health->isInvencible())
		stopRecording();

	if (ghostManager != nullptr && ghostManager->isGhost())
		stopRecording();

	if (recording)
		frame++;
}

void PathRecorder::onObjectEnter(GameObject* other)
{
	if (controllerIndex == 4 && other->getTag() == "suelo" && recording)
	{
		Vector3 endPos = gameObject->transform->getWorldPosition();

		if (currentPlatform != -1)
			lastPlatform.push(currentPlatform);

		if (graph != nullptr)
			currentPlatform = graph->getIndex(endPos);

		if (currentPlatform != -1)
		{
			NavigationLink navLink = NavigationLink(states, iniPos, endPos, frame, currentPlatform);
			if (!lastPlatform.empty())
				graph->addLinkToPlatform(lastPlatform.top(), navLink);
		}
		stopRecording();
	}
}

void PathRecorder::onObjectExit(GameObject* other)
{
	//Start recording
	if (controllerIndex == 4 && other->getTag() == "suelo" && !recording)
	{
		saveState(Action::None);
		startRecording();
	}
}

void PathRecorder::saveState(Action action)
{
	states.push_back(State(action, frame, gameObject->transform->getWorldPosition()));
}

void PathRecorder::startRecording()
{
	iniPos = gameObject->transform->getWorldPosition();
	recording = true;
}

void PathRecorder::stopRecording()
{
	recording = false;
	frame = 0;
	states.clear();
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