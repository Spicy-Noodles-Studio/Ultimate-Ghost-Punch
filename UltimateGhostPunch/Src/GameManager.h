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

	std::string level;
	std::string lastLevel;

	std::string song;
	std::string lastSong;

	int health;
	int time;
	int maxTime;

	int playersAlive;
	int winner;
	
	bool paused;

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

	std::vector<Vector3>& getPlayerColours();
	std::vector<GameObject*>& getKnights();

	void setLevel(std::string level);
	std::string getLevel() const;
	std::string getLastLevel() const;

	void setSong(std::string song);
	std::string getSong() const;
	std::string getLastSong() const;

	void setHealth(int health);
	int getHealth() const;

	void setTime(int time);
	int getTime() const;
	int getInitialTime() const;

	void setPlayersAlive(int players);
	int getPlayersAlive() const;

	void setWinner(int winner);
	int getWinner() const;
};

#endif