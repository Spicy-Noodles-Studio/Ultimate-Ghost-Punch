#pragma once
#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include <UserComponent.h>
#include <UIElement.h>

class SoundEmitter;
class InputSystem;

class PauseMenu : public UserComponent
{
private:
	InputSystem* inputSystem;
	UIElement pauseMenu;
	UIElement optionsMenu;
	SoundEmitter* soundEmitter;

	bool backButtonClick();
	bool optionsButton();
	bool resumeButton();
	void buttonClick(const std::string& sound);
	
public:
	PauseMenu(GameObject* gameObject);
	virtual ~PauseMenu();

	virtual void start();
	virtual void preUpdate(float deltaTime);

	// opens the Pause Menu and pauses the game
	void setPaused(bool paused);
	bool isVisible();
};

#endif