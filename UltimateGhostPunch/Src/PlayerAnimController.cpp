#include "PlayerAnimController.h"
#include <ComponentRegister.h>
#include <InputSystem.h>
#include <GameObject.h>
#include <Animator.h>
#include <MeshRenderer.h>
#include <RigidBody.h>
#include <sstream>


#include "PlayerController.h"
#include "PlayerFX.h"
#include "Attack.h"
#include "Jump.h"
#include "Health.h"
#include "Dodge.h"
#include "Grab.h"
#include "Block.h"
#include "GhostManager.h"
#include "UltimateGhostPunch.h"
#include "GhostManager.h"
#include "GameManager.h"
#include "PlayerState.h"

REGISTER_FACTORY(PlayerAnimController);

PlayerAnimController::PlayerAnimController(GameObject* gameObject) : UserComponent(gameObject), inputSystem(nullptr), mesh(nullptr), body(nullptr), anim(nullptr), jump(nullptr),
grab(nullptr), block(nullptr), playerFX(nullptr), ghostManag(nullptr), state(IDLE), runThreshold(0.50f), fallThreshold(1.0f), swordState(HAND), delayedAnimations(),
thrownDelay(0.35f), currentMode(ALIVE), aliveMeshId(""), aliveMeshName(""), ghostMeshId(""), ghostMeshName(""), playerState(nullptr)
{

}

PlayerAnimController::~PlayerAnimController()
{

}

void PlayerAnimController::start()
{
	inputSystem = InputSystem::GetInstance();
	anim = gameObject->getComponent<Animator>();
	body = gameObject->getComponent<RigidBody>();
	mesh = gameObject->getComponent<MeshRenderer>();
	ghostManag = gameObject->getComponent<GhostManager>();
	playerFX = gameObject->getComponent<PlayerFX>();
	playerState = gameObject->getComponent<PlayerState>();

	std::vector<GameObject*> aux = gameObject->findChildrenWithTag("groundSensor");
	if (aux.size() > 0)
	{
		jump = aux[0]->getComponent<Jump>();
		block = aux[0]->getComponent<Block>();
	}

	aux = gameObject->findChildrenWithTag("grabSensor");
	if (aux.size() > 0)
		grab = aux[0]->getComponent<Grab>();

	if (anim == nullptr)
		LOG("ERROR: Animator component not found in player.\n");
	else
		anim->printAllAnimationsNames();

	if (mesh == nullptr)
		LOG("ERROR: MeshRenderer component not found in player.\n");
	else
	{
		mesh->printAllBones();
		aliveMeshId = mesh->getMeshId();
		aliveMeshName = mesh->getMeshName();
	}
}

void PlayerAnimController::postUpdate(float deltaTime)
{	
	manageAnimations();

	/*
	// Update the current state
	updateState();

	// Handle the current state
	handleState();

	anim->updateAnimationSequence();

	// Update sword position if necessary
	if (swordState != HAND && state != GRABBING && anim->getCurrentAnimation() != "GrabFail")
	{
		// Move sword back to hand
		mesh->moveEntityToBone("player", "Mano.L", "sword");
		swordState = HAND;
	}

	// Update delayed animations
	updateDelayedAnimations(deltaTime);*/
}

void PlayerAnimController::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "runThreshold")
		{
			setFloat(runThreshold);
		}
		else if (prop.first == "strongCooldown")
		{
			setFloat(fallThreshold);
		}
		else if (prop.first == "thrownDelay")
		{
			setFloat(thrownDelay);
		}
		else if (prop.first == "ghostMesh")
		{
			if (!(ss >> ghostMeshId >> ghostMeshName))
				LOG("GHOST MANAGER: Invalid property with name \"%s\"", prop.first.c_str());
		}
	}
}

void PlayerAnimController::manageAnimations()
{
	if (true/*condiciones que determinen si se esta vivo o muerto*/) {
		manageKnightAnimations();
	}
	else {
		manageGhostAnimations();
	}
}

void PlayerAnimController::manageKnightAnimations()
{
	if (manageGroundedAnimations());
	else if (manageAirAnimations());
}

void PlayerAnimController::manageGhostAnimations()
{
}

