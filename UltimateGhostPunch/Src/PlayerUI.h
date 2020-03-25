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
private:
	Health* health;

	UIElement playerHUD;
	UIElement playerIndicator;

private:
	void createHearts();

	void updateHearts();
	void updateIndicator();
};