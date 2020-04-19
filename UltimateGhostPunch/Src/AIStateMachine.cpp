#include "AIStateMachine.h"
#include <ComponentRegister.h>
#include <GameObject.h>

#include "PlatformNavigation.h"
#include "GameManager.h"
#include "Movement.h"
#include "Jump.h"
#include "Dodge.h"

REGISTER_FACTORY(AIStateMachine);

AIStateMachine::AIStateMachine(GameObject* gameObject) : StateMachine(gameObject), movement(nullptr), jump(nullptr), dodge(nullptr)
{

}

AIStateMachine::~AIStateMachine()
{

}

void AIStateMachine::start()
{
	//Get Components
	movement = gameObject->getComponent<Movement>();
	std::vector<GameObject*> aux = gameObject->findChildrenWithTag("groundSensor");
	if (aux.size() > 0) jump = aux[0]->getComponent<Jump>();
	dodge = gameObject->getComponent<Dodge>();

	// Create states here

	/* MOVING PLATFORM STATE ACTION */
	createMovingPlatformsAction();
}

void AIStateMachine::fixedUpdate(float deltaTime)
{
	if (movement != nullptr && dir != Vector3::ZERO) 
		movement->move(dir);
}

void AIStateMachine::processActionInput()
{
	for (auto input : actionInputs) {
		switch (input)
		{
			/* MOVEMENT */
		case ActionInput::MOVE_RIGHT:
			dir = Vector3::RIGHT;
			break;
		case ActionInput::MOVE_LEFT:
			dir = Vector3::NEGATIVE_RIGHT;
			break;
		case ActionInput::JUMP:
			if (jump != nullptr) jump->jump();
			break;
		case ActionInput::CANCEL_JUMP:
			if (jump != nullptr) jump->cancelJump();
			break;
		case ActionInput::DODGE:
			if (dodge != nullptr)dodge->dodge();
			break;
		case ActionInput::STOP:
			if (movement != nullptr)movement->stop();
			dir = Vector3::ZERO;
			break;
			/* ATTACK */
		default:
			LOG("ActionInput no procesado");
			dir = Vector3::ZERO;
			break;
		}
	}
}

void AIStateMachine::createMovingPlatformsAction()
{
	PlatformNavigation* platformNavigation = new PlatformNavigation(this);
	addStateAction(platformNavigation);

	/* ADD MORE DATA IF NEEDED */
	/* GRAPH DATA */
	GameObject* aux = findGameObjectWithName(GameManager::GetInstance()->getLastLevel());
	if (aux != nullptr) {
		PlatformGraph* platformGraph = aux->getComponent<PlatformGraph>(); //TODO: gestion de errores (ademas esta feo)
		platformNavigation->setPlatformGraph(platformGraph);
	}
	platformNavigation->setCharacter(gameObject);

	// TODO: quitar cuando se unifiquen las IAs
	currentState = platformNavigation;
}
