#include "PlayerUI.h"
#include "GameObject.h"

#include "UILayout.h"
#include "Health.h"
#include "Scene.h"
#include "Camera.h"

PlayerUI::PlayerUI(GameObject* gameObject) : UserComponent(gameObject)
{

}

PlayerUI::~PlayerUI()
{

}

void PlayerUI::start()
{
	createHearts();
	updateHealth();
}

void PlayerUI::createHearts()
{
	float posX = 0.4f;

	for (int i = 1; i <= gameObject->getComponent<Health>()->getHealth(); i++)
	{
		UIElement heart = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->
			getUIElement("StaticImage").getChild(gameObject->getName() + "Background").
			createChild("TaharezLook/StaticImage", gameObject->getName() + "Heart" + std::to_string(i));

		heart.setPosition(posX, 0.05f);
		heart.setSize(0.05f, 0.2f);

		if (i % 2 == 0)
			heart.flipHorizontal();

		posX += 0.05f;
	}
}

void PlayerUI::updateIndicator()
{
	Vector3 pos = gameObject->getScene()->getMainCamera()->worldToScreen(gameObject->getComponent<Transform>()->getPosition());

	findGameObjectWithName("MainCamera")->getComponent<UILayout>()->
		getUIElement("StaticImage").getChild(gameObject->getName() + "Indicator").setPosition((float)pos.x - 0.005f, (float)pos.y - 0.4f);
}

void PlayerUI::update(float deltaTime)
{
	updateIndicator();
}

void PlayerUI::updateState(const std::string state)
{
	findGameObjectWithName("MainCamera")->getComponent<UILayout>()->
		getUIElement("StaticImage").getChild(gameObject->getName() + "StateText").setText("State: " + state);
}

void PlayerUI::updateHealth()
{
	findGameObjectWithName("MainCamera")->getComponent<UILayout>()->
		getUIElement("StaticImage").getChild(gameObject->getName() + "HealthText").setText("Health: " +
			std::to_string(gameObject->getComponent<Health>()->getHealth()));

	updateHearts();
}

void PlayerUI::updateHearts()
{
	for (int i = gameObject->getComponent<Health>()->getHealth() + 1; i <= gameObject->getComponent<Health>()->getMaxHealth(); i++)
	{
		findGameObjectWithName("MainCamera")->getComponent<UILayout>()->
			getUIElement("StaticImage").getChild(gameObject->getName() + "Background").
			getChild(gameObject->getName() + "Heart" + std::to_string(i)).setVisible(false);
	}
}