bool PlayerAnimController::manageGroundedAnimations()
{
	bool result = false;
	if (playerState->isGrounded()) {
		// El ORDEN IMPORTA, las de mas arriba son mas prioritarias
		if (result = manageHurtAnimation());
		else if (result = manageGrabAnimations());
		else if (result = manageBlockAnimations());
		else if (result = manageGroundedAttackAnimation());
		else if (result = manageJumpAnimation());
		else if (result = manageLandAnimation());
		else if (result = manageIdleAnimation());
		else if (result = manageDashAnimation());
		else if (result = manageRunAnimation());
	}
	return result;
}

bool PlayerAnimController::manageAirAnimations()
{
	bool result = false;
	if (!playerState->isGrounded()) {
		// El ORDEN IMPORTA, las de mas arriba son mas prioritarias
		if (result = manageHurtAnimation());
		else if (result = manageAirAttackAnimation());
		else if (result = manageDashAnimation());
		else if (result = manageFallAnimation());
	}
	return result;
}

bool PlayerAnimController::manageJumpAnimation()
{
	if (playerState->hasJumped()) { 
		if (manageGrabJumpAnimation()) return true;

		anim->playAnimation("JumpStart");
		anim->setLoop(false);
		return true;
	}
	return false;
}

bool PlayerAnimController::manageRunAnimation()
{
	if (playerState->isMoving() && playerState->canMove()) {
		if (manageGrabRunAnimation()) return true;

		anim->playAnimation("Run");
		anim->setLoop(true);
		return true;
	}
	return false;
}

bool PlayerAnimController::manageIdleAnimation()
{
	if (!playerState->isMoving() && playerState->canMove()) {
		if (manageGrabIdleAnimation()) return true;

		anim->playAnimation("Idle");
		anim->setLoop(true);
		return true;
	}
	return false;
}

bool PlayerAnimController::manageLandAnimation()
{
	if (playerState->hasLanded()) {
		if (manageGrabLandAnimation()) return true;

		anim->playAnimation("Land");
		anim->setLoop(false);
		return true;
	}

	// Evita interrupcion de animaciones de menor prioridad
	std::string animationName = anim->getCurrentAnimation();
	return (animationName == "Land" || animationName == "LandGrabbing") && !anim->hasEnded();
}

bool PlayerAnimController::manageGroundedAttackAnimation()
{
	if (playerState->isQuickAttacking()) {
		anim->playAnimation("AttackA");
		anim->setLoop(false);
		return true;
	}
	else if (playerState->isHeavyAttacking()) {
		anim->playAnimation("AttackB");
		anim->setLoop(false);
		return true;
	}

	std::string animationName = anim->getCurrentAnimation();
	return (animationName == "AttackA" || animationName == "AttackB") && !anim->hasEnded();
}

bool PlayerAnimController::manageBlockAnimations()
{
	std::string currentAnimation = anim->getCurrentAnimation();
	// Transicion de Empieza bloqueo -> Estar bloqueando
	if (playerState->isBlocking() || playerState->hasBlockedGrab()) {
		bool previouslyBlocking =	currentAnimation == "BlockStart"	||
									currentAnimation == "BlockHold"		||
									currentAnimation == "BlockAttack"	||
									currentAnimation == "BlockGrab";
		// ESTOS DOS SON MAS PRIORITARIOS A QUE EMPIEZE LA ANIMACION DE TRANSICION
		if (manageBlockAttackAnimation()) return true;
		if (manageBlockGrabAnimation()) return true;

		// TRANSICIONES NORMALES
		else if (!previouslyBlocking) {
			anim->playAnimation("BlockStart");
			anim->setLoop(false);
		}
		else if (previouslyBlocking) {
			//Si era el comienzo, esperamos a que termine
			if (currentAnimation == "BlockStart" && !anim->hasEnded()) return true;

			anim->playAnimation("BlockHold");
			anim->setLoop(true);
		}

		return true;
	}
	// Si no esta bloqueando, pero lo estaba
	else if (currentAnimation == "BlockStart" || currentAnimation == "BlockHold") {
		anim->playAnimation("BlockEnd");
		anim->setLoop(false);
		return true;
	}

	return (currentAnimation == "BlockEnd" || currentAnimation == "Knockback") && !anim->hasEnded();
}

