#include "Countdown.h"
#include <InterfaceSystem.h>
#include <RenderSystem.h>
#include <SceneManager.h>
#include <WindowManager.h>
#include <GameObject.h>

#include "GameManager.h"

#include <ComponentRegister.h>

#include <ctime>

#include "DebugUtils.h"

REGISTER_FACTORY(Countdown);


Countdown::Countdown(GameObject* gameObject) : UserComponent(gameObject)
{

}

Countdown::~Countdown()
{
	
}

void Countdown::start()
{
	started = false;
	charged = false;
	paused = false;
	time = 5;
}

void Countdown::preUpdate(float deltaTime)
{
	if (charged && !started) {
		pauseGame();
		started = true;
		paused = true;
		last = std::chrono::steady_clock::now();
	}
	if (paused) {
		

		std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(current - last);
		deltaTime = elapsed.count();
		last = current;

		time -= deltaTime;

		if (time <= 0) { 
			startGame(); 
			paused = false;
		}
		
	}
	
	if (!charged) charged = true;

	
}

void Countdown::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "initTime") {
			setFloat(time);
		}else
			LOG("DODGE: Invalid property name %s", prop.first.c_str());
	}
}


void Countdown::pauseGame()
{
	GameManager::GetInstance()->pauseGame(true);
}

void Countdown::startGame()
{
	GameManager::GetInstance()->pauseGame(false);
}
