#pragma once
#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "Menu.h""

#include <UIElement.h>

class InputSystem;
class UILayout;

class MainMenu : public Menu
{
private:
	InputSystem* inputSystem;

	bool singlePlayerButtonClick();
	bool multiplayerButtonClick();

	bool optionsButtonClick();
	bool exitButtonClick();

public:
	MainMenu(GameObject* gameObject);
	virtual ~MainMenu();

	virtual void start();
};

#endif