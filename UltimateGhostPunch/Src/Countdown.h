#pragma once
#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <UserComponent.h>
#include <UIElement.h>

class UILayout;

class Countdown : public UserComponent
{
private:
	
	void pauseGame();
	void startGame();

	float initTime;



public:
	Countdown(GameObject* gameObject);
	virtual ~Countdown();

	virtual void start();
	virtual void preUpdate(float deltaTime);
};

#endif