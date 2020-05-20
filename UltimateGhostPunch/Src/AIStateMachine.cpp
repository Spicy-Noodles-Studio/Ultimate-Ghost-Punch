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
#include "Health.h"
#include "GhostMovement.h"
#include "UltimateGhostPunch.h"
#include "Attack.h"
#include "FightingState.h"
#include "Block.h"
#include "Grab.h"
#include "PlayerState.h"

REGISTER_FACTORY(AIStateMachine);

AIStateMachine::AIStateMachine(GameObject* gameObject) :	StateMachine(gameObject), target(nullptr), movement(nullptr), jump(nullptr), dodge(nullptr),
															ghostMovement(nullptr), ghostPunch(nullptr), platformGraph(nullptr), platformNavigation(nullptr),
															platformMovement(nullptr), ghostNavigation(nullptr), ghostManager(nullptr), knights(nullptr), attack(nullptr), avoidGhostDist(5)
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
	if (aux.size() > 0)
	{
		jump = aux[0]->getComponent<Jump>();
		block = aux[0]->getComponent<Block>();
	}
	std::vector<GameObject*> grabSensor = gameObject->findChildrenWithTag("grabSensor");
	if (grabSensor.size() > 0)
		grab = grabSensor[0]->getComponent<Grab>();
	dodge = gameObject->getComponent<Dodge>();
	ghostMovement = gameObject->getComponent<GhostMovement>();
	ghostPunch = gameObject->getComponent<UltimateGhostPunch>();
	ghostManager = gameObject->getComponent<GhostManager>();
	playerState = gameObject->getComponent<PlayerState>();

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

	//currentState = platformMovement; // By default

	changeTarget();

	selectPlatformState();
}

void AIStateMachine::update(float deltaTime)
{
	if (playerState->isRespawning())
	{
		addActionInput(ActionInput::STOP);
	}

	if (playerState != nullptr && (playerState->isIgnoringInput() || playerState->isRespawning())) return;
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

void AIStateMachine::onCollisionEnter(GameObject* other)
{
	if (other->getTag() == "Player" && other != target)
	{
		setTarget(other);
		startFightingState();
	}
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
	fightingState->setFighting(true);
}

void AIStateMachine::startFleeingState(GameObject* fleeTarget)
{
	if (fleeTarget == nullptr) fleeTarget = target;
	if (fleeTarget == nullptr) return;
	// Fleeing state uses platform Navigation to move to the farthest platform away from the target
	// and platform movement to run away in the current platform
	PlatformNode node = platformGraph->getPlatforms()[platformGraph->getFurthestIndex(target->transform->getPosition())];
	// Fleeing between platforms
	platformNavigation->setTarget(node);
	platformNavigation->setFleeing(true);
	// Fleeing in the same platform
	platformMovement->setLimits(node.getBegining().x, node.getEnd().x);
	Vector3 AIpos = gameObject->transform->getPosition();
	platformMovement->setTargetPosition((fleeTarget->transform->getPosition().x <= AIpos.x) ? 
		node.getEnd() - Vector3(1,0,0)
		: node.getBegining() + Vector3(1, 0, 0));

	selectPlatformState();
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
				ghostPunch->aim(ghostNavigation->getDirection().x, ghostNavigation->getDirection().y);//Provisional?
				ghostPunch->ghostPunch();
				}
			break;
			/* ATTACK */
		case ActionInput::QUICK_ATTACK:
			if (attack != nullptr && !attack->attackOnCD() && !attack->isAttacking()) attack->quickAttack();
			break;
		case ActionInput::STRONG_ATTACK:
			if (attack != nullptr && !attack->attackOnCD() && !attack->isAttacking()) attack->strongAttack();
			break;
		case ActionInput::BLOCK:
			if (block != nullptr) 
				block->block();
			break;
		case ActionInput::UNBLOCK:
			if (block != nullptr)
				block->unblock();
			break;
		case ActionInput::GRAB:
			if (grab != nullptr)
				grab->grab();
			break;
		case ActionInput::DROP:
			if (grab != nullptr)
				grab->drop();
			break;
		case ActionInput::FACE_RIGHT:
			dir = Vector3::RIGHT;
			if (movement != nullptr)
				movement->move(dir);
			dir = Vector3::ZERO;
			break;
		case ActionInput::FACE_LEFT:
			dir = Vector3::NEGATIVE_RIGHT;
			if (movement != nullptr)
				movement->move(dir);
			dir = Vector3::ZERO;
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

	fightingState->setCharacter(gameObject);
}

void AIStateMachine::changeTarget()
{
	if (fightingState->isFighting() || (currentState == platformNavigation && platformNavigation->isFleeing())) // Do not change target while fighting or fleeing
	{
		return;
	}

	// TODO: de momento es random, cambiar si se quiere
	std::vector<GameObject*> alive = GameManager::GetInstance()->getAlivePlayers();
	int size = alive.size();
	// Check if only the AI is alive
	if (size <= 1) return;

	int index = rand() % size;
	target = alive[index];
	if (target == gameObject)
	{
		index = (index + 1) % size;
		target = alive[index];
	}

	setTarget(target);
}

void AIStateMachine::setTarget(GameObject* newTarget)
{
	target = newTarget;
	// TO STUFF
	if (platformNavigation != nullptr) platformNavigation->setTarget(newTarget);
	if (ghostNavigation != nullptr) ghostNavigation->setTarget(newTarget);
	if (fightingState != nullptr) fightingState->setTarget(newTarget);

	// TODO: cambiar el cambio de estado
	if (platformGraph != nullptr && platformMovement != nullptr) {
		int index = platformGraph->getIndex(newTarget->transform->getPosition());
		if (index < 0) return;
		PlatformNode node = platformGraph->getPlatforms()[index];
		platformMovement->setLimits(node.getBegining().x, node.getEnd().x);
		platformMovement->setTargetPosition(newTarget->transform->getPosition());
	}
}

void AIStateMachine::selectPlatformState()
{
	if (target == nullptr || platformGraph == nullptr) return;
	if (platformGraph->getFurthestIndex(target->transform->getPosition()) == platformGraph->getClosestIndex(gameObject->transform->getPosition()))
		currentState = platformMovement; // Target in the current platform
	else
		currentState = platformNavigation; // Target in different platform
}

void AIStateMachine::updateState()
{
	// Enter fleeing mode
	GameObject* ghost = GameManager::GetInstance()->getAnyGhost();
	if (ghostManager != nullptr && !ghostManager->isGhost() && ghost != nullptr)
	{
		float ghostDist = (ghost->transform->getPosition() - gameObject->transform->getPosition()).magnitude();
		if (ghostDist <= avoidGhostDist)
		{
			jump->jump();
			fightingState->turnBackOnTarget();
			addActionInput(ActionInput::DODGE);
		}

		startFleeingState(ghost);
		return;
	}

	// Enter / Exit ghost movement
	if (ghostManager != nullptr) {
		if (ghostManager->isGhost() && currentState != ghostNavigation)
			startGhostNavigation();
		if (!ghostManager->isGhost() && currentState == ghostNavigation)
			startPlatformNavigation();
	}

}
