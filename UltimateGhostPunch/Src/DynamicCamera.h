#pragma once
#ifndef DYNAMIC_CAMERA_H
#define DYNAMIC_CAMERA_H
#include <UserComponent.h>

class GameObject;

class DynamicCamera :
	public UserComponent
{
private:
	float minZ, maxZ;

	//Cam's Z pos = max dist between players * zoomFactor
	float zoomFactor;
	float smoothFactor;

public:
	DynamicCamera(GameObject* gameObject);

	virtual void handleData(ComponentData* data);

	virtual void start();
	virtual void fixedUpdate(float deltaTime);

	// Returns the maximum distance that exists currently between two players
	float getMaxDistBetweenPlayers();
	// Returns the mid-point between every character on screen
	Vector3 getMidPointBetweenPlayers();
	// Applies a movement in the direction specified by the players' mid-point and distance
	void dynamicMove();

	Vector3 Lerp(Vector3 s, Vector3 d, float f);
};

#endif
