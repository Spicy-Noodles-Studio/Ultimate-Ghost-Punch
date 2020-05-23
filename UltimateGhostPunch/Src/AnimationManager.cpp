#include "AnimationManager.h"

#include <ComponentRegister.h>
#include <GameObject.h>
#include <Animator.h>
#include <MeshRenderer.h>

#include "PlayerState.h"

REGISTER_FACTORY(AnimationManager);

AnimationManager::AnimationManager(GameObject* gameObject) : UserComponent(gameObject), swordState(HAND), animator(nullptr), playerState(nullptr), mesh(nullptr)
{

}

AnimationManager::~AnimationManager()
{
	animator = nullptr;
	playerState = nullptr;
	mesh = nullptr;
}

void AnimationManager::start()
{
	animator = gameObject->getComponent<Animator>();
	playerState = gameObject->getComponent<PlayerState>();
	mesh = gameObject->getComponent<MeshRenderer>();

	checkNull(animator);
	checkNull(playerState);
	checkNull(mesh);
}

void AnimationManager::postUpdate(float deltaTime)
{
	checkNullAndBreak(playerState);
	checkNullAndBreak(animator);

	manageAnimations();

	// Update sword position if necessary
	manageSword();
}

void AnimationManager::manageAnimations()
{
	if (manageTransitionAnimations());
	else if (manageKnightAnimations());
	else if (manageGhostAnimations());
}

void AnimationManager::manageSword()
{
	checkNullAndBreak(mesh);

	if (swordState != HAND && !swordInBack())
	{
		// Move sword back to hand
		mesh->moveEntityToBone("player", "Mano.L", "sword");
		swordState = HAND;
	}
	else if (swordState != SHEATHED && swordInBack()) {
		// Move sword back to hand
		mesh->moveEntityToBone("player", "Espalda", "sword");
		swordState = SHEATHED;
	}
}

bool AnimationManager::swordInBack() const
{
	std::string currentAnimation = notNull(animator) ? animator->getCurrentAnimation() : "";
	return currentAnimation == "GrabFail" || currentAnimation == "GrabHold" || currentAnimation == "GrabStart" || currentAnimation == "Throw" ||
		currentAnimation == "RunGrabbing" || currentAnimation == "DashFrontGrabbing" || currentAnimation == "FallGrabbing" ||
		currentAnimation == "JumpStartGrabbing" || currentAnimation == "JumpChangeGrabbing" || currentAnimation == "LandGrabbing";
}

bool AnimationManager::manageTransitionAnimations()
{
	bool result = false;
	if (result = manageKnightResurrect());
	else if (result = manageKnightDeath());
	else if (result = manageGhostAppear());
	else if (result = manageGhostDisappear());
	return result;
}

bool AnimationManager::manageKnightAnimations()
{
	checkNullAndBreak(playerState, false);

	bool result = false;
	if (playerState->isResurrecting() || playerState->isDying() || !playerState->isGhost())
		if (result = manageGroundedAnimations());
		else if (result = manageAirAnimations());
	return result;
}

bool AnimationManager::manageGhostAnimations()
{
	checkNullAndBreak(playerState, false);

	bool result = false;
	if (playerState->isGhost() || playerState->isAppearing() || playerState->isDisappearing())
	{
		//ODER MATTERS: Top ones have higher priority
		if (result = manageChargeAnimation());
		else if (result = managePunchingAnimation());
		else if (result = managePunchSuccessAnimation());
		else if (result = managePunchFailAnimation());
		else if (result = manageGhostIdleAnimation());
		else if (result = manageGhostMoveAnimation());
	}
	return result;
}

bool AnimationManager::manageKnightResurrect()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (playerState->isResurrecting() || playerState->isRespawning()) {
		animator->playAnimation("Resurrect");
		animator->setLoop(false);
		return true;
	}

	return false;
}

bool AnimationManager::manageKnightDeath()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (playerState->isDying()) {
		animator->playAnimation("Die");
		animator->setLoop(false);
		return true;
	}

	return false;
}

bool AnimationManager::manageGhostAppear()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (playerState->isAppearing()) {
		animator->playAnimation("Appear");
		animator->setLoop(false);
		return true;
	}

	return false;
}

bool AnimationManager::manageGhostDisappear()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (playerState->isDisappearing()) {
		animator->playAnimation("Disappear");
		animator->setLoop(false);
		return true;
	}

	return false;
}

