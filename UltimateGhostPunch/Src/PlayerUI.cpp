#include "PlayerUI.h"
#include "GameObject.h"

#include "UILayout.h"
#include "Health.h"

#include "Scene.h"
#include "Camera.h"

PlayerUI::PlayerUI(GameObject* gameObject) : UserComponent(gameObject), playerHUD(nullptr), playerIndicator(nullptr), pauseMenu(nullptr)
{

}

PlayerUI::~PlayerUI()
{

}

void PlayerUI::start()
{
	health = gameObject->getComponent<Health>();

	playerHUD = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild(gameObject->getName() + "Background");

	playerIndicator = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild(gameObject->getName() + "Indicator");

	pauseMenu = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("PauseBackground");

	playerHUD.setVisible(true);
	playerIndicator.setVisible(true);

	for (int i = 0; i < playerHUD.getChildCount(); i++)
		playerHUD.getChildAtIndex(i).setInheritsAlpha(false);

	createHearts();
	updateHealth();
}

void PlayerUI::createHearts()
{
	float posX = 0.3f;

	for (int i = 1; i <= gameObject->getComponent<Health>()->getHealth(); i++)
	{
		UIElement heart = playerHUD.createChild("TaharezLook/StaticImage",
			gameObject->getName() + "Heart" + std::to_string(i));

		heart.setPosition(posX, 0.1f);
		heart.setSize(0.05f, 0.2f);
		if (i % 2 != 0)
			heart.flipHorizontal();
		else
			posX += 0.03f;

		posX += 0.02f;
	}
}

void PlayerUI::updateIndicator()
{
	Vector3 pos = gameObject->getScene()->getMainCamera()->worldToScreen(gameObject->getComponent<Transform>()->getPosition());
	playerIndicator.setPosition((float)pos.x - 0.005f, (float)pos.y - 0.4f);
}

void PlayerUI::update(float deltaTime)
{
	updateIndicator();
}

void PlayerUI::updateState(const std::string state)
{
	playerHUD.getChild(gameObject->getName() + "StateText").setText("State: " + state);
}

void PlayerUI::updateHealth()
{
	playerHUD.getChild(gameObject->getName() + "HealthText").setText("Health: " + std::to_string(health->getHealth()));

	updateHearts();
}

void PlayerUI::setPauseMenuVisible(bool show)
{
	pauseMenu.setVisible(show);
}

bool PlayerUI::isPauseMenuVisible()
{
	return pauseMenu.isVisible();
}

void PlayerUI::updateHearts()
{
	for (int i = 1; i <= health->getMaxHealth(); i++)
	{
		if (i > health->getHealth())
			playerHUD.getChild(gameObject->getName() + "Heart" + std::to_string(i)).setVisible(false);
		else
			playerHUD.getChild(gameObject->getName() + "Heart" + std::to_string(i)).setVisible(true);
	}
}