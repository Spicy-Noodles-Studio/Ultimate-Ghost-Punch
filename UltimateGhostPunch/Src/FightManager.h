#pragma once
#ifndef FIGHT_MANAGER_H
#define FIGHT_MANAGER_H

#include <UserComponent.h>
#include <UIElement.h>
#include <Vector3.h>

#include <vector>
#include <string>

class GameManager;
class UILayout;
class InputSystem;

class FightManager : public UserComponent
{
private:
	GameManager* gameManager;
	UILayout* fightLayout;
	UIElement timeText;
	UIElement winnerPanel;
	UIElement winnerText;

	std::vector<int> playerIndexes;
	std::vector<Vector3> playerPositions;

	float fightTimer; // If time is -1, then infinite
	float finishTimer; // Time taken to send us back to MainMenu
	std::vector<std::pair<Vector3, Vector3>> playerTransforms;
	std::vector<std::pair<Vector3, Vector3>> spikesTransforms;
	int nSpikes;

	bool timed;

	int winner;

	void createLevel();
	void createKnights();
	void createSpikes();
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