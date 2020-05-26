#pragma once
#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <UserComponent.h>
#include <Vector3.h>

class CameraController : public UserComponent
{
private:
	enum CameraState
	{
		MIDPOINT, SLOWMO
	};

	CameraState state;
	float minX, maxX, minY, maxY, minZ, maxZ;

	// Cam's Z pos = max dist between players * zoomFactor
	float zoomFactor;
	float smoothFactor;

	// Camera follows the target
	Vector3 target;

	float time;
	float slowMoTime;
	float slowMoDistance;
	float slowMoTimeScale;
	float slowMoZ;

	GameObject* playerPunching;

protected:
	virtual void preUpdate(float deltaTime);
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

public:
	CameraController(GameObject* gameObject);
	virtual ~CameraController();
	void setMinZ(float minZ);
	void setMaxZ(float maxZ);
	void setMinX(float minX);
	void setMaxX(float maxX);
	void setMinY(float minY);
	void setMaxY(float maxY);


private:
	void smoothMove();
	void handleState();

	// Returns the maximum distance that exists currently between two players
	float getMaxDistBetweenPlayers();

	// Returns the mid-point between every character on screen
	Vector3 getMidPointBetweenPlayers();

	// Adjusts mid-point to boundaries and zoom
	void getMidPointAdjusted(Vector3* midPoint, float zoom);

	// Set the target position specified by the players' mid-point and distance
	void setTargetToMidPointPlayers();

	// Set the target position to the player punching position and slowMoZ
	void setTargetToSlowMo();

	// If there is someone punching activates the slowMo effect
	void checkSlowMo();

	void activateSlowMo();
	void deactivateSlowMo();

	// Returns the first player punching
	GameObject* someonePunching();
	
};

#endif