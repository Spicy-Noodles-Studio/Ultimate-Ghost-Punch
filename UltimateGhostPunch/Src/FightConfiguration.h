#pragma once
#ifndef FIGHTCONFIGURATION_H
#define FIGHTCONFIGURATION_H

#include "UserComponent.h"

#include <vector>
#include <string>

class InputSystem;
class UIElement;

class FightConfiguration : public UserComponent
{
private:
	InputSystem* inputSystem;

	int nPlayers;
	std::vector<std::pair<int, UIElement>> slots;

	std::string level;
	std::string song;

private:
	void checkController();
	void checkKeyboard();

	void fillSlot(int slotIndex, int deviceIndex);
	void clearSlot(int index);

	int isIndexConnected(int index);

	void reorderSlots(int index);

	bool fightButtonClick();

public:
	FightConfiguration(GameObject* gameObject);
	~FightConfiguration();

	virtual void start();
	virtual void update(float deltaTime);

};

#endif