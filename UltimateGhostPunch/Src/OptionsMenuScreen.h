#pragma once
#ifndef OPTIONS_MENU_SCREEN_H
#define OPTIONS_MENU_SCREEN_H

#include "OptionsMenu.h"

class GameObject;

class OptionsMenuScreen : public OptionsMenu
{
private:
	GameObject* screen;

	UIElement pauseMenu;
	UIElement optionsMenu;

	bool backToMenuButtonClick();

public:
	OptionsMenuScreen(GameObject* gameObject);
	virtual ~OptionsMenuScreen();

	virtual void start();
	virtual void preUpdate(float deltaTime);
};

#endif