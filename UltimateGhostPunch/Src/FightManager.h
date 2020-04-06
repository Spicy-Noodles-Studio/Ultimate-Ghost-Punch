#pragma once
#ifndef FIGHTMANAGER_H
#define FIGHTMANAGER_H

#include "UserComponent.h"

#include <vector>
#include <string>

#include "Vector3.h"
#include "UIElement.h"

class InputSystem;

class FightManager : public UserComponent
{
private:
	UIElement winnerPanel;
	UIElement winnerText;

	std::vector<int> playerIndexes;

	std::vector<Vector3> playerPositions = { {-20,30,0}, {20,30,0}, {-17.5,10,0}, {17.5,10,0} };

	float time;
	bool timed;

	int winner;

	void createLevel();
	void createKnights();
	void playSong();

	void chooseWinner();

public:
	FightManager(GameObject* gameObject);
	~FightManager();

	virtual void start();
	virtual void update(float deltaTime);

	void playerDie();

};

#endif