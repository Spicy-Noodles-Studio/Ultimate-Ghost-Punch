#include "SoundManager.h"

#include <ComponentRegister.h>

#include <ResourcesManager.h>

#include "SoundSystem.h"

#include "SoundEmitter.h"

#include "SoundListener.h"

#include "GameObject.h"


REGISTER_FACTORY(SoundManager);

SoundManager::SoundManager(GameObject* gameObject) : UserComponent(gameObject)
{

}

SoundManager::~SoundManager()
{

}

void SoundManager::start()
{
	ss = SoundSystem::GetInstance();
	emitter = new SoundEmitter(gameObject);
	lis = gameObject->getComponent<SoundListener>();
	
}

void SoundManager::update(float deltaTime)
{
	
}

void SoundManager::playSound(const std::string& name)
{
	//EmitterData* aux = ss->createEmitter(&gameObject->transform->getPosition());
	
}

void SoundManager::playMusic(const std::string& name)
{
	emitter->playMusic(name);
	
}

void SoundManager::stopSound(const std::string& name)
{
	//Sound* sound = ResourcesManager::getSound(name);
	
}

void SoundManager::setPauseAllSounds(bool pause)
{
	ss->setPauseAllSounds(pause);
}

void SoundManager::setMusicVolume(float volume)
{
	ss->setMusicVolume(volume);
}

void SoundManager::setSoundEffectsVolume(float volume)
{
	ss->setSoundEffectsVolume(volume);
}

void SoundManager::setGeneralVolume(float volume)
{
	ss->setGeneralVolume(volume);
}

void SoundManager::setListenerAttributes(const Vector3& position, const Vector3& forward, const Vector3& up)
{
	ss->setListenerAttributes(position, forward, up);
}

float SoundManager::getGeneralVolume()
{
	return ss->getGeneralVolume();
}

float SoundManager::getMusicVolume()
{
	return ss->getMusicVolume();
}

float SoundManager::getSoundVolume()
{
	return ss->getSoundVolume();
}
