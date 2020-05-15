#pragma once
#ifndef PLAYER_UI_H
#define PLAYER_UI_H

#include <UserComponent.h>
#include <UIElement.h>

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

	void setVisible(bool visible);

	void setVisible(bool visible);
private:
	void createHearts();
	void updateHearts();

	void createGhost();
	void updateGhost();

	void updateIndicator();

private:
	Health* health;
	GhostManager* ghostManager;
	Camera* mainCamera;

	UIElement playerHUD;
	UIElement playerIndicator;

	std::string name;

	bool isVisible;
};

#endif