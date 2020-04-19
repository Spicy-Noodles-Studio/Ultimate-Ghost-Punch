#pragma once
#include <UserComponent.h>





class GhostAnimController :
	public UserComponent
{
private:

	// Checks if there has been a change of state and updates the state if that is the case
	void updateState();
	// For the current state of the player, plays the appropriate animation
	void handleState();
public:
	GhostAnimController(GameObject* gameObject);


	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);
};

