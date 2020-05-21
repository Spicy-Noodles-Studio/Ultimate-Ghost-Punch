#pragma once
#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "Menu.h"

#include <UIElement.h>

class UILayout;
class WindowManager;

class MainMenu : public Menu
{
private:
	bool playButtonClick();
	bool optionsButtonClick();
	bool exitButtonClick();

	bool controlsButtonClick();
	bool creditsButtonClick();

	WindowManager* windowManager;

public:
	MainMenu(GameObject* gameObject);
	virtual ~MainMenu();

	virtual void start();
};

#endif