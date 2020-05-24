#pragma once
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <UserComponent.h>
#include <vector>
#include <queue>
#include <string>

#include "Score.h"

typedef std::pair<int, int> ii;

class Less
{
public:
	bool operator()(const ii& a, const ii& b)
	{
		return a.second < b.second;
	}
};

class GameObject;

class GameManager : public UserComponent
{
private:
	static GameManager* instance;

	Score scores;

	std::vector<int> playerIndexes;
	std::vector<int> playerRanking;
	std::priority_queue<ii, std::vector<ii>, Less> ranking;

	std::vector<Vector3> playerColours;
	std::vector<GameObject*> knights;

	std::string level;
	std::string levelName;

	std::string song;
	std::string songName;

	int initialPlayers;
	int winner;

	bool paused;

	// Remember Configuration Menu's information
	int health;

	int time;
	int initialTime;
	bool timeMode;

	float initialBrightness;

protected:
	virtual void start();

public:
	GameManager();
	GameManager(GameObject* gameObject);

	virtual ~GameManager();

	static GameManager* GetInstance();

	Score* getScore();

	void setPlayerIndexes(std::vector<int>& playerIndexes);
	std::vector<int>& getPlayerIndexes();

	void setPlayerRanking(int index, int rank);
	int getPlayerRanking(int index) const;

	std::priority_queue<ii, std::vector<ii>, Less>& getRanking();
	void emptyRanking();

	std::vector<Vector3>& getPlayerColours();

	std::vector<GameObject*>& getKnights();
	void emptyKnights();

	void setLevel(std::string level, std::string name);
	std::pair<std::string, std::string> getLevel() const;

	void setSong(std::string song, std::string name);
	std::pair<std::string, std::string> getSong() const;

	void setInitialPlayers(int players);
	int getInitialPlayers() const;

	void setWinner(int winner);
	int getWinner() const;

	void setPaused(bool setPaused);
	bool isPaused() const;

	// Remember Configuration Menu's information
	void setHealth(int health);
	int getHealth() const;

	void setTime(int time);
	int getTime() const;
	int getInitialTime() const;

	void setTimeMode(bool mode);
	bool getTimeMode() const;

	float getInitialBrightness() const;

	// Other functions
	GameObject* getAnyGhost();
	std::vector<GameObject*> getAlivePlayers(bool getGhosts);

	void pauseAllSounds();
	void resumeAllSound();

	bool playerUsingKeyboard() const;
};

#endif