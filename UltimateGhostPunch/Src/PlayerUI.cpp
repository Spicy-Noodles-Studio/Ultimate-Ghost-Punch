#include "PlayerUI.h"

#include <ComponentRegister.h>
#include <GameObject.h>
#include <Camera.h>
#include <UILayout.h>

#include "PlayerIndex.h"
#include "GhostManager.h"
#include "Health.h"

REGISTER_FACTORY(PlayerUI);

PlayerUI::PlayerUI(GameObject* gameObject) : UserComponent(gameObject), playerHUD(nullptr), playerIndicator(nullptr),
health(nullptr), ghostManager(nullptr), mainCamera(nullptr), name(""), isVisible(false)
{

}

PlayerUI::~PlayerUI()
{

}

void PlayerUI::start()
{
	// Initialize name to search through layout
	PlayerIndex* playerIndex = gameObject->getComponent<PlayerIndex>();
	if (notNull(playerIndex))
		name = "Player" + std::to_string(playerIndex->getIndex());

	// Get health component to update stats
	health = gameObject->getComponent<Health>();
	checkNull(health);

	//Get ghostManager component to update status
	ghostManager = gameObject->getComponent<GhostManager>();
	checkNull(ghostManager);

	// Get camera
	GameObject* cameraObject = findGameObjectWithName("MainCamera");
	UILayout* cameraLayout = nullptr;

	if (notNull(cameraObject))
	{
		mainCamera = cameraObject->getComponent<Camera>();
		cameraLayout = cameraObject->getComponent<UILayout>();
	}

	// Get UI elements for PlayerIndicator and PlayerStatsPanel
	if (notNull(cameraLayout))
	{
		playerHUD = cameraLayout->getRoot().getChild(name + "Background");
		playerIndicator = cameraLayout->getRoot().getChild(name + "Indicator");
	}

	checkNull(mainCamera);

	playerHUD.setVisible(true);
	playerIndicator.setVisible(true);

	//Initialize layout aspect
	createHearts();
	createGhost();

	isVisible = true;
}

void PlayerUI::update(float deltaTime)
{
	updateHearts();
	updateGhost();
	updateIndicator();
}

void PlayerUI::createHearts()
{
	checkNullAndBreak(health);

	float posX = 0.05f;
	float posY = 0.65f;
	float heartOffset = 0.15f;

	for (int i = 1; i <= health->getHealth(); i++)
	{
		// Create heart
		UIElement heart = playerHUD.createChild("TaharezLook/StaticImage", name + "Heart" + std::to_string(i));
		heart.setProperty("Disabled", "true");
		heart.setProperty("Image", "heart");
		heart.setProperty("FrameEnabled", "false");
		heart.setProperty("BackgroundEnabled", "false");

		// Set position
		heart.setPosition(posX, posY);
		heart.setSize(0.15f, 0.25f);

		if (i % 2 != 0)
			heart.flipHorizontal();
		else
			posX += heartOffset;
	}
}

void PlayerUI::updateHearts()
{
	checkNullAndBreak(health);

	for (int i = 1; i <= health->getMaxHealth(); i++)
		playerHUD.getChild(name + "Heart" + std::to_string(i)).setVisible(i <= health->getHealth());
}

void PlayerUI::createGhost()
{
	// Create ghost
	UIElement ghost = playerHUD.createChild("TaharezLook/StaticImage", name + "Ghost");
	ghost.setProperty("Disabled", "true");
	ghost.setProperty("Image", "ghost");
	ghost.setProperty("FrameEnabled", "false");
	ghost.setProperty("BackgroundEnabled", "false");

	// Set position
	ghost.setPosition(0.45f, 0.05f);
	ghost.setSize(0.2f, 0.5f);
}

void PlayerUI::updateGhost()
{
	if (notNull(ghostManager) && ghostManager->ghostUsed() && playerHUD.getChild(name + "Ghost").isVisible())
		playerHUD.getChild(name + "Ghost").setVisible(false);
}

void PlayerUI::updateIndicator()
{
	checkNullAndBreak(mainCamera);
	checkNullAndBreak(gameObject->transform);

	Vector3 pos = mainCamera->worldToScreen(gameObject->transform->getPosition());

	if (notNull(ghostManager) && ghostManager->isGhost())
		playerIndicator.setPosition((float)pos.x - 0.025f, (float)pos.y - 0.25f);
	else
		playerIndicator.setPosition((float)pos.x - 0.025f, (float)pos.y - 0.15f);
}

void PlayerUI::setVisible(bool visible)
{
	if (visible == isVisible)
		return;

	isVisible = visible;
	playerIndicator.setVisible(visible);
}