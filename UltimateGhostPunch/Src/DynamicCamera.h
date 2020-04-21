#pragma once
#ifndef DYNAMIC_CAMERA_H
#define DYNAMIC_CAMERA_H

#include <UserComponent.h>
#include <Vector3.h>

class DynamicCamera : public UserComponent
{
private:
	float minZ, maxZ;

	enum CameraState {
		NORMAL, SLOWMO
	};
	CameraState state;

	//Cam's Z pos = max dist between players * zoomFactor
	float zoomFactor;
	float smoothFactor;

	float time;
	float slowMoTime;

	Vector3 dest;

	GameObject* ghostDoingUGP;

public:
	DynamicCamera(GameObject* gameObject);
	~DynamicCamera();

	virtual void preUpdate(float deltaTime);
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

private:
	// Returns the maximum distance that exists currently between two players
	float getMaxDistBetweenPlayers();

	// Returns the mid-point between every character on screen
	Vector3 getMidPointBetweenPlayers();

	// Applies a movement in the direction specified by the players' mid-point and distance
	void dynamicMove();

	GameObject* someoneDoingUGP();

};

#endif
