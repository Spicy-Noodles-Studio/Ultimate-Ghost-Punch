#pragma once
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "UserComponent.h"
#include "Score.h"
#include <vector>
#include <string>

class GameObject;

class GameManager : public UserComponent
{
private:
	static GameManager* instance;

	bool paused;

	int numPlayers;

	std::vector<int> playerIndexes;
	std::vector<GameObject*> knights;

	std::string level;
	std::string song;

	int health;
	int time;
	Score scores;
	// Bottom limit of the level 
	float bottomLimit;

public:
	GameManager();
	GameManager(GameObject* gameObject);

	virtual ~GameManager();

	static GameManager* GetInstance();

	virtual void start();

	void setNumPlayers(int nPlayers);
	int getNumPlayers();
	void reset();
	void pauseGame(bool setPaused);
	bool gameIsPaused();

	void setPlayerIndexes(std::vector<int> playerIndexes);
	std::vector<int>& getPlayerIndexes();
	std::vector<GameObject*>& getKnights();

	void setLevel(std::string level);
	std::string getLevel();

	void setSong(std::string song);
	std::string getSong();

	void setHealth(int health);
	int getHealth();

	void setTime(int time);
	int getTime();
	Score* getScore();
};

#endif