bool AnimationManager::manageGroundedAnimations()
{
	checkNullAndBreak(playerState, false);

	bool result = false;
	if (playerState->isGrounded()) {
		//ODER MATTERS: Top ones have higher priority
		if (result = manageGrabbedAnimations());
		else if (result = manageStunnedAnimations());
		else if (result = manageHurtAnimation());
		else if (result = manageBlockAnimations());
		else if (result = manageGroundedAttackAnimation());
		else if (result = manageGrabAnimations());
		else if (result = manageJumpAnimation());
		else if (result = manageLandAnimation());
		else if (result = manageIdleAnimation());
		else if (result = manageDashAnimation());
		else if (result = manageRunAnimation());
	}
	return result;
}

bool AnimationManager::manageAirAnimations()
{
	checkNullAndBreak(playerState, false);

	bool result = false;
	if (!playerState->isGrounded()) {
		//ODER MATTERS: Top ones have higher priority
		if (result = manageGrabbedAnimations());
		else if (result = manageHurtAnimation());
		else if (result = manageAirAttackAnimation());
		else if (result = manageDashAnimation());
		else if (result = manageJumpAnimation());
		else if (result = manageLandAnimation());
		else if (result = manageFallAnimation());
	}
	return result;
}

bool AnimationManager::manageJumpAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if ((playerState->isGrounded() && playerState->isJumping()) || playerState->hasJumped()) {
		if (manageGrabJumpAnimation()) return true;

		animator->playAnimation("JumpStart");
		animator->setLoop(false);
		return true;
	}
	return false;
}

bool AnimationManager::manageRunAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (playerState->isMoving() && playerState->canMove()) {
		if (manageGrabRunAnimation()) return true;

		animator->playAnimation("Run");
		animator->setLoop(true);
		return true;
	}
	return false;
}

bool AnimationManager::manageIdleAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (!playerState->isMoving() && playerState->canMove()) {
		if (manageGrabIdleAnimation()) return true;
		if (manageTauntAnimation()) return true;

		animator->playAnimation("Idle");
		animator->setLoop(true);
		return true;
	}
	return false;
}

bool AnimationManager::manageLandAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (playerState->hasLanded()) {
		if (manageGrabLandAnimation()) return true;

		animator->playAnimation("Land");
		animator->setLoop(false);
		return true;
	}

	// Avoids the interruption of lower priority animations
	std::string animationName = animator->getCurrentAnimation();
	return (animationName == "Land" || animationName == "LandGrabbing") && !animator->hasEnded();
}

bool AnimationManager::manageGroundedAttackAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (playerState->isQuickAttacking()) {
		animator->playAnimation("AttackA");
		animator->setLoop(false);
		return true;
	}
	else if (playerState->isHeavyAttacking()) {
		animator->playAnimation("AttackB");
		animator->setLoop(false);
		return true;
	}

	std::string animationName = animator->getCurrentAnimation();
	return (animationName == "AttackA" || animationName == "AttackB") && !animator->hasEnded();
}

bool AnimationManager::manageBlockAnimations()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	std::string animationName = animator->getCurrentAnimation();
	//Transition begin block -> blocking
	if (playerState->isBlocking() || playerState->hasBlockedGrab()) {
		bool previouslyBlocking = animationName == "BlockStart" ||
			animationName == "BlockHold" ||
			animationName == "BlockAttack" ||
			animationName == "BlockGrab";
		// HIGHER PRIORITY THAN THE TRANSITION ANIMATION
		if (manageBlockAttackAnimation()) return true;
		if (manageBlockGrabAnimation()) return true;

		// NORMAL TRANSITIONS
		else if (!previouslyBlocking) {
			animator->playAnimation("BlockStart");
			animator->setLoop(false);
		}
		else if (previouslyBlocking) {
			//If it was at the beginning, we wait until it ends
			if (animationName == "BlockStart" && !animator->hasEnded()) return true;

			animator->playAnimation("BlockHold");
			animator->setLoop(true);
		}

		return true;
	}
	// If it wasnt blocking
	else if (animationName == "BlockStart" || animationName == "BlockHold") {
		animator->playAnimation("BlockEnd");
		animator->setLoop(false);
		return true;
	}

	return (animationName == "BlockEnd" || animationName == "Knockback") && !animator->hasEnded();
}

bool AnimationManager::manageBlockAttackAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (playerState->hasBlocked()) {
		animator->playAnimation("BlockAttack");
		animator->setLoop(false);
		return true;
	}
	return animator->getCurrentAnimation() == "BlockAttack" && !animator->hasEnded();
}

