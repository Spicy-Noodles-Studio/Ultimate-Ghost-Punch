#pragma once
#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <UserComponent.h>
#include <UIElement.h>

class UILayout;
class CameraController;

class Countdown : public UserComponent
{
private:
	UIElement text;

	std::vector<GameObject*> players;
	CameraController* cameraControl;

	float time;

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