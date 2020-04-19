#include "GhostAnimController.h"

void GhostAnimController::updateState()
{
}

void GhostAnimController::handleState()
{
}

GhostAnimController::GhostAnimController(GameObject* gameObject) : UserComponent(gameObject)
{
}

void GhostAnimController::start()
{
}

void GhostAnimController::update(float deltaTime)
{
	// Update the current state
	updateState();

	// Handle the current state
	handleState();

	//anim->updateAnimationSequence();
}

void GhostAnimController::handleData(ComponentData* data)
{
}
