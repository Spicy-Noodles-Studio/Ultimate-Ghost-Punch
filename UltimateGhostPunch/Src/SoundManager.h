#pragma once
#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <UserComponent.h>
#include <UIElement.h>
#include <Vector3.h>

#include <vector>
#include <string>


class SoundSystem;
class GameObject;
class SoundEmitter;
class SoundListener;

class SoundManager : public UserComponent
{

private:
	SoundSystem* ss;
	SoundEmitter* emitter;
	SoundListener* lis;


public:
	SoundManager(GameObject* gameObject);
	virtual ~SoundManager();

	virtual void start();
	virtual void update(float deltaTime);

	void playSound(const std::string& name);
	void playMusic(const std::string& name);

	void stopSound(const std::string& name);

	void setPauseAllSounds(bool pause);

	void setMusicVolume(float volume);
	void setSoundEffectsVolume(float volume);
	void setGeneralVolume(float volume);
	void setListenerAttributes(const Vector3& position, const Vector3& forward, const Vector3& up);

	float getGeneralVolume();
	float getMusicVolume();
	float getSoundVolume();



};

#endif