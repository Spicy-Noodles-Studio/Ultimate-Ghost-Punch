#include "CameraController.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <MathUtils.h>
#include <Timer.h>
#include <sstream>

#include "GameManager.h"
#include "UltimateGhostPunch.h"
#include "PlayerState.h"

REGISTER_FACTORY(CameraController);

CameraController::CameraController(GameObject* gameObject) : UserComponent(gameObject), minZ(20), maxZ(100), minX(20), maxX(100), minY(20), maxY(100),
smoothFactor(0.125f), zoomFactor(1.0f), time(0.0f), slowMoTime(0.3f), slowMoDistance(5.0f), slowMoTimeScale(0.3f), slowMoZ(15.0f), state(MIDPOINT), playerPunching(nullptr)
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
	checkNullAndBreak(data);
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
		if (prop.first == "minX")
		{
			setFloat(minX);
		}
		else if (prop.first == "maxX")
		{
			setFloat(maxX);
		}
		if (prop.first == "minY")
		{
			setFloat(minY);
		}
		else if (prop.first == "maxY")
		{
			setFloat(maxY);
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

void CameraController::setMinZ(float minZ)
{
	this->minZ = minZ;
}

void CameraController::setMaxZ(float maxZ)
{
	this->maxZ = maxZ;
}

void CameraController::setMinX(float minX)
{
	this->minX = minX;
}

void CameraController::setMaxX(float maxX)
{
	this->maxX = maxX;
}

void CameraController::setMinY(float minY)
{
	this->minY = minY;
}

void CameraController::setMaxY(float maxY)
{
	this->maxY = maxY;
}

void CameraController::smoothMove()
{
	checkNullAndBreak(gameObject->transform);

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
	checkNullAndBreak(GameManager::GetInstance(), 0);
	std::vector<GameObject*> alive = GameManager::GetInstance()->getAlivePlayers();

	// Number of players alive
	int n = alive.size();
	float maxDist = -1;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i != j && notNull(alive[i]) && notNull(alive[i]->transform) && notNull(alive[j]) && notNull(alive[j]->transform)) {
				float d = (alive[i]->transform->getPosition() - alive[j]->transform->getPosition()).magnitude();
				if (d > maxDist) maxDist = d;
			}
		}
	}

	return maxDist;
}

void CameraController::setTargetToSlowMo()
{
	checkNullAndBreak(playerPunching);
	checkNullAndBreak(playerPunching->transform);

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
	checkNullAndBreak(Timer::GetInstance());

	Timer::GetInstance()->setTimeScale(slowMoTimeScale);
	time = slowMoTime;
	state = SLOWMO;
}

void CameraController::deactivateSlowMo()
{
	checkNullAndBreak(Timer::GetInstance());

	Timer::GetInstance()->setTimeScale(1.0f);
	state = MIDPOINT;
}

Vector3 CameraController::getMidPointBetweenPlayers()
{
	checkNullAndBreak(GameManager::GetInstance(), Vector3::ZERO);

	// Vector with every player alive
	std::vector<GameObject*> alive = GameManager::GetInstance()->getAlivePlayers();

	// Number of players alive
	int n = alive.size();
	float midX = 0.0f, midY = 0.0f;

	for (auto a : alive) {
		if (notNull(a) && notNull(a->transform)) {
			midX += a->transform->getPosition().x / n;
			midY += a->transform->getPosition().y / n;
		}
	}

	return Vector3(midX, midY, 0.0f);
}

void CameraController::getMidPointAdjusted(Vector3* midPoint, float zoom)
{
	float zoomMargin = (((zoom - minZ) * (15 - (0))) / (maxZ - minZ)) + 0;

	if (midPoint->x < (minX + zoomMargin)) midPoint->x = minX + zoomMargin;
	if (midPoint->x > (maxX - zoomMargin)) midPoint->x = maxX - zoomMargin;
	if (midPoint->y < (minY + zoomMargin)) midPoint->y = minY + zoomMargin;
	if (midPoint->y > (maxY - zoomMargin)) midPoint->y = maxY - zoomMargin;
}

void CameraController::setTargetToMidPointPlayers()
{
	// Move towards mid-point position
	Vector3 midPos = getMidPointBetweenPlayers();

	// Zoom in/out
	float dist = getMaxDistBetweenPlayers();

	//Clamp between minZ and maxZ
	dist *= zoomFactor;
	dist = std::min(maxZ, std::max(dist, minZ));

	// Adjust mid-point to boundaries and zoom
	getMidPointAdjusted(&midPos, dist);

	target = midPos + Vector3(0, 0, dist);
}

GameObject* CameraController::someonePunching()
{
	// Vector with every player
	checkNullAndBreak(GameManager::GetInstance(), nullptr);

	std::vector<GameObject*> players = GameManager::GetInstance()->getKnights();

	// Number of players
	int n = players.size();

	int i = 0;
	while (i < n && notNull(players[i]) && notNull(players[i]->getComponent<UltimateGhostPunch>()) && !players[i]->getComponent<UltimateGhostPunch>()->isPunching())
		i++;

	if (i < n) return players[i];

	return nullptr;
}