bool AnimationManager::manageBlockGrabAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	// Blocks a grab
	if (playerState->hasBlockedGrab()) {
		LOG("BLOCKED GRAB");
		animator->playAnimation("Knockback");
		animator->setLoop(false);
		return true;
	}
	return animator->getCurrentAnimation() == "Knockback" && !animator->hasEnded();
}

bool AnimationManager::manageGrabAnimations()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	std::string animationName = animator->getCurrentAnimation();

	bool previouslyGrabbing = animationName == "GrabHold" ||
		animationName == "RunGrabbing" ||
		animationName == "DashFrontGrabbing" ||
		animationName == "FallGrabbing" ||
		animationName == "JumpStartGrabbing" ||
		animationName == "JumpChangeGrabbing" ||
		animationName == "LandGrabbing";

	if (playerState->isGrabbing() || playerState->hasMissedGrab()) {
		// NORMAL TRANSITIONS
		if (!previouslyGrabbing) {
			if (playerState->hasMissedGrab()) {
				animator->playAnimation("GrabFail");
				animator->setLoop(false);
				return true;
			}
			else {
				if (animator->getCurrentAnimation() == "GrabStart" && animator->hasEnded()) return false;

				animator->playAnimation("GrabStart");
				animator->setLoop(false);
				return true;
			}
		}
		return false;
	}
	// If it wasnt grabbed
	else if (previouslyGrabbing) {
		animator->playAnimation("Throw");
		animator->setLoop(false);
		return true;
	}

	return (animationName == "Throw" || animationName == "GrabFail") && !animator->hasEnded();
}

bool AnimationManager::manageGrabIdleAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (playerState->isGrabbing()) {
		animator->playAnimation("GrabHold");
		animator->setLoop(true);
		return true;
	}
	return false;
}

bool AnimationManager::manageGrabRunAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (playerState->isGrabbing()) {
		animator->playAnimation("RunGrabbing");
		animator->setLoop(true);
		return true;
	}
	return false;
}

bool AnimationManager::manageGrabJumpAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (playerState->isGrabbing()) {
		animator->playAnimation("JumpStartGrabbing");
		animator->setLoop(false);
		return true;
	}
	return false;
}

bool AnimationManager::manageGrabLandAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (playerState->isGrabbing()) {
		animator->playAnimation("LandGrabbing");
		animator->setLoop(false);
		return true;
	}

	return false;
}

bool AnimationManager::manageStunnedAnimations()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	std::string animationName = animator->getCurrentAnimation();

	if (playerState->isStunned()) {
		bool previouslyStunned = animationName == "StunnedStart" ||
			animationName == "StunnedHold" ||
			animationName == "StunnedAttack";
		// NORMAL TRANSITIONS
		if (!previouslyStunned) {
			animator->playAnimation("StunnedStart");
			animator->setLoop(false);
		}
		else if (previouslyStunned) {
			//If it was at the beginning, we wait until it ends
			if (animationName == "StunnedStart" && !animator->hasEnded()) return true;

			animator->playAnimation("StunnedHold");
			animator->setLoop(true);
		}

		return true;
	}
	// If it wasnt stunned
	else if (animationName == "StunnedStart" || animationName == "StunnedHold") {
		animator->playAnimation("StunnedEnd");
		animator->setLoop(false);
		return true;
	}

	return animationName == "StunnedEnd" && !animator->hasEnded();
}

bool AnimationManager::manageTauntAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (playerState->hasTaunted()) {
		animator->playAnimation("Taunt");
		animator->setLoop(false);
		return true;
	}

	return animator->getCurrentAnimation() == "Taunt" && !animator->hasEnded();
}

bool AnimationManager::manageFallAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	bool result = false;
	if (playerState->isFalling()) {
		if (manageGrabFallAnimation()) return true;

		// Falling after a jump -> transition
		if (animator->getCurrentAnimation() == "JumpStart" && animator->hasEnded()) {
			animator->playAnimation("JumpChange");
			animator->setLoop(false);
		}
		else if (animator->getCurrentAnimation() == "JumpChange") {
			if (animator->hasEnded()) {	//To avoid skipping the transition
				animator->playAnimation("Fall");
				animator->setLoop(true);
			}
		}
		// Falling without jumping
		else {
			animator->playAnimation("Fall");
			animator->setLoop(true);
		}
		result = true;
	}
	return result;
}

