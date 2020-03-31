#pragma once
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "UserComponent.h"

#include <vector>
#include <string>

class GameObject;

class GameManager : public UserComponent
{
private:
	static GameManager* instance;

	bool paused;

	int nPlayers;

	std::vector<int> playerIndexes;
	std::vector<GameObject*> knights;

	std::string level;
	std::string song;

	int health;
	int time;

public:
	GameManager();
	GameManager(GameObject* gameObject);
	~GameManager();

	static GameManager* GetInstance();

	virtual void start();
	virtual void update(float deltaTime);

	void setNPlayers(int nPlayers);
	int getNPlayers();

	std::vector<GameObject*>& getKnights();

	void setLevel(std::string level);
	std::string getLevel();

	void setSong(std::string song);
	std::string getSong();

	void setHealth(int health);
	int getHealth();

	void setTime(int time);
	int getTime();

	void setPlayerIndexes(std::vector<int> playerIndexes);
	std::vector<int>& getPlayerIndexes();

};

#endif