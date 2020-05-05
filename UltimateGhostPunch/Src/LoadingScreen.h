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
	float loadDelay;

	std::vector<std::string> tipsVector;

private:
	std::string getRandomTip();
public:
	virtual void start();
	virtual void update(float deltaTime);

	LoadingScreen(GameObject* gameObject);
	virtual ~LoadingScreen();

	virtual void handleData(ComponentData* data);
};

#endif