bool AnimationManager::manageGrabFallAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	bool result = false;
	if (playerState->isGrabbing()) {
		// Falling after a jump -> transition
		if (animator->getCurrentAnimation() == "JumpStartGrabbing" && animator->hasEnded()) {
			animator->playAnimation("JumpChangeGrabbing");
			animator->setLoop(false);
		}
		else if (animator->getCurrentAnimation() == "JumpChangeGrabbing") {
			if (animator->hasEnded()) {
				animator->playAnimation("FallGrabbing");
				animator->setLoop(true);
			}
		}
		else {
			animator->playAnimation("FallGrabbing");
			animator->setLoop(true);
		}
		result = true;
	}
	return result;
}

bool AnimationManager::manageAirAttackAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (playerState->isQuickAttacking()) {
		animator->playAnimation("AttackAAir");
		animator->setLoop(false);
		return true;
	}
	else if (playerState->isHeavyAttacking()) {
		animator->playAnimation("AttackB");
		animator->setLoop(false);
		return true;
	}

	std::string animationName = animator->getCurrentAnimation();
	return (animationName == "AttackAAir" || animationName == "AttackB") && !animator->hasEnded();
}

bool AnimationManager::manageDashAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (playerState->isDodging()) {
		if (manageGrabDashAnimation()) return true;

		animator->playAnimation("DashFront");
		animator->setLoop(false);
		return true;
	}
	// Avoids interruption of lower priority animations
	std::string animationName = animator->getCurrentAnimation();
	return (animationName == "DashFront" || animationName == "DashFrontGrabbing") && !animator->hasEnded();
}

bool AnimationManager::manageGrabDashAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (playerState->isGrabbing()) {
		animator->playAnimation("DashFrontGrabbing");
		animator->setLoop(false);
		return true;
	}
	return false;
}

bool AnimationManager::manageHurtAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (playerState->isHurt()) {
		animator->playAnimation("Hurt");
		animator->setLoop(false);
		return true;
	}
	return animator->getCurrentAnimation() == "Hurt" && !animator->hasEnded();
}

bool AnimationManager::manageGrabbedAnimations()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	std::string animationName = animator->getCurrentAnimation();
	if (playerState->isGrabbed()) {
		bool previouslyGrabbed = animationName == "GrabbedStart" ||
			animationName == "GrabbedHold";

		if (!previouslyGrabbed) {
			animator->playAnimation("GrabbedStart");
			animator->setLoop(false);
		}
		else if (previouslyGrabbed) {
			if (animationName == "GrabbedStart" && !animator->hasEnded()) return true;

			animator->playAnimation("GrabbedHold");
			animator->setLoop(true);
		}
		return true;
	}

	//If it wasnt grabbed
	else if (animationName == "GrabbedStart" || animationName == "GrabbedHold") {
		animator->playAnimation("Thrown");
		animator->setLoop(false);
		return true;
	}

	return animationName == "Thrown" && !animator->hasEnded();
}

bool AnimationManager::manageChargeAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (playerState->isAiming()) {
		animator->playAnimation("ChargeUGP");
		animator->setLoop(true);
		return true;
	}
	return false;
}

bool AnimationManager::managePunchingAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (playerState->isPunching()) {
		animator->playAnimation("UGP");
		animator->setLoop(true);
		return true;
	}
	return false;
}

bool AnimationManager::managePunchSuccessAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (playerState->hasPunchSucceeded()) {
		animator->playAnimation("UGPSuccess");
		animator->setLoop(false);
		return true;
	}
	return animator->getCurrentAnimation() == "UGPSuccess" && !animator->hasEnded();
}

bool AnimationManager::managePunchFailAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (playerState->hasPunchFailed()) {
		animator->playAnimation("UGPFail");
		animator->setLoop(false);
		return true;
	}
	return animator->getCurrentAnimation() == "UGPFail" && !animator->hasEnded();
}

bool AnimationManager::manageGhostMoveAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (playerState->isGhostMoving() && playerState->canGhostMove()) {
		animator->playAnimation("Move");
		animator->setLoop(true);
		return true;
	}
	return false;
}

bool AnimationManager::manageGhostIdleAnimation()
{
	checkNullAndBreak(playerState, false);
	checkNullAndBreak(animator, false);

	if (!playerState->isGhostMoving() && playerState->canGhostMove()) {
		animator->playAnimation("Idle");
		animator->setLoop(true);
		return true;
	}
	return false;
}