#pragma once
#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include <UserComponent.h>
#include <UIElement.h>

class InputSystem;

class PauseMenu : public UserComponent
{
private:
	InputSystem* inputSystem;
	UIElement pauseMenu;

	bool backButtonClick();
public:
	PauseMenu(GameObject* gameObject);
	~PauseMenu();

	virtual void start();
	virtual void preUpdate(float deltaTime);

	// opens the Pause Menu and pauses the game
	void setPaused(bool paused);
	bool isVisible();
};

#endif