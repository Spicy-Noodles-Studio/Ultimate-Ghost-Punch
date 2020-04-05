#pragma once
#ifndef FIGHT_MANAGER_H
#define FIGHT_MANAGER_H

#include "UserComponent.h"

#include <vector>
#include <string>

#include "UIElement.h"
#include "Vector3.h"

class InputSystem;

class FightManager : public UserComponent
{
private:
	UIElement winnerPanel;
	UIElement winnerText;

	std::vector<int> playerIndexes;
	std::vector<Vector3> playerPositions = { {-20,30,0}, {20,30,0}, {-17.5,10,0}, {17.5,10,0} };

	float time;
	bool countingTime;

	int winner;

private:
	void createLevel();
	void createKnights();
	void playSong();

	void chooseWinner();

public:
	FightManager(GameObject* gameObject);
	virtual ~FightManager();

	virtual void start();
	virtual void update(float deltaTime);

	void playerDie();
};

#endif