#pragma once
#ifndef CONTROLS_MENU_H
#define CONTROLS_MENU_H

#include "Menu.h"

#include <vector>
#include <string>

class ControlsMenu : public Menu
{
private:
	std::vector<std::string> actions;
	std::vector<std::string> keyboard;
	std::vector<std::string> controller;

	bool checkControllersInput();

public:
	ControlsMenu(GameObject* gameObject);
	virtual ~ControlsMenu();

	virtual void start();
	virtual void update(float deltaTime);
};

#endif