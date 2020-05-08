#include "Countdown.h"
#include <ComponentRegister.h>
#include <InterfaceSystem.h>
#include <RenderSystem.h>
#include <SceneManager.h>
#include <WindowManager.h>
#include <GameObject.h>
#include <UILayout.h>

#include "PlayerController.h"
#include "CameraController.h"
#include "GameManager.h"
#include "DebugUtils.h"

REGISTER_FACTORY(Countdown);

Countdown::Countdown(GameObject* gameObject) : UserComponent(gameObject), text(NULL), players(), cameraControl(nullptr), time(0), startCounting(false), countingDown(false)
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


	cameraControl = findGameObjectWithName("MainCamera")->getComponent<CameraController>();
	players = GameManager::GetInstance()->getKnights();
}

void Countdown::update(float deltaTime)
{
	if (!startCounting)
	{
		for (int i = 0; i < players.size(); i++)
			players[i]->getComponent<PlayerController>()->setActive(false);

		cameraControl->setActive(false);

		startCounting = true;
		countingDown = true;
	}

	if (countingDown)
	{
		time -= deltaTime;

		if (time >= 1)
			text.setText(std::to_string((int)time));
		else
			text.setText("FIGHT!");

		if (time <= 0)
		{
			text.setText("");

			for (int i = 0; i < players.size(); i++)
				players[i]->getComponent<PlayerController>()->setActive(true);

			cameraControl->setActive(true);

			countingDown = false;
		}
	}
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

bool Countdown::isCounting() const
{
	return countingDown;
}