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
	std::string levelName;

	std::string song;
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

	float initialBrightness;

public:
	GameManager();
	GameManager(GameObject* gameObject);

	virtual ~GameManager();

	static GameManager* GetInstance();

	virtual void start();

	void setPaused(bool setPaused);
	bool isPaused() const;

	Score* getScore();

	void setPlayerIndexes(std::vector<int>& playerIndexes);
	std::vector<int>& getPlayerIndexes();

	void initPlayerRanking(int tam);
	void setPlayerRanking(int index, int rank);
	int getPlayerRanking(int index) const;

	void setInitialPlayers(int players);
	int getInitialPlayers() const;

	std::vector<Vector3>& getPlayerColours();
	std::vector<GameObject*>& getKnights();

	void emptyKnights();

	void setLevel(std::string level, std::string name);
	std::pair<std::string, std::string> getLevel() const;

	void setSong(std::string song, std::string name);
	std::pair<std::string, std::string> getSong() const;

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

	bool isAnyGhost() const;

	void pauseAllSounds();
	void resumeAllSound();

	bool playerUsingKeyboard() const;

	float getInitialBrightness() const;
};

#endif