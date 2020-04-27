#include "SoundManager.h"

#include <ComponentRegister.h>

#include <ResourcesManager.h>


REGISTER_FACTORY(SoundManager);

SoundManager::SoundManager(GameObject* gameObject) : UserComponent(gameObject)
{

}

SoundManager::~SoundManager()
{

}

void SoundManager::start()
{
}

void SoundManager::update(float deltaTime)
{
	
}


