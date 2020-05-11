#pragma once
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <UserComponent.h>
#include <vector>
#include <string>

#include "Score.h"

class GameObject;

class GameManager : public UserComponent
{
private:
	static GameManager* instance;

	Score scores;

	std::vector<int> playerIndexes;
	std::vector<int> playerRanking;

	std::vector<Vector3> playerColours;
	std::vector<GameObject*> knights;

	std::string levelName;
	std::string songName;

	int initialPlayers;
	int playersAlive;
	int winner;

	bool paused;

	// Remember Configuration Menu's information
	int health;

	int time;
	int initialTime;
	bool timeMode;

	int level;
	int song;

public:
	GameManager();
	GameManager(GameObject* gameObject);

	virtual ~GameManager();

	static GameManager* GetInstance();

	virtual void start();

	void reset();
	void setPaused(bool setPaused);
	bool isPaused() const;

	Score* getScore();

	void setPlayerIndexes(std::vector<int>& playerIndexes);
	std::vector<int>& getPlayerIndexes();

	void initPlayerRanking(int tam);
	void setPlayerRanking(int index, int rank);
	int getPlayerRanking(int index) const;

	int getInitialPlayers() const;
	std::vector<Vector3>& getPlayerColours();
	std::vector<GameObject*>& getKnights();

	void setLevel(int level);
	int getLevel() const;

	void setLevelName(std::string name);
	std::string getLevelName() const;

	void setSong(int song);
	int getSong() const;

	void setSongName(std::string name);
	std::string getSongName() const;

	void setHealth(int health);
	int getHealth() const;

	void setTime(int time);
	int getTime() const;
	int getInitialTime() const;

	void setTimeMode(bool mode);
	bool getTimeMode() const;

	void setPlayersAlive(int players);
	int getPlayersAlive() const;

	void setWinner(int winner);
	int getWinner() const;
};

#endif