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

	checkNull(inputSystem);
	checkNull(gameManager);

	if (notNull(cursor)) {
		cursor->setVisibleOnWindow(false);
		hideCursor();
	}
	
	checkNullAndBreak(interfaceSystem);
	interfaceSystem->setControllerNavigation(false);
}

void CursorGame::preUpdate(float deltaTime)
{
	if (isPaused()) {
		if(notNull(interfaceSystem) && !interfaceSystem->isControllerNavigationEnabled())
			interfaceSystem->setControllerNavigation(true);

		if (mouseUsed()) showCursor();
		else if (controllerUsed() || keyboardUsed()) hideCursor();
	}
	else {
		if(notNull(interfaceSystem) && interfaceSystem->isControllerNavigationEnabled())
			interfaceSystem->setControllerNavigation(false);

		if (usingMouse()) 
			showCursor();
		else 
			hideCursor();
	}
}

bool CursorGame::isPaused() const
{
	checkNullAndBreak(gameManager, false);
	return gameManager->isPaused();
}

bool CursorGame::mouseUsed() const
{
	checkNullAndBreak(inputSystem, false);
	return inputSystem->isMouseUsed();
}

bool CursorGame::keyboardUsed() const
{
	checkNullAndBreak(inputSystem, false);

	return inputSystem->isKeyboardUsed();
}

bool CursorGame::controllerUsed() const
{
	checkNullAndBreak(inputSystem, false);

	return inputSystem->isControllerUsed();
}

bool CursorGame::usingMouse() const
{
	checkNullAndBreak(gameManager, false);
	return gameManager->playerUsingKeyboard();
}

void CursorGame::hideCursor()
{
	checkNullAndBreak(cursor);
	cursor->setSpriteVisible(false);
}

void CursorGame::showCursor()
{
	checkNullAndBreak(cursor);
	cursor->setSpriteVisible(true);
}
