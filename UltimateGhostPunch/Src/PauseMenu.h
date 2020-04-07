#pragma once
#include <UserComponent.h>
#include "UIElement.h"

class InputSystem;

class PauseMenu : public UserComponent
{
private:
	UIElement pauseMenu;
	InputSystem* inputSystem;
public:
	PauseMenu(GameObject* gameObject);

	virtual void start();
	virtual void update(float deltaTime);

	// opens the Pause Menu and pauses the game
	void setPaused(bool paused);
	bool isVisible();
};

