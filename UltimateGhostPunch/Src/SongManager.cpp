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

	soundEmitter = nullptr;
	gameManager = nullptr;
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
	if(!notNull(soundEmitter)) soundEmitter = gameObject->getComponent<SoundEmitter>();

	if (!songPlaying && notNull(soundEmitter)) {
		soundEmitter->playMusic(menuMusic);
		songPlaying = true;
	}
}

void SongManager::stopMenuSong()
{
	if (songPlaying && notNull(soundEmitter)) {
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
	if (notNull(soundEmitter)) soundEmitter->playMusic(song);
}

void SongManager::stopSong(const std::string& song)
{
	if (notNull(soundEmitter)) soundEmitter->stop(song);
}

void SongManager::pauseSong(const std::string& song)
{
	if (notNull(soundEmitter)) soundEmitter->pause(song);
}

void SongManager::resumeSong(const std::string& song)
{
	if (notNull(soundEmitter)) soundEmitter->resume(song);
}

void SongManager::play2DSound(const std::string& sound)
{
	if (notNull(soundEmitter)) soundEmitter->playSound(sound);
}

void SongManager::ghostSong()
{
	if (notNull(soundEmitter) && notNull(gameManager) && gameManager->isAnyGhost())
		soundEmitter->setPitch(0.65);
	else if(notNull(soundEmitter))
		soundEmitter->setPitch(1.0);
}
