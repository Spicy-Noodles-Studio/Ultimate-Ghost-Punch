#include "CameraController.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <MathUtils.h>
#include <Timer.h>
#include <sstream>

#include "GameManager.h"
#include "UltimateGhostPunch.h"
#include "Health.h"
#include "GhostManager.h"

REGISTER_FACTORY(CameraController);

CameraController::CameraController(GameObject* gameObject) : UserComponent(gameObject), minZ(20), maxZ(100), smoothFactor(0.125f), zoomFactor(1.0f),
time(0.0f), slowMoTime(0.3f), slowMoDistance(5.0f), slowMoTimeScale(0.3f), slowMoZ(15.0f), state(MIDPOINT), playerPunching(nullptr)
{

}

CameraController::~CameraController()
{

}

void CameraController::preUpdate(float deltaTime)
{
	// Update SLOWMO timer
	if (time > 0.0f)
		time -= deltaTime;
	else if (state == SLOWMO)
		deactivateSlowMo();
}

void CameraController::update(float deltaTime)
{
	handleState();

	if (state != SLOWMO)
		checkSlowMo();

	smoothMove();
}

void CameraController::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "minZ")
		{
			setFloat(minZ);
		}
		else if (prop.first == "maxZ")
		{
			setFloat(maxZ);
		}
		else if (prop.first == "smoothFactor")
		{
			setFloat(smoothFactor);
		}
		else if (prop.first == "zoomFactor")
		{
			setFloat(zoomFactor);
		}
		else if (prop.first == "slowMoTime")
		{
			setFloat(slowMoTime);
		}
		else if (prop.first == "slowMoDistance")
		{
			setFloat(slowMoDistance);
		}
		else if (prop.first == "slowMoTimeScale")
		{
			setFloat(slowMoTimeScale);
		}
		else if (prop.first == "slowMoZ")
		{
			setFloat(slowMoZ);
		}
		else
			LOG("CAMERA CONTROLLER: Invalid property with name \"%s\"", prop.first.c_str());
	}
}

void CameraController::smoothMove()
{
	Vector3 lerpDest = gameObject->transform->getPosition();
	lerpDest.lerp(target, smoothFactor);
	gameObject->transform->setPosition(lerpDest);
}

void CameraController::handleState()
{
	if (state == MIDPOINT)
		setTargetToMidPointPlayers();
	else if (state == SLOWMO)
		setTargetToSlowMo();
}

float CameraController::getMaxDistBetweenPlayers()
{
	// Vector with every player alive
	std::vector<GameObject*> alive = getAlivePlayers();

	// number of players alive
	int n = alive.size();
	float maxDist = -1;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i != j) {
				float d = (alive[i]->transform->getPosition() - alive[j]->transform->getPosition()).magnitude();
				if (d > maxDist) maxDist = d;
			}
		}
	}

	return maxDist;
}

void CameraController::setTargetToSlowMo()
{
	if (playerPunching == nullptr)
		return;

	Vector3 playerPunchingPos = playerPunching->transform->getPosition();
	target = { playerPunchingPos.x, playerPunchingPos.y, slowMoZ };
}

void CameraController::checkSlowMo()
{
	playerPunching = someonePunching();

	if (playerPunching != nullptr && getMaxDistBetweenPlayers() < slowMoDistance)
		activateSlowMo();
}

void CameraController::activateSlowMo()
{
	Timer::GetInstance()->setTimeScale(slowMoTimeScale);
	time = slowMoTime;
	state = SLOWMO;
}

void CameraController::deactivateSlowMo()
{
	Timer::GetInstance()->setTimeScale(1.0f);
	state = MIDPOINT;
}

Vector3 CameraController::getMidPointBetweenPlayers()
{
	// Vector with every player alive
	std::vector<GameObject*> alive = getAlivePlayers();

	// number of players alive
	int n = alive.size();
	float midX = 0.0f, midY = 0.0f;

	for (auto a : alive) {
		midX += a->transform->getPosition().x / n;
		midY += a->transform->getPosition().y / n;
	}

	return Vector3(midX, midY, 0.0f);
}

void CameraController::setTargetToMidPointPlayers()
{
	// Move towards mid-point position
	Vector3 midPos = getMidPointBetweenPlayers();

	// Zoom in/out
	float dist = getMaxDistBetweenPlayers();

	//clamp between minZ and maxZ
	dist *= zoomFactor;
	dist = std::min(maxZ, std::max(dist, minZ));

	target = midPos + Vector3(0, 0, dist);
}

GameObject* CameraController::someonePunching()
{
	// Vector with every player
	std::vector<GameObject*> players = GameManager::GetInstance()->getKnights();

	// number of players
	int n = players.size();

	int i = 0;
	while (i < n && !players[i]->getComponent<UltimateGhostPunch>()->isPunching())
		i++;

	if (i < n) return players[i];
	return nullptr;
}

std::vector<GameObject*> CameraController::getAlivePlayers()
{
	std::vector<GameObject*> players = GameManager::GetInstance()->getKnights();
	std::vector<GameObject*> alive;
	for (auto p : players) {
		if (p->getComponent<Health>()->isAlive() || p->getComponent<GhostManager>()->isGhost()) alive.push_back(p);
	}
	return alive;
}