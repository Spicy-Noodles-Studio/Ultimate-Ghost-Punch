#pragma once
#ifndef TRAIL_MANAGER
#define TRAIL_MANAGER
#include <UserComponent.h>

class Trail;
class PlayerState;

class TrailManager : public UserComponent
{
private:
	Trail* quickAttackTrail;
	Trail* heavyAttackTrail;
	Trail* dashTrail;
	Trail* thrownTrail;
	Trail* UGPTrail;

	PlayerState* playerState;

	float time;
	float thrownTime;

protected:
	virtual void start();
	virtual void preUpdate(float deltaTime);

public:
	TrailManager(GameObject* gameObject);
	virtual ~TrailManager();

private:
	// Trails creation
	void createTrail(Trail** trail, const std::string& trailFilename);

	// Trails management
	void manageQuickAttackTrail();
	void manageHeavyAttackTrail();
	void manageDashTrail();
	void manageThrownTrail(float deltaTime);
	void manageUGPTrail();
};

#endif