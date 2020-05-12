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

	bool paused;
	int numPlayers;

	std::vector<Vector3> playerColours;
	std::vector<int> playerIndexes;
	std::vector<GameObject*> knights;

	std::string level;
	std::string song;

	std::string lastLevel;
	std::string lastSong;

	int health;
	int time;
	int maxTime;
	float bottomLimit;

	Score scores;


public:
	GameManager();
	GameManager(GameObject* gameObject);

	virtual ~GameManager();

	static GameManager* GetInstance();

	virtual void start();

	void reset();
	void pauseGame(bool setPaused);
	bool gameIsPaused();

	void setNumPlayers(int nPlayers);
	int getNumPlayers();

	void setPlayerIndexes(std::vector<int> playerIndexes);
	std::vector<int>& getPlayerIndexes();
	std::vector<GameObject*>& getKnights();
	std::vector<Vector3>& getPlayerColours();

	void emptyKnights();

	void setLevel(std::string level);
	std::string getLevel();
	std::string getLastLevel();

	void setSong(std::string song);
	std::string getSong();
	std::string getLastSong();

	void setHealth(int health);
	int getHealth();

	void setTime(int time);
	int getTime();
	int getInitialTime();

	Score* getScore();

	bool isAnyGhost() const;

	void pauseAllSounds();
	void resumeAllSound();
};

#endif