bool PlayerAnimController::manageBlockAttackAnimation()
{
	if (playerState->hasBlocked()) {
		anim->playAnimation("BlockAttack");
		anim->setLoop(false);
		return true;
	}
	return anim->getCurrentAnimation() == "BlockAttack" && !anim->hasEnded();
}

bool PlayerAnimController::manageBlockGrabAnimation()
{
	// Bloquea un agarre
	if (playerState->hasBlockedGrab()) {
		LOG("BLOCKED GRAB");
		anim->playAnimation("Knockback");
		anim->setLoop(false);
		return true;
	}
	return anim->getCurrentAnimation() == "Knockback" && !anim->hasEnded();
}

bool PlayerAnimController::manageGrabAnimations()
{
	// TODO: ESTO SOLO TIENE SENTIDO SI SE ESTA EN IDLE
	// TODO: cambiar condiciones a algo mas logico
	/*if (playerState->hasMissedGrab()) {
		anim->playAnimation("GrabStart");
		anim->setLoop(false);
		return true;
	}
	else if (playerState->hasMissedGrab()) {
		anim->playAnimation("GrabFail");
		anim->setLoop(false);
		return true;
	}

	std::string animationName = anim->getCurrentAnimation();*/
	return false; // (animationName == "GrabStart" || animationName == "GrabFail") && !anim->hasEnded();;
}

bool PlayerAnimController::manageGrabIdleAnimation()
{
	if (playerState->isGrabbing()) {
		anim->playAnimation("GrabHold");
		anim->setLoop(true);
		return true;
	}
	return false;
}

bool PlayerAnimController::manageGrabRunAnimation()
{
	if (playerState->isGrabbing()) {
		anim->playAnimation("RunGrabbing");
		anim->setLoop(true);
		return true;
	}
	return false;
}

bool PlayerAnimController::manageGrabJumpAnimation()
{
	if (playerState->isGrabbing()) {
		anim->playAnimation("JumpStartGrabbing");
		anim->setLoop(false);
		return true;
	}
	return false;
}

bool PlayerAnimController::manageGrabLandAnimation()
{
	if (playerState->isGrabbing()) {
		anim->playAnimation("LandGrabbing");
		anim->setLoop(false);
		return true;
	}

	return false;
}

bool PlayerAnimController::manageFallAnimation()
{
	bool result = false;
	if (playerState->isFalling()) {
		if (manageGrabFallAnimation()) return true;

		// Caer tras un salto (hacemos transicion)
		if (anim->getCurrentAnimation() == "JumpStart" && anim->hasEnded()) {
			anim->playAnimation("JumpChange");
			anim->setLoop(false);
		}
		else if (anim->getCurrentAnimation() == "JumpChange") {
			if (anim->hasEnded()) {	// Que la condicion este dentro, evita que se salte la transicion
				anim->playAnimation("Fall");
				anim->setLoop(true);
			}
		}
		// Caer sin mas (tras un dash por ejemplo)
		else {
			anim->playAnimation("Fall");
			anim->setLoop(true);
		}
		result = true;
	}
	return result;
}

bool PlayerAnimController::manageGrabFallAnimation()
{
	bool result = false;
	if (playerState->isGrabbing()) {
		// Caer tras un salto (hacemos transicion)
		if (anim->getCurrentAnimation() == "JumpStartGrabbing" && anim->hasEnded()) {
			anim->playAnimation("JumpChangeGrabbing");
			anim->setLoop(false);
		}
		else if (anim->getCurrentAnimation() == "JumpChangeGrabbing") {
			if (anim->hasEnded()) {
				anim->playAnimation("FallGrabbing");
				anim->setLoop(true);
			}
		}
		else {
			anim->playAnimation("FallGrabbing");
			anim->setLoop(true);
		}
		result = true;
	}
	return result;
}

bool PlayerAnimController::manageAirAttackAnimation()
{
	if (playerState->isQuickAttacking()) {
		anim->playAnimation("AttackAAir");
		anim->setLoop(false);
		return true;
	}
	else if (playerState->isHeavyAttacking()) {
		anim->playAnimation("AttackB");
		anim->setLoop(false);
		return true;
	}

	std::string animationName = anim->getCurrentAnimation();
	return (animationName == "AttackAAir" || animationName == "AttackB") && !anim->hasEnded();
}

