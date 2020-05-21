#include "TrailManager.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <Trail.h>
#include <MeshRenderer.h>
#include <GaiaData.h>
#include <sstream>

#include "PlayerState.h"

REGISTER_FACTORY(TrailManager);

TrailManager::TrailManager(GameObject* gameObject) : UserComponent(gameObject), quickAttackTrail(nullptr), heavyAttackTrail(nullptr),
time(0), thrownTime(1)
{

}

TrailManager::~TrailManager()
{

}

void TrailManager::start()
{
	// PlayerState for info
	playerState = gameObject->getComponent<PlayerState>();
	checkNull(playerState);

	MeshRenderer* mesh = gameObject->getComponent<MeshRenderer>();
	Vector3 diffuse = (mesh != nullptr) ? mesh->getDiffuse(0) : Vector3::ZERO;

	createTrail(&heavyAttackTrail, "heavyAttackTrail");
	createTrail(&quickAttackTrail, "quickAttackTrail");
	createTrail(&dashTrail, "dashTrail");
	if (dashTrail != nullptr) dashTrail->setColour(diffuse, 1);

	createTrail(&thrownTrail, "thrownTrail");
	if (thrownTrail != nullptr)thrownTrail->setColour(diffuse, 1);

	createTrail(&UGPTrail, "UGPTrail");
}

void TrailManager::preUpdate(float deltaTime)
{
	manageQuickAttackTrail();
	manageHeavyAttackTrail();
	manageDashTrail();
	manageThrownTrail(deltaTime);
	manageUGPTrail();
}

void TrailManager::createTrail(Trail** trail, const std::string& trailFilename)
{
	if (*trail != nullptr) return;

	GameObject* trailObject = instantiate("Trail");
	if (trailObject == nullptr)
		return;

	gameObject->addChild(trailObject);

	*trail = trailObject->getComponent<Trail>();
	if (*trail == nullptr)
		return;

	// Set parameters from file
	MeshRenderer* mesh = gameObject->getComponent<MeshRenderer>();
	if (mesh != nullptr)
		(*trail)->setMeshRenderer(mesh);
	(*trail)->configureTrail("./Assets/Trails/" + trailFilename + ".trail");
}

void TrailManager::manageQuickAttackTrail()
{
	if (quickAttackTrail == nullptr || playerState == nullptr) return;

	if (playerState->isQuickAttacking())
		quickAttackTrail->start();
	else
		quickAttackTrail->stop();
}

void TrailManager::manageHeavyAttackTrail()
{
	if (heavyAttackTrail == nullptr || playerState == nullptr) return;

	if (playerState->isHeavyAttacking()) {
		if (!heavyAttackTrail->started()) heavyAttackTrail->start();
	}
	else
		if (heavyAttackTrail->started()) heavyAttackTrail->stop();
}

void TrailManager::manageDashTrail()
{
	if (dashTrail == nullptr || playerState == nullptr) return;

	if (playerState->isDodging()) {
		if (!dashTrail->started()) dashTrail->start();
	}
	else
		if (dashTrail->started()) dashTrail->stop();
}

void TrailManager::manageThrownTrail(float deltaTime)
{
	if (thrownTrail == nullptr || playerState == nullptr) return;

	if (playerState->hasBeenThrown()) {
		time = thrownTime;
		if (!thrownTrail->started()) thrownTrail->start();
	}

	if (time > 0)
		time -= deltaTime;

	else if (time <= 0)
		if (thrownTrail->started()) thrownTrail->stop();
}

void TrailManager::manageUGPTrail()
{
	if (UGPTrail == nullptr || playerState == nullptr) return;

	if (playerState->isPunching()) {
		if (!UGPTrail->started()) UGPTrail->start();
	}
	else
		if (UGPTrail->started()) UGPTrail->stop();
}