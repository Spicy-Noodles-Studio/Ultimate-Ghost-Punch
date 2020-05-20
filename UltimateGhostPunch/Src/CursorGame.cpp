#include "CursorGame.h"
#include <InputSystem.h>
#include <InterfaceSystem.h>
#include <GameObject.h>
#include <Cursor.h>

#include "GameManager.h"

#include <ComponentRegister.h>

REGISTER_FACTORY(CursorGame);

CursorGame::CursorGame(GameObject* gameObject) : UserComponent(gameObject), inputSystem(nullptr), cursor(nullptr), gameManager(nullptr)
{

}

CursorGame::~CursorGame()
{

}

void CursorGame::start()
{
	inputSystem = InputSystem::GetInstance();
	interfaceSystem = InterfaceSystem::GetInstance();
	cursor = gameObject->getComponent<Cursor>();
	gameManager = GameManager::GetInstance();

	if (cursor == nullptr) {
		LOG("Cursor not found");
		return;
	}
	cursor->setVisibleOnWindow(false);
	hideCursor();
	
	if (interfaceSystem == nullptr) return;
	interfaceSystem->setControllerNavigation(false);
}

void CursorGame::preUpdate(float deltaTime)
{
	if (isPaused()) {
		if(interfaceSystem != nullptr && !interfaceSystem->isControllerNavigationEnabled())
			interfaceSystem->setControllerNavigation(true);

		if (mouseUsed()) showCursor();
		else if (controllerUsed() || keyboardUsed()) hideCursor();
	}
	else {
		if(interfaceSystem != nullptr && interfaceSystem->isControllerNavigationEnabled())
			interfaceSystem->setControllerNavigation(false);

		if (usingMouse()) 
			showCursor();
		else 
			hideCursor();
	}
}

bool CursorGame::isPaused() const
{
	if (gameManager == nullptr) return false;
	return gameManager->isPaused();
}

bool CursorGame::mouseUsed() const
{
	if (inputSystem == nullptr) return false;

	return inputSystem->isMouseUsed();
}

bool CursorGame::keyboardUsed() const
{
	if (inputSystem == nullptr) return false;

	return inputSystem->isKeyboardUsed();
}

bool CursorGame::controllerUsed() const
{
	if (inputSystem == nullptr) return false;

	return inputSystem->isControllerUsed();
}

bool CursorGame::usingMouse() const
{
	if (gameManager == nullptr) return false;
	return gameManager->playerUsingKeyboard();
}

void CursorGame::hideCursor()
{
	if (cursor == nullptr) return;
	cursor->setSpriteVisible(false);
}

void CursorGame::showCursor()
{
	if (cursor == nullptr) return;
	cursor->setSpriteVisible(true);
}