bool PlayerAnimController::manageDashAnimation()
{
	if (playerState->isDodging()) {
		if (manageGrabDashAnimation()) return true;

		anim->playAnimation("DashFront");
		anim->setLoop(false);
		return true;
	}
	// Evitar interrupcion de prioridades menores
	std::string animationName = anim->getCurrentAnimation();
	return (animationName == "DashFront" || animationName == "DashFrontGrabbing") && !anim->hasEnded();
}

bool PlayerAnimController::manageGrabDashAnimation()
{
	if (playerState->isGrabbing()) {
		anim->playAnimation("DashFrontGrabbing");
		anim->setLoop(false);
		return true;
	}
	return false;
}

bool PlayerAnimController::manageHurtAnimation()
{
	if (playerState->isHurt()) {
		anim->playAnimation("Hurt");
		anim->setLoop(false);
		return true;
	}
	return anim->getCurrentAnimation() == "Hurt" && !anim->hasEnded();
}


/******************************
	ANIMATION METHODS
*******************************/

void PlayerAnimController::jumpAnimation() //  JUMP ANIMATION //
{
	if (anim->getCurrentAnimation() == "JumpStart" || anim->getCurrentAnimation() == "JumpChange" || anim->getCurrentAnimation() == "Fall")
		return;

	if (state != GRABBING)
	{
		anim->playAnimation("JumpStart");
		anim->setLoop(false);
		state = JUMP;
	}
	else
	{
		anim->playAnimation("JumpStartGrabbing");
		anim->setLoop(false);
	}
}

void PlayerAnimController::dashAnimation() // DASH ANIMATION //
{
	if (state != GRABBING)
		notLoopAnimation("DashFront");
	else
	{
		anim->playAnimation("DashFrontGrabbing");
		anim->setLoop(false);
	}
}

void PlayerAnimController::tauntAnimation() // TAUNT ANIMATION //
{
	if (state != IDLE) return;

	notLoopAnimation("Taunt");
}

void PlayerAnimController::hurtAnimation() //  HURT ANIMATION //
{
	anim->playAnimation("Hurt");
	anim->setLoop(false);

	if (jump->isGrounded())
		state = NOT_LOOPING_STATE;
	else
		state = FALL;

	playerFX->activateHurt();
}

void PlayerAnimController::stunnedAnimation() // STUNNED ANIMATION //
{
	if (anim->getCurrentAnimation() == "StunnedStart" || anim->getCurrentAnimation() == "StunnedHold")
		return;

	anim->playAnimationSequence({ "StunnedStart", "StunnedHold" }, true);
	anim->setLoop(false);
	state = STUNNED;
}

void PlayerAnimController::resurrectAnimation() // RESURRECT ANIMATION //
{
	notLoopAnimation("Resurrect");
}

void PlayerAnimController::quickAttackAnimation() //  QUICK ATTACK ANIMATION //
{
	attackAnimation(0);
}

void PlayerAnimController::strongAttackAnimation() //  STRONG ATTACK ANIMATION //
{
	attackAnimation(1);
}

void PlayerAnimController::grabAnimation() //  GRAB ANIMATION //
{
	if (anim->getCurrentAnimation() == "GrabStart" || anim->getCurrentAnimation() == "GrabHold")
		return;

	anim->playAnimation("GrabStart");
	anim->setLoop(false);
	state = GRABBING;

	// Move Sword to back
	mesh->moveEntityToBone("player", "Espalda", "sword");
	swordState = SHEATHED;
}

void PlayerAnimController::grabFailedAnimation() // GRAB FAILED ANIMATION //
{
	notLoopAnimation("GrabFail");

	// Move Sword to back
	gameObject->getComponent<MeshRenderer>()->moveEntityToBone("player", "Espalda", "sword");
	swordState = SHEATHED;
}

void PlayerAnimController::grabbedByEnemyAnimation() // GRAB BY ENEMY ANIMATION //
{
	if (anim->getCurrentAnimation() == "GrabbedStart" || anim->getCurrentAnimation() == "GrabbedHold")
		return;

	anim->playAnimation("GrabbedStart");
	anim->setLoop(false);
	state = GRABBING;

	mesh->moveEntityToBone("player", "Espalda", "sword");
	swordState = SHEATHED;
}

