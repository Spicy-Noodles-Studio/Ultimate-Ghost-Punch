#pragma once
#ifndef FIGHTMANAGER_H
#define FIGHTMANAGER_H

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

	std::vector<std::pair<Vector3, Vector3>> playerTransforms;
	std::vector<std::pair<Vector3, Vector3>> spikesTransforms;
	int nSpikes;

	float time;
	bool timed;

	int winner;

private:
	void createLevel();
	void createKnights();
	void createSpikes();
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