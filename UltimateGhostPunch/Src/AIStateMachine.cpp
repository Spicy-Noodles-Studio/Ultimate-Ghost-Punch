#include "AIStateMachine.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <MathUtils.h>

#include "PlatformNavigation.h"
#include "PlatformMovement.h"
#include "GhostNavigation.h"

#include "GameManager.h"
#include "Movement.h"
#include "Jump.h"
#include "Dodge.h"
#include "GhostManager.h"
#include "GhostMovement.h"
#include "UltimateGhostPunch.h"
#include "Attack.h"
#include "FightingState.h"

REGISTER_FACTORY(AIStateMachine);

AIStateMachine::AIStateMachine(GameObject* gameObject) :	StateMachine(gameObject), target(nullptr), movement(nullptr), jump(nullptr), dodge(nullptr),
															ghostMovement(nullptr), ghostPunch(nullptr), platformGraph(nullptr), platformNavigation(nullptr),
															platformMovement(nullptr), ghostNavigation(nullptr), ghostManager(nullptr), knights(nullptr), attack(nullptr)
{

}

AIStateMachine::~AIStateMachine()
{

}

void AIStateMachine::start()
{
	/* GET ALL KNIGHTS */
	knights = &GameManager::GetInstance()->getKnights();

	/* GET GRAPH INFO */
	GameObject* level = findGameObjectWithName("LevelCollider");
	if (level != nullptr) {
		platformGraph = level->getComponent<PlatformGraph>();
	}
	/* GET COMPONENTS */
	movement = gameObject->getComponent<Movement>();
	std::vector<GameObject*> aux = gameObject->findChildrenWithTag("groundSensor");
	if (aux.size() > 0) jump = aux[0]->getComponent<Jump>();
	dodge = gameObject->getComponent<Dodge>();
	ghostMovement = gameObject->getComponent<GhostMovement>();
	ghostPunch = gameObject->getComponent<UltimateGhostPunch>();
	ghostManager = gameObject->getComponent<GhostManager>();

	aux = gameObject->findChildrenWithTag("attackSensor");
	if (aux.size() > 0)
		attack = aux[0]->getComponent<Attack>();

	// Create states here
	/* PLATFORM NAVIGATION STATE */
	createPlatformNavigation();

	/* PLATFORM MOVEMENT STATE */
	createPlatformMovement();

	/* GHOST NAVIGATION STATE */
	createGhostNavigation();

	/*CREATE COMBAT STATE*/
	createFightingState();

	// Initialize auxiliar variables
	timeTargetChange = 3.0f; // 5 seconds

	currentState = platformMovement; // By default

	changeTarget();
}

void AIStateMachine::update(float deltaTime)
{
	StateMachine::update(deltaTime);

	timerTargetChange += deltaTime;
	if (timerTargetChange >= timeTargetChange) {
		timerTargetChange = 0.0f;
		changeTarget();
	}

	updateState();
}

void AIStateMachine::fixedUpdate(float deltaTime)
{
	if (movement != nullptr && dir != Vector3::ZERO) 
		movement->move(dir);
}

void AIStateMachine::startPlatformNavigation()
{
	currentState = platformNavigation;
}

void AIStateMachine::startPlatformMovement()
{
	currentState = platformMovement;
}

void AIStateMachine::startGhostNavigation()
{
	currentState = ghostNavigation;
}

void AIStateMachine::startFightingState()
{
	currentState = fightingState;
}

void AIStateMachine::processActionInput()
{
	dir = Vector3::ZERO;
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
		case ActionInput::QUICK_ATTACK:
			if (attack != nullptr) attack->quickAttack();
			break;
		case ActionInput::STRONG_ATTACK:
			if (attack != nullptr) attack->strongAttack();
			break;

		default:
			LOG("ActionInput no procesado");
			break;
		}
	}
}

void AIStateMachine::createPlatformNavigation()
{
	platformNavigation = new PlatformNavigation(this);
	addStateAction(platformNavigation);

	/* ADD MORE DATA IF NEEDED */
	/* GRAPH DATA */
	platformNavigation->setPlatformGraph(platformGraph);
	platformNavigation->setCharacter(gameObject);
}

void AIStateMachine::createPlatformMovement()
{
	platformMovement = new PlatformMovement(this);
	addStateAction(platformMovement);

	platformMovement->setPlatformGraph(platformGraph);
	platformMovement->setTargetPosition(Vector3::ZERO);
	platformMovement->setCharacter(gameObject);
}

void AIStateMachine::createGhostNavigation()
{
	ghostNavigation = new GhostNavigation(this);
	addStateAction(ghostNavigation);

	ghostNavigation->setCharacter(gameObject);
}

void AIStateMachine::createFightingState()
{
	fightingState = new FightingState(this);
	addStateAction(fightingState);
}

void AIStateMachine::changeTarget()
{
	// TODO: de momento es random, cambiar si se quiere
	int size = knights->size();

	do {
		target = knights->at(rand() % size);
	} while (target == gameObject);

	// TO STUFF
	if(platformNavigation != nullptr) platformNavigation->setTarget(target);
	if(ghostNavigation != nullptr) ghostNavigation->setTarget(target);

	// TODO: cambiar el cambio de estado
	if (platformGraph != nullptr && platformMovement != nullptr) {
		int index = platformGraph->getIndex(target->transform->getPosition());		
		if (index < 0) return;
		PlatformNode node = platformGraph->getPlatforms()[index];
		platformMovement->setLimits(node.getBegining().x, node.getEnd().x);
		platformMovement->setTargetPosition(target->transform->getPosition());
	}
}

void AIStateMachine::updateState()
{
	if (ghostManager != nullptr) {
		if (ghostManager->isGhost() && currentState != ghostNavigation)
			startGhostNavigation();
		if (!ghostManager->isGhost() && currentState == ghostNavigation)
			startPlatformNavigation();
	}
}