void PlayerAnimController::blockAnimation() //  BLOCK ANIMATION //
{
	if (anim->getCurrentAnimation() == "BlockStart" || anim->getCurrentAnimation() == "BlockHold")
		return;

	anim->playAnimationSequence({ "BlockStart", "BlockHold" }, true);
	anim->setLoop(false);
	state = BLOCKING;

	playerFX->activateShield();
}

void PlayerAnimController::blockedAttackAnimation() // BLOCKED ATTACK ANIMATION //
{
	anim->playAnimation("BlockAttack");
	anim->setLoop(false);
	state = BLOCKING;
}

void PlayerAnimController::blockedEnemyGrabAnimation() // BLOCKED ENEMY GRAB ANIMATION //
{
	notLoopAnimation("Knockback");
}

void PlayerAnimController::enemyBlockedMyGrabAnimation() // ENEMY BLOCKED MY ATTACK ANIMATION //
{
	if (anim->getCurrentAnimation() != "GrabStart")
		anim->playAnimation("GrabStart");

	anim->setLoop(false);
	state = STUNNED;
}

void PlayerAnimController::throwEnemyAnimation() // THROW ENEMY ANIMATION //
{
	notLoopAnimation("Throw");
}

void PlayerAnimController::thrownAwayAnimation() // THROWN AWAY ANIMATION //
{
	playAnimationWithDelay("Thrown", thrownDelay);
}

void PlayerAnimController::notLoopAnimation(std::string name)
{
	anim->playAnimation(name);
	anim->setLoop(false);
	state = NOT_LOOPING_STATE;
}

void PlayerAnimController::attackAnimation(int type) //  ATTACK ANIMATION //
{
	if (type == 0)
	{
		if (jump->isGrounded())
			anim->playAnimation("AttackA");
		else
			notLoopAnimation("AttackAAir");
	}
	else
	{
		if (jump->isGrounded())
			anim->playAnimation("AttackB");
		else
			anim->playAnimation("AttackB");
	}

	anim->setLoop(false);
	state = NOT_LOOPING_STATE;
}

void PlayerAnimController::playAnimationWithDelay(std::string name, float delay)
{
	delayedAnimations.push({ name, delay });
}

void PlayerAnimController::enterMode(PlayerMode mode)
{
	if (mode != currentMode)
	{
		switch (mode)
		{
		case PlayerAnimController::ALIVE:
			mesh->changeMesh(aliveMeshId, aliveMeshName);
			notLoopAnimation("Resurrect");
			ghostManag->deactivateGhost();
			break;
		case PlayerAnimController::GHOST:
			mesh->changeMesh(ghostMeshId, ghostMeshName);
			notLoopAnimation("Appear");
			anim->printAllAnimationsNames();
			ghostManag->activateGhost();
			break;
		default:
			break;
		}

		currentMode = mode;
	}
}

void PlayerAnimController::chargingGhostAnimation()
{
	anim->playAnimation("ChargeUGP");
	anim->setLoop(true);
	state = CHARGING_UGP;
}

void PlayerAnimController::punchingGhostAnimation()
{
	anim->playAnimation("UGP");
	anim->setLoop(true);
	state = UGP;
}

void PlayerAnimController::punchSuccessAnimation()
{
	anim->playAnimation("UGPSuccess");
	anim->setLoop(false);
	state = NOT_LOOPING_STATE;
}

bool PlayerAnimController::checkIdle()
{
	return abs(body->getLinearVelocity().x) < runThreshold && (currentMode == ALIVE || abs(body->getLinearVelocity().y) < runThreshold);
}

bool PlayerAnimController::checkStartedFalling()
{
	return abs(body->getLinearVelocity().y) <= fallThreshold;
}

/******************************
	STATE CHANGES
*******************************/

void PlayerAnimController::updateState()
{
	switch (state)
	{
	case PlayerAnimController::IDLE:				// IDLE //
		updateIdle();
		break;
	case PlayerAnimController::RUN:					// RUN //
		updateRun();
		break;
	case PlayerAnimController::JUMP:				// JUMP //
		updateJump();
		break;
	case PlayerAnimController::FALL:				// FALL //
		updateFall();
		break;
	case PlayerAnimController::BLOCKING:			// BLOCKING // 
		updateBlocking();
		break;
	case PlayerAnimController::GRABBING:			// GRABBING //
		updateGrabbing();
		break;
	case PlayerAnimController::GRABBED:				// GRABBED //
		break;
	case PlayerAnimController::STUNNED:				// STUNNED //
		updateStunned();
		break;
	case PlayerAnimController::NOT_LOOPING_STATE:	// NOT_LOOPING_STATE //
		updateNotLoopingState();
		break;
	case PlayerAnimController::UGP:					// UGP //
		break;
	default:
		break;
	}
}

