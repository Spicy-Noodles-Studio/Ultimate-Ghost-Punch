#pragma once
#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include <UserComponent.h>

class Animator;
class PlayerState;
class MeshRenderer;

class AnimationManager : public UserComponent
{
private:
	Animator* animator;
	PlayerState* playerState;
	MeshRenderer* mesh;

	enum SwordState
	{
		HAND, SHEATHED
	};

	// Indicates the current position of the sword
	SwordState swordState;

protected:
	virtual void start();
	virtual void postUpdate(float deltaTime);

public:
	AnimationManager(GameObject* gameObject);
	virtual ~AnimationManager();

private:
	void manageAnimations();
	void manageSword();

	bool swordInBack() const;

	// MAIN HIERARCHY ANIMATIONS
	bool manageTransitionAnimations();
	bool manageKnightAnimations();
	bool manageGhostAnimations();

	// TRANSITION ANIMATiONS
	bool manageKnightResurrect();
	bool manageKnightDeath();
	bool manageGhostAppear();
	bool manageGhostDisappear();

	// KNIHGT ANIMATIONS
	bool manageGroundedAnimations();
	bool manageAirAnimations();

	// GROUNDED ANIMATIONS
	bool manageJumpAnimation();
	bool manageRunAnimation();
	bool manageIdleAnimation();
	bool manageLandAnimation();
	bool manageGroundedAttackAnimation();
	bool manageBlockAnimations();
	bool manageBlockAttackAnimation();
	bool manageBlockGrabAnimation();
	bool manageGrabAnimations(); 
	bool manageGrabIdleAnimation();
	bool manageGrabRunAnimation();
	bool manageGrabJumpAnimation();
	bool manageGrabLandAnimation();
	bool manageStunnedAnimations();
	bool manageTauntAnimation();

	// NOT GROUNDED ANIMATIONS
	bool manageFallAnimation();
	bool manageGrabFallAnimation();
	bool manageAirAttackAnimation();

	// BOTH GROUNDED AND NOT ANIMATIONS
	bool manageDashAnimation();
	bool manageGrabDashAnimation();
	bool manageHurtAnimation();
	bool manageGrabbedAnimations();

	// GHOST ANIMATION
	bool manageChargeAnimation();
	bool managePunchingAnimation();
	bool managePunchSuccessAnimation();
	bool managePunchFailAnimation();
	bool manageGhostMoveAnimation();
	bool manageGhostIdleAnimation();
};

#endif