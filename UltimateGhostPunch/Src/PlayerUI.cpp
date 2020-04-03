#include "PlayerUI.h"

#include <ComponentRegister.h>
#include <GameObject.h>
#include <Camera.h>
#include <UILayout.h>

#include "Health.h"
#include "PlayerController.h"

REGISTER_FACTORY(PlayerUI);

PlayerUI::PlayerUI(GameObject* gameObject) : UserComponent(gameObject), playerHUD(nullptr), playerIndicator(nullptr), health(nullptr)
{

}

PlayerUI::~PlayerUI()
{

}

void PlayerUI::start()
{
	// Initialize name to search through layout
	PlayerController* playerController = gameObject->getComponent<PlayerController>();
	if (playerController != nullptr)
		name = "Player" + std::to_string(playerController->getPlayerIndex());
	else
		LOG("PlayerController not found. Cannot get player index");

	// Get health component to update stats
	health = gameObject->getComponent<Health>();
	if (health == nullptr)
		LOG("Health component not found");

	// Get camera
	GameObject* cameraObject = findGameObjectWithName("MainCamera");
	if(cameraObject != nullptr)
		mainCamera = cameraObject->getComponent<Camera>();
	if (mainCamera == nullptr)
		LOG("Camera component not found");

	// Get player layout
	UILayout* cameraLayout = nullptr;
	if(cameraObject != nullptr)
		cameraLayout = cameraObject->getComponent<UILayout>();

	// Get UI elements for PlayerIndicator and PlayerStatsPanel
	if (cameraLayout != nullptr) {
		playerHUD = cameraLayout->getRoot().getChild(name + "Background");
		playerIndicator = cameraLayout->getRoot().getChild(name + "Indicator");
	}
	playerHUD.setVisible(true);
	playerIndicator.setVisible(true);

	//TODO: esto esta feo aqui (o eso creo)
	for (int i = 0; i < playerHUD.getChildCount(); i++)
		playerHUD.getChildAtIndex(i).setInheritsAlpha(false);

	//Initialize layout aspect
	createHearts();
	updateHealth();
}

void PlayerUI::createHearts()
{
	if (health == nullptr)
		return;
	std::string heartName = name + "Heart";
	float posX = 0.3f;
	float posY = 0.1f;
	float xOffset = 0.03f;
	float heartOffset = 0.02f;

	for (int i = 1; i <= health->getHealth(); i++) {
		// Create heart
		UIElement heart = playerHUD.createChild("TaharezLook/StaticImage", heartName + std::to_string(i));

		// Set position
		heart.setPosition(posX, posY);
		heart.setSize(0.05f, 0.2f);
		if (i % 2 != 0)
			heart.flipHorizontal();
		else
			posX += xOffset;

		posX += heartOffset;
	}
}

void PlayerUI::updateIndicator()
{
	if (mainCamera == nullptr)
		return;
	Vector3 pos = mainCamera->worldToScreen(gameObject->transform->getPosition());
	playerIndicator.setPosition((float)pos.x - 0.005f, (float)pos.y - 0.24f);
}

void PlayerUI::update(float deltaTime)
{
	updateIndicator();
}

void PlayerUI::updateState(const std::string state)
{
	playerHUD.getChild(name + "StateText").setText("State: " + state);
}

void PlayerUI::updateHealth()
{
	playerHUD.getChild(name + "HealthText").setText("Health: " + std::to_string(health->getHealth()));

	updateHearts();
}

/*void PlayerUI::setPauseMenuVisible(bool show)
{
	pauseMenu.setVisible(show);
}

bool PlayerUI::isPauseMenuVisible()
{
	return pauseMenu.isVisible();
}*/

void PlayerUI::updateHearts()
{
	for (int i = 1; i <= health->getMaxHealth(); i++)
		playerHUD.getChild(name + "Heart" + std::to_string(i)).setVisible(i <= health->getHealth());
}