#include "LoadingScreen.h"

#include <ComponentRegister.h>
#include <InterfaceSystem.h>
#include <SceneManager.h>
#include <WindowManager.h>
#include <GameObject.h>

REGISTER_FACTORY(LoadingScreen);

LoadingScreen::LoadingScreen(GameObject* gameObject) : UserComponent(gameObject)
{
}

LoadingScreen::~LoadingScreen()
{
}

void LoadingScreen::start()
{
	logo = instantiate("Cubo", { 10, 0, 0 });
}

void LoadingScreen::update(float deltaTime)
{
	if (logo) logo->transform->rotate({ 0,10,0 });
}

void LoadingScreen::handleData(ComponentData* data)
{
}
