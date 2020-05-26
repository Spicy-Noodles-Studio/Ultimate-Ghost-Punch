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

AIStateMachine::AIStateMachine(GameObject* gameObject) : StateMachine(gameObject), target(nullptr), movement(nullptr), jump(nullptr), dodge(nullptr), block(nullptr), grab(nullptr), playerState(nullptr),
ghostMovement(nullptr), ghostPunch(nullptr), platformGraph(nullptr), platformNavigation(nullptr), fightingState(nullptr),
platformMovement(nullptr), ghostNavigation(nullptr), ghostManager(nullptr), attack(nullptr), avoidGhostDist(5), timerTargetChange(0), timeTargetChange(0)
{

}

AIStateMachine::~AIStateMachine()
{
	grab = nullptr;
	jump = nullptr;
	dodge = nullptr;
	block = nullptr;
	target = nullptr;
	movement = nullptr;
	ghostPunch = nullptr;
	playerState = nullptr;
	ghostMovement = nullptr;
	platformGraph = nullptr;
	fightingState = nullptr;
	platformNavigation = nullptr;
	platformMovement = nullptr;
	ghostNavigation = nullptr;
	ghostManager = nullptr;
	attack = nullptr;
}

void AIStateMachine::start()
{
	checkNullAndBreak(gameObject);

	/* GET GRAPH INFO */
	GameObject* level = findGameObjectWithName("LevelCollider");
	if (notNull(level))
		platformGraph = level->getComponent<PlatformGraph>();
	checkNull(platformGraph);

	/* GET COMPONENTS */
	std::vector<GameObject*> aux = gameObject->findChildrenWithTag("groundSensor");
	if (aux.size() > 0 && notNull(aux[0]))
	{
		jump = aux[0]->getComponent<Jump>();
		block = aux[0]->getComponent<Block>();
	}
	checkNull(jump);
	checkNull(block);

	std::vector<GameObject*> grabSensor = gameObject->findChildrenWithTag("grabSensor");
	if (grabSensor.size() > 0 && notNull(grabSensor[0]))
		grab = grabSensor[0]->getComponent<Grab>();
	checkNull(grab);

	movement = gameObject->getComponent<Movement>();
	dodge = gameObject->getComponent<Dodge>();
	ghostMovement = gameObject->getComponent<GhostMovement>();
	ghostPunch = gameObject->getComponent<UltimateGhostPunch>();
	ghostManager = gameObject->getComponent<GhostManager>();
	playerState = gameObject->getComponent<PlayerState>();

	checkNull(movement);
	checkNull(dodge);
	checkNull(ghostMovement);
	checkNull(ghostPunch);
	checkNull(ghostManager);
	checkNull(playerState);

	aux = gameObject->findChildrenWithTag("attackSensor");
	if (aux.size() > 0 && notNull(aux[0]))
		attack = aux[0]->getComponent<Attack>();
	checkNull(attack);

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

	changeTarget();

	selectPlatformState();
}

void AIStateMachine::update(float deltaTime)
{
	if (notNull(playerState) && playerState->isRespawning())
		addActionInput(ActionInput::STOP);

	if (notNull(playerState) && (playerState->isIgnoringInput() || playerState->isRespawning())) return;
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
	if (notNull(movement) && dir != Vector3::ZERO)
		movement->move(dir);
}

void AIStateMachine::onCollisionEnter(GameObject* other)
{
	if (notNull(other) && other->getTag() == "Player" && other != target)
	{
		setTarget(other);
		startFightingState();
	}
}

void AIStateMachine::startPlatformNavigation()
{
	checkNullAndBreak(platformNavigation);
	currentState = platformNavigation;
}

void AIStateMachine::startPlatformMovement()
{
	checkNullAndBreak(platformMovement);
	currentState = platformMovement;
}

void AIStateMachine::startGhostNavigation()
{
	checkNullAndBreak(ghostNavigation);
	currentState = ghostNavigation;
}

void AIStateMachine::startFightingState()
{
	checkNullAndBreak(fightingState);

	currentState = fightingState;
	fightingState->setFighting(true);
}

