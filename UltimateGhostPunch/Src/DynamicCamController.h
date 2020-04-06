#pragma once
#ifndef DYNAMIC_CAM_CONTROLLER_H
#define DYNAMIC_CAM_CONTROLLER_H

#include <UserComponent.h>

class Movement;

class DynamicCamController : public UserComponent
{
private:
	Movement* camMove = nullptr;
	float minZ;
	float maxZ;

	// Stores the previous maximum distance between two players
	float prevDist;

	// Cam will not move if dist to mid-point is <= threshold
	float mpThreshold;
	float zoomThreshold;

public:
	DynamicCamController(GameObject* gameObject);
	~DynamicCamController();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

	// Returns the maximum distance that exists currently between two players
	float getMaxDistBetweenPlayers();

	// Returns the mid-point between every character on screen
	Vector3 getMidPointBetweenPlayers();

	// Applies a movement in the direction specified by the players' mid-point and distance
	void dynamicMove();
};

#endif