/******************************
	STATE HANDLING
*******************************/

void PlayerAnimController::handleState()
{
	switch (state)
	{
	case PlayerAnimController::IDLE:				// IDLE //
		if (anim->getCurrentAnimation() != "Idle")
		{
			anim->playAnimation("Idle");
			anim->setLoop(true);
		}
		break;
	case PlayerAnimController::RUN:					// RUN //
		if (anim->getCurrentAnimation() != "Run" && anim->getCurrentAnimation() != "Move")
		{
			if (currentMode == ALIVE) anim->playAnimation("Run");
			else anim->playAnimation("Move");

			anim->setLoop(true);
		}
		break;
	case PlayerAnimController::FALL:					// FALL //
		if (anim->getCurrentAnimation() != "Fall" && anim->getCurrentAnimation() != "Hurt")
		{
			anim->playAnimation("Fall");
			anim->setLoop(true);
		}
		break;
	default:
		break;
	}
}

void PlayerAnimController::updateIdle()	//  IDLE //
{
	// TRANSITION TO FALL IF
	if (!jump->isGrounded() && currentMode == ALIVE)
	{
		state = FALL;
		return;
	}

	// TRANSITION TO RUN IF
	if (abs(body->getLinearVelocity().x) >= runThreshold || (currentMode == GHOST && abs(body->getLinearVelocity().y) >= runThreshold))
	{
		state = RUN;
		return;
	}
}

void PlayerAnimController::updateRun() //  RUN //
{
	// TRANSITION TO FALL IF
	if (!jump->isGrounded() && currentMode == ALIVE)
	{
		state = FALL;
		return;
	}

	// TRANSITION TO IDLE IF
	if (checkIdle())
	{
		state = IDLE;
		return;
	}
}

void PlayerAnimController::updateJump() //  JUMP //
{
	if (currentMode == GHOST) return;

	if (anim->getCurrentAnimation() == "JumpStart" && anim->hasEnded() && checkStartedFalling())
	{
		anim->playAnimationSequence({ "JumpChange", "Fall" }, true);
		anim->setLoop(false);
		return;
	}

	if (anim->getCurrentAnimation() == "Fall") // Player is falling: transition to Fall
		state = FALL;
}

void PlayerAnimController::updateFall() //  FALL //
{
	if (!jump->isGrounded() || currentMode == GHOST)
		return;

	// If isGrounded, the player has landed
	anim->playAnimation("Land");
	anim->setLoop(false);
	state = NOT_LOOPING_STATE;
}

void PlayerAnimController::updateGrabbing() //  GRABBING //
{
	if (currentMode == GHOST) return;

	if (!grab->isGrabbing())
	{
		if (anim->getCurrentAnimation() == "JumpStartGrabbing" || anim->getCurrentAnimation() == "JumpChangeGrabbing")
		{
			state = JUMP;
			updateJump();
		}
		else if (anim->getCurrentAnimation() == "FallGrabbing")
		{
			state = FALL;
			updateFall();
		}
	}

	if (anim->getCurrentAnimation() == "JumpStartGrabbing" && anim->hasEnded() && checkStartedFalling())
	{
		anim->playAnimationSequence({ "JumpChangeGrabbing", "FallGrabbing" }, true);
		anim->setLoop(false);
		return;
	}

	if (anim->getCurrentAnimation() == "FallGrabbing" && jump->isGrounded())
	{
		notLoopAnimation("LandGrabbing");
		return;
	}

	if (anim->getCurrentAnimation() == "GrabStart" && anim->hasEnded())
	{
		if (grab->isGrabbing())
		{
			anim->playAnimation("GrabHold");
			anim->setLoop(true);
		}
		else
		{
			anim->playAnimation("GrabFail");
			anim->setLoop(false);
		}
		return;
	}

	if (anim->getCurrentAnimation() == "DashFrontGrabbing" && anim->hasEnded())
	{
		if (grab->isGrabbing())
		{
			anim->playAnimation("GrabHold");
			anim->setLoop(true);
			return;
		}
	}

	if (anim->getCurrentAnimation() == "GrabHold" && !checkIdle())
	{
		anim->playAnimation("RunGrabbing");
		anim->setLoop(true);
	}
	else if (anim->getCurrentAnimation() == "RunGrabbing" && checkIdle())
	{
		anim->playAnimation("GrabHold");
		anim->setLoop(true);
	}

	if (((anim->getCurrentAnimation() == "GrabHold" || anim->getCurrentAnimation() == "DashFrontGrabbing" || anim->getCurrentAnimation() == "RunGrabbing")
		&& !grab->isGrabbing()) || (anim->getCurrentAnimation() == "GrabFail" && anim->hasEnded()))
	{
		state = IDLE;
		updateIdle();
	}
}

