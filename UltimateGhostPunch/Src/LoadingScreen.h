#pragma once
#ifndef LOADING_SCREEN_H
#define LOADING_SCREEN_H

#include <UserComponent.h>
#include <UIElement.h>

class InputSystem;
class UILayout;

class LoadingScreen : public UserComponent
{
private:
	GameObject* logo;

public:
	virtual void start();
	virtual void update(float deltaTime);

	LoadingScreen(GameObject* gameObject);
	virtual ~LoadingScreen();

	virtual void handleData(ComponentData* data);
};

#endif