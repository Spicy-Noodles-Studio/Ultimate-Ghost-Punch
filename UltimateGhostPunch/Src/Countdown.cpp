#include "Countdown.h"
#include <ComponentRegister.h>
#include <InterfaceSystem.h>
#include <RenderSystem.h>
#include <SceneManager.h>
#include <WindowManager.h>
#include <GameObject.h>
#include <UILayout.h>
#include <ctime>

#include "GameManager.h"
#include "DebugUtils.h"

REGISTER_FACTORY(Countdown);

Countdown::Countdown(GameObject* gameObject) : UserComponent(gameObject), text(NULL), time(0), started(false), charged(false), paused(false)
{

}

Countdown::~Countdown()
{
	
}

void Countdown::start()
{
	UILayout* cameraLayout = findGameObjectWithName("MainCamera")->getComponent<UILayout>();

	if (cameraLayout != nullptr)
		text = cameraLayout->getRoot().getChild("Countdown");
}

void Countdown::preUpdate(float deltaTime)
{
	if (charged && !started)
	{
		pauseGame();
		started = true;
		paused = true;
		last = std::chrono::steady_clock::now();
	}

	if (paused)
	{
		std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(current - last);

		deltaTime = elapsed.count();
		last = current;
		time -= deltaTime;

		if (time >= 1)
			text.setText(std::to_string((int)time));
		else
		{ 
			text.setPosition(0.3f, 0.25f);
			text.setText("FIGHT!"); 
		}

		if (time <= 0)
		{ 
			startGame(); 
			paused = false;
			text.setText("");
		}
	}
	
	if (!charged)
		charged = true;
}

void Countdown::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "time")
		{
			setFloat(time);
		}
		else
			LOG("COUNTDOWN: Invalid property name %s", prop.first.c_str());
	}
}

void Countdown::startGame()
{
	GameManager::GetInstance()->pauseGame(false);
}

void Countdown::pauseGame()
{
	GameManager::GetInstance()->pauseGame(true);
}