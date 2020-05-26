#pragma once
#ifndef OPTIONS_MENU_SCREEN_H
#define OPTIONS_MENU_SCREEN_H

#include "OptionsMenu.h"

class OptionsMenuScreen : public OptionsMenu
{
private:
	GameObject* screen;

	UIElement pauseMenu;
	UIElement optionsMenu;

	bool backToMenuButtonClick();

protected:
	virtual void start();
	virtual void preUpdate(float deltaTime);

public:
	OptionsMenuScreen(GameObject* gameObject);
	virtual ~OptionsMenuScreen();
};

#endif