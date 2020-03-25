#pragma once

#include "UserComponent.h"

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

	void createHearts();

	void updateHearts();
	void updateIndicator();
};