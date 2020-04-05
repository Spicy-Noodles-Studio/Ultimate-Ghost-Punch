#include "DynamicCamera.h"
#include "GameObject.h"
#include <sstream>

#include "GameManager.h"
#include <queue>
#include <algorithm>

#include "ComponentRegister.h"

REGISTER_FACTORY(DynamicCamera);

DynamicCamera::DynamicCamera(GameObject* gameObject) : UserComponent(gameObject), smoothFactor(0.125f), minZ(20), maxZ(100), zoomFactor(1.0f)
{
}

void DynamicCamera::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "minZ") {
			if (!(ss >> minZ))
				LOG("DYNAMIC CAMERA: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "maxZ") {
			if (!(ss >> maxZ))
				LOG("DYNAMIC CAMERA: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "smoothFactor") {
			if (!(ss >> smoothFactor))
				LOG("DYNAMIC CAMERA: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "zoomFactor") {
			if (!(ss >> zoomFactor))
				LOG("DYNAMIC CAMERA: Invalid property with name \"%s\"", prop.first.c_str());
		}
	}
}

void DynamicCamera::start()
{
}

void DynamicCamera::fixedUpdate(float deltaTime)
{
	dynamicMove();
}

float DynamicCamera::getMaxDistBetweenPlayers()
{
	std::vector<GameObject*> players = GameManager::GetInstance()->getKnights();
	// number of players
	int n = players.size();
	float maxDist = -1;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i != j)
			{
				float d = (players[i]->transform->getPosition() - players[j]->transform->getPosition()).magnitude();
				if (d > maxDist) maxDist = d;
			}
		}
	}

	return maxDist;
}

Vector3 DynamicCamera::getMidPointBetweenPlayers()
{
	// mid point
	Vector3 mp;
	// Queue with every player's position
	std::queue<Vector3> pts;
	// Vector with every player
	std::vector<GameObject*> players = GameManager::GetInstance()->getKnights();
	// number of players
	int n = players.size();

	float midX = 0.0f, midY = 0.0f;
	for (auto p : players)
	{
		midX += p->transform->getPosition().x / n;
		midY += p->transform->getPosition().y / n;
	}

	mp = { midX, midY, 0.0f };

	return mp;
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

	gameObject->transform->setPosition(Lerp(gameObject->transform->getPosition(), dest, smoothFactor));
}

Vector3 DynamicCamera::Lerp(Vector3 s, Vector3 d, float f)
{
	//clamp between 0 and 1
	f = std::min(1.0f, std::max(f, 0.0f));

	return Vector3(s.x + (d.x - s.x) * f, s.y + (d.y - s.y) * f, s.z + (d.z - s.z) * f);
}

