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

REGISTER_FACTORY(PathRecorder);

PathRecorder::PathRecorder(GameObject* gameObject) : UserComponent(gameObject), recording(false), graph(nullptr), inputSystem(nullptr), frame(-1)
{
	states = std::vector<State>();
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
	if(aux != nullptr) graph = aux->getComponent<PlatformGraph>();

	inputSystem = InputSystem::GetInstance();

	health = gameObject->getParent()->getComponent<Health>();
	ghostManager = gameObject->getParent()->getComponent<GhostManager>();
	controllerIndex = gameObject->getParent()->getComponent<PlayerController>()->getControllerIndex();

	std::vector<GameObject*> v = gameObject->findChildrenWithTag("groundSensor");
	if (v.size() > 0) jump = v[0]->getComponent<Jump>();
}

void PathRecorder::update(float deltaTime)
{
	if (inputSystem->getKeyPress("O"))
		graph->saveGraph();

	if (controllerIndex == 4) {
		//If it is an actual jump
		if (inputSystem->getKeyPress("Space") && (jump == nullptr ||jump->canJump())) {
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

	if (recording) frame++;

}

void PathRecorder::onObjectEnter(GameObject* other)
{
	if (controllerIndex == 4 && other->getTag() == "suelo" && recording) {
		int endIndex = -1, iniIndex = -1;
		Vector3 endPos = gameObject->transform->getWorldPosition();
		if (graph != nullptr) {
			endIndex = graph->getIndex(endPos);
			iniIndex = graph->getIndex(iniPos);
		}
		if (endIndex != -1) {
			NavigationLink navLink = NavigationLink(states, iniPos, endPos, frame, endIndex);
			if (iniIndex != -1) {
				graph->addLinkToPlatform(iniIndex, navLink);
			}
		}
		stopRecording();
	}
}

void PathRecorder::onObjectExit(GameObject* other)
{
	//Start recording
	if (controllerIndex == 4 && other->getTag() == "suelo" && !recording) {
		saveState(Action::None);
		startRecording();
	}
}

void PathRecorder::saveState(Action action)
{
	states.push_back(State(action, frame, gameObject->transform->getWorldPosition()));
}

void PathRecorder::stopRecording()
{
	recording = false;
	frame = 0;
	states.clear();
}

void PathRecorder::startRecording()
{
	iniPos = gameObject->transform->getWorldPosition();
	recording = true;
}

