#pragma once
#ifndef SONGMANAGER_H
#define SONGMANAGER_H

#include <UserComponent.h>

class SoundEmitter;
class GameManager;

class SongManager : public UserComponent
{
private:
	static SongManager* instance;

	SoundEmitter* soundEmitter;
	GameManager* gameManager;

	bool songPlaying;
	std::string menuMusic = "cycles";

protected:
	virtual void start();
	virtual void update(float deltaTime);

public:
	SongManager();
	SongManager(GameObject* gameObject);
	~SongManager();

	static SongManager* GetInstance();

	void playMenuSong();
	void stopMenuSong();
	void pauseMenuSong();
	void resumeMenuSong();
	void playSong(const std::string& song);
	void stopSong(const std::string& song);
	void pauseSong(const std::string& song);
	void resumeSong(const std::string& song);

	void play2DSound(const std::string& sound);

	void ghostSong();
};

#endif