void AIStateMachine::startFleeingState(GameObject* fleeTarget)
{
	if (!notNull(fleeTarget)) fleeTarget = target;
	if (!notNull(fleeTarget) || !notNull(platformGraph) || !notNull(platformMovement) || !notNull(platformNavigation) || !notNull(target) ||
		!notNull(target->transform) || !notNull(gameObject) || !notNull(gameObject->transform) || !notNull(fleeTarget->transform)) return;
	// Fleeing state uses platform Navigation to move to the farthest platform away from the target
	// and platform movement to run away in the current platform
	PlatformNode node = platformGraph->getPlatforms()[platformGraph->getFurthestIndex(fleeTarget->transform->getPosition())];
	// Fleeing between platforms
	platformNavigation->setTarget(node);
	platformNavigation->setFleeing(true, fleeTarget);
	// Fleeing in the same platform
	platformMovement->setLimits(node.getBegining().x, node.getEnd().x);
	Vector3 AIpos = gameObject->transform->getPosition();
	platformMovement->setTargetPosition((fleeTarget->transform->getPosition().x <= AIpos.x) ?
		node.getEnd() - Vector3::RIGHT
		: node.getBegining() + Vector3::RIGHT);
	
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
			if (notNull(jump)) jump->jump();
			break;
		case ActionInput::CANCEL_JUMP:
			if (notNull(jump)) jump->cancelJump();
			break;
		case ActionInput::DODGE:
			if (notNull(dodge))dodge->dodge();
			break;
		case ActionInput::STOP:
			if (notNull(movement))movement->stopHorizontal();
			dir = Vector3::ZERO;
			break;
			/*GHOST*/
		case ActionInput::GHOST_MOVE:
			if (notNull(ghostMovement) && notNull(ghostNavigation))ghostMovement->move(ghostNavigation->getDirection());
			break;
		case ActionInput::GHOST_PUNCH:
			if (notNull(ghostPunch) && notNull(ghostNavigation)) {
				ghostPunch->aim(ghostNavigation->getDirection().x, ghostNavigation->getDirection().y);//Provisional?
				ghostPunch->ghostPunch();
			}
			break;
			/* ATTACK */
		case ActionInput::QUICK_ATTACK:
			if (notNull(attack)) attack->quickAttack();
			break;
		case ActionInput::STRONG_ATTACK:
			if (notNull(attack)) attack->strongAttack();
			break;
		case ActionInput::BLOCK:
			if (notNull(block))
				block->block();
			break;
		case ActionInput::UNBLOCK:
			if (notNull(block))
				block->unblock();
			break;
		case ActionInput::GRAB:
			if (notNull(grab))
				grab->chargeGrab();
			break;
		case ActionInput::DROP:
			if (notNull(grab))
				grab->drop();
			break;
		case ActionInput::FACE_RIGHT:
			dir = Vector3::RIGHT;
			if (notNull(movement))
				movement->move(dir);
			dir = Vector3::ZERO;
			break;
		case ActionInput::FACE_LEFT:
			dir = Vector3::NEGATIVE_RIGHT;
			if (notNull(movement))
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
	checkNullAndBreak(platformNavigation);

	addStateAction(platformNavigation);
	platformNavigation->setPlatformGraph(platformGraph);
	platformNavigation->setCharacter(gameObject);
}

void AIStateMachine::createPlatformMovement()
{
	platformMovement = new PlatformMovement(this);
	checkNullAndBreak(platformMovement);

	addStateAction(platformMovement);
	platformMovement->setPlatformGraph(platformGraph);
	platformMovement->setTargetPosition(Vector3::ZERO);
	platformMovement->setCharacter(gameObject);
}

void AIStateMachine::createGhostNavigation()
{
	ghostNavigation = new GhostNavigation(this);
	checkNullAndBreak(ghostNavigation);

	addStateAction(ghostNavigation);
	ghostNavigation->setCharacter(gameObject);
}

void AIStateMachine::createFightingState()
{
	fightingState = new FightingState(this);
	checkNullAndBreak(fightingState);

	addStateAction(fightingState);
	fightingState->setCharacter(gameObject);
}

void AIStateMachine::changeTarget()
{
	if ((notNull(fightingState) && fightingState->isFighting()) || (notNull(currentState) && notNull(ghostNavigation) && currentState == ghostNavigation) ||
		(notNull(currentState) && notNull(platformNavigation) && currentState == platformNavigation && platformNavigation->isFleeing())) // Do not change target while fighting or fleeing
		return;

	checkNullAndBreak(GameManager::GetInstance());
	std::vector<GameObject*> alive = GameManager::GetInstance()->getAlivePlayers(true);
	int size = alive.size();
	// Check if only the AI is alive
	if (size <= 1) return;

	int index = rand() % size;
	if (notNull(alive[index]))
		target = alive[index];
	checkNullAndBreak(target);

	if (target == gameObject)
	{
		index = (index + 1) % size;
		if (notNull(alive[index]))
			target = alive[index];
	}

	setTarget(target);
}

void AIStateMachine::setTarget(GameObject* newTarget)
{
	checkNullAndBreak(newTarget);

	target = newTarget;
	// TO STUFF
	if (notNull(platformNavigation)) platformNavigation->setTarget(newTarget);
	if (notNull(ghostNavigation)) ghostNavigation->setTarget(newTarget);
	if (notNull(fightingState)) fightingState->setTarget(newTarget);

	if (notNull(platformGraph) && notNull(platformMovement) && notNull(newTarget->transform)) {
		int index = platformGraph->getIndex(newTarget->transform->getPosition());
		if (index < 0) return;
		PlatformNode node = platformGraph->getPlatforms()[index];
		platformMovement->setLimits(node.getBegining().x, node.getEnd().x);
		platformMovement->setTargetPosition(newTarget->transform->getPosition());
	}
}

void AIStateMachine::selectPlatformState()
{
	checkNullAndBreak(platformNavigation);

	if (platformNavigation->hasArrived()) 
		currentState = platformMovement; // Target in the current platform
	else
		currentState = platformNavigation; // Target in different platform
}

void AIStateMachine::updateState()
{
	// Enter fleeing mode
	checkNullAndBreak(GameManager::GetInstance());

	GameObject* ghost = GameManager::GetInstance()->getAnyGhost();
	if (notNull(ghostManager) && !ghostManager->isGhost() && ghost != nullptr) //If im not a ghost and there is a ghost on the match
	{
		startFleeingState(ghost);
		return;
	}

	// Enter / Exit ghost movement
	if (notNull(ghostManager) && notNull(currentState)) {
		if (ghostManager->isGhost() && currentState != ghostNavigation)
			startGhostNavigation();
		if (!ghostManager->isGhost() && currentState == ghostNavigation)
			startPlatformNavigation();
	}

}