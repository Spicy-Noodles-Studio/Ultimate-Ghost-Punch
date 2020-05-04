#include "LoadingScreen.h"

#include <ComponentRegister.h>
#include <InterfaceSystem.h>
#include <SceneManager.h>
#include <WindowManager.h>
#include <GameObject.h>
#include <RigidBody.h>
#include "GameManager.h"

REGISTER_FACTORY(LoadingScreen);

LoadingScreen::LoadingScreen(GameObject* gameObject) : UserComponent(gameObject)
{
}

LoadingScreen::~LoadingScreen()
{
}

void LoadingScreen::start()
{
	logo = instantiate("Cubo", { 0, 10, 0 });
	logo->getComponent<RigidBody>()->setGravity({ 0,0,0 });
	LOG("Nivel: %s", GameManager::GetInstance()->getLevel().c_str());
}

void LoadingScreen::update(float deltaTime)
{
	if (logo) logo->transform->rotate({ 0,0.1f,0 });
}

void LoadingScreen::handleData(ComponentData* data)
{
}