void PlayerAnimController::updateGrabbed() //  GRABBED //
{
	if (currentMode == GHOST) return;

	if (anim->getCurrentAnimation() == "GrabbedStart" && anim->hasEnded())
	{
		anim->playAnimation("GrabbedHold");
		anim->setLoop(true);
	}
}

void PlayerAnimController::updateBlocking() //  BLOCKING //
{
	if (currentMode == GHOST) return;

	if (anim->getCurrentAnimation() == "BlockHold" && !block->isBlocking())
	{
		anim->playAnimation("BlockEnd");
		anim->setLoop(false);

		playerFX->deactivateShield();

		return;
	}

	if (anim->getCurrentAnimation() == "BlockEnd" && anim->hasEnded())
	{
		state = IDLE;
		updateIdle();
		return;
	}

	if (anim->getCurrentAnimation() == "BlockAttack" && anim->hasEnded() && block->isBlocking())
	{
		anim->playAnimation("BlockHold");
		anim->setLoop(true);
		return;
	}
}

void PlayerAnimController::updateStunned() // STUNNED //
{
	if (currentMode == GHOST) return;

	if (anim->getCurrentAnimation() == "GrabStart" && anim->hasEnded())
	{
		stunnedAnimation();
		return;
	}

	if (anim->getCurrentAnimation() == "StunnedHold")
	{
		bool active = true;
		PlayerController* controller = gameObject->getComponent<PlayerController>();
		if (controller != nullptr)
			active = controller->isActive();

		if (!active) return;
		anim->playAnimation("StunnedEnd");
		anim->setLoop(false);
		return;
	}

	if (anim->getCurrentAnimation() == "StunnedEnd" && anim->hasEnded())
	{
		state = IDLE;
		updateIdle();
		return;
	}
}

void PlayerAnimController::updateNotLoopingState()
{

	// GHOST -> ALIVE RESURRECT
	if (anim->getCurrentAnimation() == "UGPSuccess" && anim->hasEnded())
	{
		enterMode(ALIVE);
		return;
	}

	if (anim->getCurrentAnimation() == "Die" && anim->hasEnded())
	{
		//ghostManag->handlePlayerDeath();
		return;
	}

	// Only transition when the animation has finished
	if (!anim->getLoop() && !anim->hasEnded())
		return;

	// TRANSITION TO FALL IF
	if (currentMode == ALIVE && !jump->isGrounded())
	{
		state = FALL;
		return;
	}

	// GHOST DISAPPEARING -> DEACTIVATE PLAYER 
	if (currentMode == GHOST && anim->getCurrentAnimation() == "Disappear")
	{
		ghostManag->deactivatePlayer();

		return;
	}

	// TRANSITION TO IDLE IF
	if (checkIdle())
	{
		state = IDLE;
		return;
	}
	else
	{
		// TRANSITION TO RUN
		state = RUN;
		return;
	}
}

void PlayerAnimController::updateDelayedAnimations(float deltaTime)
{
	std::vector<DelayedAnimation> updated;
	while (!delayedAnimations.empty())
	{
		// Treat the animation
		DelayedAnimation dAnim = delayedAnimations.front(); delayedAnimations.pop();
		dAnim.delayTime -= deltaTime;

		if (dAnim.delayTime <= 0)
			notLoopAnimation(dAnim.name); // If the time has come, play the animation
		else
			updated.push_back(dAnim); //else, place it again in delayed animations
	}

	for (auto u : updated)
		delayedAnimations.push(u);
}