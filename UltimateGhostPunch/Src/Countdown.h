#pragma once
#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <UserComponent.h>
#include <UIElement.h>

#include <chrono>

class UILayout;
class SoundEmitter;

class Countdown : public UserComponent
{
private:
	void startGame();
	void pauseGame();

	float time;

	bool started;
	bool charged;
	bool paused;

	std::chrono::steady_clock::time_point last;

	UIElement text;
	SoundEmitter* soundEmitter;

public:
	Countdown(GameObject* gameObject);
	virtual ~Countdown();

	virtual void start();
	virtual void preUpdate(float deltaTime);
	virtual void handleData(ComponentData* data);
};

#endif