#include "AIStateMachine.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <MathUtils.h>

#include "PlatformNavigation.h"
#include "GhostNavigation.h"
#include "GameManager.h"
#include "Movement.h"
#include "Jump.h"
#include "Dodge.h"
#include "GhostManager.h"
#include "GhostMovement.h"
#include "UltimateGhostPunch.h"

REGISTER_FACTORY(AIStateMachine);

AIStateMachine::AIStateMachine(GameObject* gameObject) :	StateMachine(gameObject), target(nullptr), movement(nullptr), jump(nullptr), dodge(nullptr),
															ghostMovement(nullptr), ghostPunch(nullptr), platformNavigation(nullptr), ghostNavigation(nullptr),
															ghostManager(nullptr), knights(nullptr)
{

}

AIStateMachine::~AIStateMachine()
{

}

void AIStateMachine::start()
{
	/* GET ALL KNIGHTS */
	knights = &GameManager::GetInstance()->getKnights();

	//Get Components
	movement = gameObject->getComponent<Movement>();
	std::vector<GameObject*> aux = gameObject->findChildrenWithTag("groundSensor");
	if (aux.size() > 0) jump = aux[0]->getComponent<Jump>();
	dodge = gameObject->getComponent<Dodge>();
	ghostMovement = gameObject->getComponent<GhostMovement>();
	ghostPunch = gameObject->getComponent<UltimateGhostPunch>();
	ghostManager = gameObject->getComponent<GhostManager>();

	// Create states here

	/* MOVING PLATFORM STATE ACTION */
	createMovingPlatformsAction();

	/*GHOST CONTROL ACTION*/
	createGhostAction();

	// Initialize auxialiar variables
	timeTargetChange = 5.0f; // 5 seconds
}

void AIStateMachine::update(float deltaTime)
{
	StateMachine::update(deltaTime);

	timerTargetChange += deltaTime;
	if (timerTargetChange >= timeTargetChange) {
		timerTargetChange = 0.0f;
		if (platformNavigation == nullptr) return;
		changeTarget();
	}

	updateState();
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
			/*GHOST*/
		case ActionInput::GHOST_MOVE:
			if (ghostMovement != nullptr && ghostNavigation != nullptr)ghostMovement->move(ghostNavigation->getDirection());
			break;
		case ActionInput::GHOST_PUNCH:
			if (ghostPunch != nullptr){
				ghostPunch->aim(ghostNavigation->getDirection());//Provisional?
				ghostPunch->ghostPunch();
				}
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
	platformNavigation = new PlatformNavigation(this);
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

void AIStateMachine::createGhostAction()
{
	ghostNavigation = new GhostNavigation(this);
	addStateAction(ghostNavigation);

	ghostNavigation->setCharacter(gameObject);
}

void AIStateMachine::changeTarget()
{
	// TODO: de momento es random, cambiar si se quiere
	int size = knights->size();

	do {
		target = knights->at(rand() % size);
	} while (target == gameObject);

	// TO STUFF
	platformNavigation->setTarget(target);
	ghostNavigation->setTarget(target);
}

void AIStateMachine::updateState()
{
	if (ghostManager != nullptr) {
		if (ghostManager->isGhost() && currentState != ghostNavigation) 
			currentState = ghostNavigation;
	}
}
