#include "PathRecorder.h"

#include <GameObject.h>
#include <InputSystem.h>
#include <RigidBody.h>
#include <ComponentRegister.h>

#include "PlatformGraph.h"
#include "PlatformNode.h"
#include "PlayerController.h"

REGISTER_FACTORY(PathRecorder);

PathRecorder::PathRecorder(GameObject* gameObject) : UserComponent(gameObject), record(true), recording(false), rigidBody(nullptr), graph(nullptr), inputSystem(nullptr)
{
	states = std::vector<State>();
}

PathRecorder::~PathRecorder()
{
}

void PathRecorder::start()
{
	rigidBody = gameObject->getComponent<RigidBody>();
	
	GameObject* aux = findGameObjectWithName("Level1");
	if(aux != nullptr) graph = aux->getComponent<PlatformGraph>();

	inputSystem = InputSystem::GetInstance();

	controllerIndex = gameObject->getParent()->getComponent<PlayerController>()->getControllerIndex();
}

void PathRecorder::update(float deltaTime)
{
	if (controllerIndex == 4 && record) {
		if (inputSystem->getKeyPress("Space")) {
			if(!recording)
				iniPos = gameObject->getParent()->transform->getWorldPosition();
			saveState(Action::Jump);
			recording = true;
		}
		else if (recording && inputSystem->getKeyPress("Shift"))
			saveState(Action::Dash);
		else if (recording && inputSystem->getKeyPress("A"))
			saveState(Action::MoveLeft);
		else if (recording && inputSystem->getKeyPress("D"))
			saveState(Action::MoveRight);
		else if (recording)
			saveState(Action::None);
	}
}

void PathRecorder::onObjectEnter(GameObject* other)
{
	if (controllerIndex == 4 && other->getTag() == "suelo") {
		int endIndex = -1, iniIndex = -1;
		if (graph != nullptr) {
			endIndex = graph->getIndex(gameObject->getParent()->transform->getWorldPosition());
			iniIndex = graph->getIndex(iniPos);
		}
		if (endIndex != -1) {
			NavigationLink navLink = NavigationLink(states, endIndex);
			if (iniIndex != -1) {
				graph->addLinkToPlatform(iniIndex, navLink);
				LOG("Link Created");
			}
		}
		states.clear();
		recording = false;
	}
}

void PathRecorder::onObjectExit(GameObject* other)
{
	if (controllerIndex == 4 && other->getTag() == "suelo" && !recording) {
		saveState(Action::None);
		iniPos = gameObject->getParent()->transform->getWorldPosition();
		recording = true;
	}
}

void PathRecorder::saveState(Action action)
{
	states.push_back(State(gameObject->getParent()->transform->getWorldPosition(), rigidBody->getLinearVelocity(), rigidBody->getTotalForce(), action));
	LOG("State Recorded");
}

