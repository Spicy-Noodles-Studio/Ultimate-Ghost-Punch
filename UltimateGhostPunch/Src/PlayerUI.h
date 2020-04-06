#pragma once
#ifndef PLAYER_UI_H
#define PLAYER_UI_H

#include "UserComponent.h"
#include "UIElement.h"

class Camera;
class Health;
class GhostManager;

class PlayerUI : public UserComponent
{
public:
	PlayerUI(GameObject* gameObject);
	virtual ~PlayerUI();

	virtual void start();
	virtual void update(float deltaTime);

	void changeState(const std::string state);
	void updateHealth();

	// TODO: quitar esto, aqui sobra
	//void setPauseMenuVisible(bool show);
	//bool isPauseMenuVisible();

private:
	void createHearts();

	void updateState();
	void updateHearts();
	void updateIndicator();

private:
	Camera* mainCamera;
	Health* health;
	GhostManager* ghostManager;

	std::string name;
	UIElement playerHUD;
	UIElement playerIndicator;

	// TODO: quitar esto, aqui sobra
	//UIElement pauseMenu;
};

#endif