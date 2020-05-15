#pragma once
#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include <UIElement.h>
#include "Menu.h"

class Countdown;

class PauseMenu : public Menu
{
private:
	Countdown* countdown;

	UIElement pauseMenu;
	UIElement pausePanel;

	UIElement optionsMenu;

	bool resumeButtonClick();
	bool optionsButtonClick();
	bool exitButtonClick();

	bool checkControllersInput();
	
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