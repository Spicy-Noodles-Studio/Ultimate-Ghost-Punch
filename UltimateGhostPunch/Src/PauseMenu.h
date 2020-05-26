#pragma once
#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include <UIElement.h>
#include "Menu.h"

class Game;
class Countdown;

class PauseMenu : public Menu
{
private:
	Game* game;
	Countdown* countdown;

	UIElement pauseMenu;
	UIElement pausePanel;

	UIElement optionsMenu;

	bool resumeButtonClick();
	bool optionsButtonClick();
	bool exitButtonClick();

	bool checkControllersInput();
	
protected:
	virtual void start();
	virtual void preUpdate(float deltaTime);

public:
	PauseMenu(GameObject* gameObject);
	virtual ~PauseMenu();

	// Opens the Pause Menu and pauses the game
	void setPaused(bool paused);
	bool isVisible();
};

#endif