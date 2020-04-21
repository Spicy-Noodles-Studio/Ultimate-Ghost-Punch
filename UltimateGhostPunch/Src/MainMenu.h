#pragma once
#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <UserComponent.h>
#include <UIElement.h>

class InputSystem;
class UILayout;

class MainMenu : public UserComponent
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
};

#endif