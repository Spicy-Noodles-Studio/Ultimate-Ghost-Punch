#pragma once

#ifndef MENU_H
#define MENU_H

#include <UserComponent.h>

class GameManager;
class InputSystem;
class InterfaceSystem;
class SongManager;

class Menu : public UserComponent
{
protected:
	std::string backSound = "back";
	std::string buttonSound = "button4";

	GameObject* mainCamera;
	GameManager* gameManager;
	InputSystem* inputSystem;
	InterfaceSystem* interfaceSystem;
	SongManager* songManager;

	virtual void buttonClick(const std::string& sound);
	virtual bool backButtonClick();

public:
	Menu(GameObject* gameObject);
	~Menu();

	virtual void start();
};

#endif