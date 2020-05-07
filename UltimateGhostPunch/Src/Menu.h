#pragma once

#ifndef MENU_H
#define MENU_H

#include <UserComponent.h>

class SoundEmitter;

class Menu : public UserComponent
{
protected:
	std::string backSound = "back";
	std::string buttonSound = "button4";
	SoundEmitter* soundEmitter;
	GameObject* mainCamera;

	virtual void buttonClick(const std::string& sound);
	virtual bool backButtonClick();

public:
	Menu(GameObject* gameObject);
	~Menu();

	virtual void start();
};

#endif