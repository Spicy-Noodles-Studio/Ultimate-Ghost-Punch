#pragma once
#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include "Menu.h"

#include <UIElement.h>

class InputSystem;

class PauseMenu : public Menu
{
private:
	InputSystem* inputSystem;
	UIElement pauseMenu;
	UIElement optionsMenu;

	bool optionsButton();
	bool resumeButton();
	bool backButtonClick();
	
public:
	PauseMenu(GameObject* gameObject);
	virtual ~PauseMenu();

	virtual void start();
	virtual void preUpdate(float deltaTime);

	// opens the Pause Menu and pauses the game
	void setPaused(bool paused);
	bool isVisible();
};

#endif