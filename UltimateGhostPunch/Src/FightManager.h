#pragma once
#ifndef FIGHTMANAGER_H
#define FIGHTMANAGER_H

#include "UserComponent.h"

#include <vector>
#include <string>

#include "UIElement.h"

class InputSystem;

class FightManager : public UserComponent
{
private:
	UIElement winnerPanel;
	UIElement winnerText;

	std::vector<int> playerIndexes;

	float time;
	bool timed;

	int winner;

private:
	void createLevel();
	void createKnights();
	void playSong();

	void chooseWinner();

public:
	FightManager(GameObject* gameObject);
	~FightManager();

	virtual void start();
	virtual void update(float deltaTime);

};

#endif