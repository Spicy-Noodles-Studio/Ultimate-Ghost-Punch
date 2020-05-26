#pragma once
#ifndef LOADING_SCREEN_H
#define LOADING_SCREEN_H

#include <UserComponent.h>
#include <UIElement.h>

#include <vector>

class InputSystem;
class UILayout;

class LoadingScreen : public UserComponent
{
private:
	std::string sceneToLoad;
	std::vector<std::string> tipsVector;

	float loadDelay;
	float currentDelay;

	std::string getRandomTip();

protected:
	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

public:
	LoadingScreen(GameObject* gameObject);
	virtual ~LoadingScreen();
};

#endif