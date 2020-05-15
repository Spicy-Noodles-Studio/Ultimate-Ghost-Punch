#pragma once
#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <UserComponent.h>
#include <UIElement.h>

#include <chrono>

class UILayout;
class CameraController;

class Countdown : public UserComponent
{
private:
	UIElement panel;

	std::vector<GameObject*> players;
	CameraController* cameraControl;

	float time;
	std::chrono::steady_clock::time_point last;

	bool startCounting;
	bool countingDown;

public:
	Countdown(GameObject* gameObject);
	virtual ~Countdown();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

	bool isCounting() const;
};

#endif