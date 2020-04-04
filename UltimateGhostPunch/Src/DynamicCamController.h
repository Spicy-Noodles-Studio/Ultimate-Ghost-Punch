#pragma once

#include <UserComponent.h>

class Movement;

class DynamicCamController : public UserComponent
{
private:
	Movement* camMove = nullptr;
	float minZ = 10,
		maxZ = 100;

	// Stores the previous maximum distance between two players
	float prevDist = 0.0f;
	// Cam will not move if dist to mid-point is <= threshold
	float mpThreshold = 100.0f;
	float zoomThreshold = 5.0f;
public:
	DynamicCamController(GameObject* gameObject);

	virtual void handleData(ComponentData* data);

	virtual void start();
	virtual void update(float deltaTime);

	// Returns the maximum distance that exists currently between two players
	float getMaxDistBetweenPlayers();
	// Returns the mid-point between every character on screen
	Vector3 getMidPointBetweenPlayers();
	// Applies a movement in the direction specified by the players' mid-point and distance
	void dynamicMove();
};

