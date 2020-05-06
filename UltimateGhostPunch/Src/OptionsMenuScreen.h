#pragma once
#ifndef OPTIONS_MENU_SCREEN_H
#define OPTIONS_MENU_SCREEN_H

#include "OptionsMenu.h"

class GameObject;
class SoundEmitter;

class OptionsMenuScreen : public OptionsMenu
{
private:
	GameObject* screen;
	SoundEmitter* soundEmitter;

	UIElement pauseMenu;
	UIElement optionsMenu;

	bool backToMenuButtonClick();
	void buttonClick(const std::string& sound);

public:
	OptionsMenuScreen(GameObject* gameObject);
	virtual ~OptionsMenuScreen();

	virtual void start();
};

#endif