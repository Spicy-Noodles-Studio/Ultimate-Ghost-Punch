#pragma once

#include "UserComponent.h"
#include "UIElement.h"

class Health;

class PlayerUI : public UserComponent
{
public:
	PlayerUI(GameObject* gameObject);
	virtual ~PlayerUI();

	virtual void start();
	virtual void update(float deltaTime);

	void updateState(const std::string state);
	void updateHealth();

	void setPauseMenuVisible(bool show);
	bool isPauseMenuVisible();

private:
	Health* health;

	UIElement playerHUD;
	UIElement playerIndicator;

	UIElement pauseMenu;

private:
	void createHearts();

	void updateHearts();
	void updateIndicator();
};