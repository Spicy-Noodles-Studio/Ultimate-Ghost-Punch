#include "DynamicCamera.h"
#include <sstream>
#include <ComponentRegister.h>
#include <GameObject.h>
#include <MathUtils.h>

#include "GameManager.h"

REGISTER_FACTORY(DynamicCamera);

DynamicCamera::DynamicCamera(GameObject* gameObject) : UserComponent(gameObject), smoothFactor(0.125f), minZ(20), maxZ(100), zoomFactor(1.0f)
{
}

DynamicCamera::~DynamicCamera()
{
}

void DynamicCamera::update(float deltaTime)
{
	dynamicMove();
}

void DynamicCamera::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "minZ") {
			if (!(ss >> minZ))
				LOG("DYNAMIC CAMERA: Invalid value for property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "maxZ") {
			if (!(ss >> maxZ))
				LOG("DYNAMIC CAMERA: Invalid value for property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "smoothFactor") {
			if (!(ss >> smoothFactor))
				LOG("DYNAMIC CAMERA: Invalid value for property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "zoomFactor") {
			if (!(ss >> zoomFactor))
				LOG("DYNAMIC CAMERA: Invalid value for property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("DYNAMIC CAMERA: Invalid property with name \"%s\"", prop.first.c_str());
	}
}

float DynamicCamera::getMaxDistBetweenPlayers()
{
	std::vector<GameObject*> players = GameManager::GetInstance()->getKnights();
	// number of players
	int n = players.size();
	float maxDist = -1;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i != j) {
				float d = (players[i]->transform->getPosition() - players[j]->transform->getPosition()).magnitude();
				if (d > maxDist) maxDist = d;
			}
		}
	}

	return maxDist;
}

Vector3 DynamicCamera::getMidPointBetweenPlayers()
{
	// Vector with every player
	std::vector<GameObject*> players = GameManager::GetInstance()->getKnights();
	// number of players
	int n = players.size();
	float midX = 0.0f, midY = 0.0f;

	for (auto p : players) {
		midX += p->transform->getPosition().x / n;
		midY += p->transform->getPosition().y / n;
	}

	return Vector3(midX, midY, 0.0f);
}

void DynamicCamera::dynamicMove()
{
	// Move towards mid-point position
	Vector3 midPos = getMidPointBetweenPlayers();// +Vector3(0, 0, gameObject->transform->getPosition().z);

	// Zoom in/out
	float dist = getMaxDistBetweenPlayers();
	//clamp between minZ and maxZ
	dist *= zoomFactor;
	dist = std::min(maxZ, std::max(dist, minZ));

	Vector3 dest = midPos + Vector3(0, 0, dist);

	Vector3 lerpDest = gameObject->transform->getPosition();
	lerpDest.lerp(dest, smoothFactor);
	gameObject->transform->setPosition(lerpDest);
}

