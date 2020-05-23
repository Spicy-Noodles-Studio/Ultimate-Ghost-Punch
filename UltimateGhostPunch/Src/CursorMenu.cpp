#include "CursorMenu.h"
#include <InputSystem.h>
#include <GameObject.h>
#include <Cursor.h>

#include <ComponentRegister.h>

REGISTER_FACTORY(CursorMenu);

CursorMenu::CursorMenu(GameObject* gameObject) : UserComponent(gameObject), inputSystem(nullptr), cursor(nullptr)
{

}

CursorMenu::~CursorMenu()
{
}

void CursorMenu::start()
{
	inputSystem = InputSystem::GetInstance();
	cursor = gameObject->getComponent<Cursor>();

	checkNull(inputSystem);
	checkNullAndBreak(cursor);
	cursor->setVisibleOnWindow(false);
}

void CursorMenu::preUpdate(float deltaTime)
{
	if (mouseUsed()) showCursor();
	else if (controllerUsed() || keyboardUsed()) hideCursor();
}

bool CursorMenu::mouseUsed() const
{
	checkNullAndBreak(inputSystem, false);

	return inputSystem->isMouseUsed();
}

bool CursorMenu::keyboardUsed() const
{
	checkNullAndBreak(inputSystem, false);

	return inputSystem->isKeyboardUsed();
}

bool CursorMenu::controllerUsed() const
{
	checkNullAndBreak(inputSystem, false);

	return inputSystem->isControllerUsed();
}

void CursorMenu::hideCursor()
{
	checkNullAndBreak(cursor);
	cursor->setSpriteVisible(false);
}

void CursorMenu::showCursor()
{
	checkNullAndBreak(cursor);
	cursor->setSpriteVisible(true);
}
