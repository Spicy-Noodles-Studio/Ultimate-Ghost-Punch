#include "SongManager.h"

#include <GameObject.h>
#include <SoundEmitter.h>
#include <ComponentRegister.h>

#include "GameManager.h"

REGISTER_FACTORY(SongManager);

SongManager* SongManager::instance = nullptr;

SongManager::SongManager() : UserComponent(nullptr), songPlaying(false), soundEmitter(nullptr), gameManager(nullptr)
{
}

SongManager::SongManager(GameObject* gameObject) : UserComponent(gameObject), songPlaying(false), soundEmitter(nullptr), gameManager(nullptr)
{
	if (instance == nullptr)
		instance = this;
	else
		destroy(gameObject);
}

SongManager::~SongManager()
{
	if (instance == this)
		instance = nullptr;
}

SongManager* SongManager::GetInstance()
{
	return instance;
}

void SongManager::start()
{
	gameManager = GameManager::GetInstance();
	soundEmitter = gameObject->getComponent<SoundEmitter>();
	checkNull(gameManager);
	checkNull(soundEmitter);
	dontDestroyOnLoad(gameObject);
}

void SongManager::update(float deltaTime)
{
	ghostSong();
}

void SongManager::playMenuSong()
{
	if(soundEmitter == nullptr) soundEmitter = gameObject->getComponent<SoundEmitter>();

	if (!songPlaying && soundEmitter != nullptr) {
		soundEmitter->playMusic(menuMusic);
		songPlaying = true;
	}
}

void SongManager::stopMenuSong()
{
	if (songPlaying && soundEmitter != nullptr) {
		soundEmitter->stop(menuMusic);
		songPlaying = false;
	}
}

void SongManager::pauseMenuSong()
{
	pauseSong(menuMusic);
}

void SongManager::resumeMenuSong()
{
	resumeSong(menuMusic);
}

void SongManager::playSong(const std::string& song)
{
	if (soundEmitter != nullptr) soundEmitter->playMusic(song);
}

void SongManager::stopSong(const std::string& song)
{
	if (soundEmitter != nullptr) soundEmitter->stop(song);
}

void SongManager::pauseSong(const std::string& song)
{
	if (soundEmitter != nullptr) soundEmitter->pause(song);
}

void SongManager::resumeSong(const std::string& song)
{
	if (soundEmitter != nullptr) soundEmitter->resume(song);
}

void SongManager::play2DSound(const std::string& sound)
{
	if (soundEmitter != nullptr) soundEmitter->playSound(sound);
}

void SongManager::ghostSong()
{
	if (gameManager != nullptr && soundEmitter != nullptr && gameManager->isAnyGhost())
		soundEmitter->setPitch(0.65);
	else if(soundEmitter !=nullptr)
		soundEmitter->setPitch(1.0);
}
