#pragma once
#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <UserComponent.h>
#include <UIElement.h>

class InputSystem;
class UILayout;
class SoundEmitter;

class MainMenu : public UserComponent
{
private:
	InputSystem* inputSystem;
	SoundEmitter* soundEmitter;

	bool singlePlayerButtonClick();
	bool multiplayerButtonClick();

	bool optionsButtonClick();
	bool exitButtonClick();

	void buttonClick(const std::string& sound);

public:
	MainMenu(GameObject* gameObject);
	virtual ~MainMenu();

	virtual void start();
};

#endif