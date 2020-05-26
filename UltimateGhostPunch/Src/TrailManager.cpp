#include "TrailManager.h"

#include <ComponentRegister.h>
#include <GameObject.h>
#include <Trail.h>
#include <MeshRenderer.h>
#include <GaiaData.h>
#include <sstream>

#include "PlayerState.h"

REGISTER_FACTORY(TrailManager);

TrailManager::TrailManager(GameObject* gameObject) : UserComponent(gameObject), quickAttackTrail(nullptr), heavyAttackTrail(nullptr), dashTrail(nullptr), thrownTrail(nullptr), UGPTrail(nullptr), playerState(nullptr),
time(0), thrownTime(1)
{

}

TrailManager::~TrailManager()
{
	quickAttackTrail = nullptr;
	heavyAttackTrail = nullptr;
	dashTrail = nullptr;
	thrownTrail = nullptr;
	UGPTrail = nullptr;
	playerState = nullptr;
}

void TrailManager::stopAll()
{
	if (notNull(quickAttackTrail)) quickAttackTrail->stop();
	if (notNull(heavyAttackTrail)) heavyAttackTrail->stop();
	if (notNull(dashTrail)) dashTrail->stop();
	if (notNull(thrownTrail)) thrownTrail->stop();
	if (notNull(UGPTrail)) UGPTrail->stop();
}

void TrailManager::reconfigureAttackTrails()
{
	if (notNull(quickAttackTrail))
		quickAttackTrail->configureTrail("./Assets/Trails/quickAttackTrail.trail");

	if (notNull(heavyAttackTrail))
		heavyAttackTrail->configureTrail("./Assets/Trails/heavyAttackTrail.trail");
}

void TrailManager::start()
{
	checkNullAndBreak(gameObject);

	// PlayerState for info
	playerState = gameObject->getComponent<PlayerState>();
	checkNull(playerState);

	MeshRenderer* mesh = gameObject->getComponent<MeshRenderer>();
	Vector3 diffuse = (notNull(mesh)) ? mesh->getDiffuse(0) : Vector3::ZERO;

	createTrail(&heavyAttackTrail, "heavyAttackTrail");
	createTrail(&quickAttackTrail, "quickAttackTrail");
	createTrail(&dashTrail, "dashTrail");
	if (notNull(dashTrail)) dashTrail->setColour(diffuse, 1);

	createTrail(&thrownTrail, "thrownTrail");
	if (notNull(thrownTrail))thrownTrail->setColour(diffuse, 1);

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
	checkNullAndBreak(trailObject);
	checkNullAndBreak(gameObject);

	gameObject->addChild(trailObject);

	*trail = trailObject->getComponent<Trail>();
	checkNullAndBreak(*trail);

	// Set parameters from file
	MeshRenderer* mesh = gameObject->getComponent<MeshRenderer>();
	if (notNull(mesh))
		(*trail)->setMeshRenderer(mesh);
	(*trail)->configureTrail("./Assets/Trails/" + trailFilename + ".trail");
}

void TrailManager::manageQuickAttackTrail()
{
	checkNullAndBreak(playerState);
	checkNullAndBreak(quickAttackTrail);

	if (playerState->isQuickAttacking())
		quickAttackTrail->start();
	else
		quickAttackTrail->stop();
}

void TrailManager::manageHeavyAttackTrail()
{
	checkNullAndBreak(playerState);
	checkNullAndBreak(heavyAttackTrail);

	if (playerState->isHeavyAttacking()) {
		if (!heavyAttackTrail->started()) heavyAttackTrail->start();
	}
	else
		if (heavyAttackTrail->started()) heavyAttackTrail->stop();
}

void TrailManager::manageDashTrail()
{
	checkNullAndBreak(playerState);
	checkNullAndBreak(dashTrail);

	if (playerState->isDodging()) {
		if (!dashTrail->started()) dashTrail->start();
	}
	else
		if (dashTrail->started()) dashTrail->stop();
}

void TrailManager::manageThrownTrail(float deltaTime)
{
	checkNullAndBreak(playerState);
	checkNullAndBreak(thrownTrail);

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
	checkNullAndBreak(playerState);
	checkNullAndBreak(UGPTrail);

	if (playerState->isPunching()) {
		if (!UGPTrail->started()) UGPTrail->start();
	}
	else
		if (UGPTrail->started()) UGPTrail->stop();
}