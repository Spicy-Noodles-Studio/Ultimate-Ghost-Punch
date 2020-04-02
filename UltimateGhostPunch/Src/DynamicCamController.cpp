#include "DynamicCamController.h"
#include "GameObject.h"
#include <sstream>

#include "Movement.h"
#include "GameManager.h"
#include <queue>

DynamicCamController::DynamicCamController(GameObject* gameObject) : UserComponent(gameObject)
{
}

void DynamicCamController::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "minZ") {
			if (!(ss >> minZ))
				LOG("ATTACK: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "maxZ") {
			if (!(ss >> maxZ))
				LOG("ATTACK: Invalid property with name \"%s\"", prop.first.c_str());
		}
	}
}

void DynamicCamController::start()
{
	camMove = gameObject->getComponent<Movement>();
}

void DynamicCamController::update(float deltaTime)
{
	dynamicMove();
}

float DynamicCamController::getMaxDistBetweenPlayers()
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

Vector3 DynamicCamController::getMidPointBetweenPlayers()
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

void DynamicCamController::dynamicMove()
{
	Vector3 movDir;
	// Move towards mid-point position
	Vector3 midPos = getMidPointBetweenPlayers() + Vector3(0, 0, gameObject->transform->getPosition().z);
	movDir = midPos - gameObject->transform->getPosition();
	
	
	if (movDir.magnitude() > mpThreshold)
	{
		movDir.normalize();
		camMove->move(movDir);
	}
	//else
		//camMove->stop();

	// Zoom in/out
	printf("Dist: %f\n", getMaxDistBetweenPlayers());
}

