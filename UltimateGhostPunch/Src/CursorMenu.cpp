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

	if (cursor == nullptr) {
		LOG("Cursor not found");
		return;
	}
	cursor->setVisibleOnWindow(false);
}

void CursorMenu::preUpdate(float deltaTime)
{
	if (mouseUsed()) showCursor();
	else if (controllerUsed() || keyboardUsed()) hideCursor();
}

bool CursorMenu::mouseUsed() const
{
	if (inputSystem == nullptr) return false;

	return inputSystem->isMouseUsed();
}

bool CursorMenu::keyboardUsed() const
{
	if (inputSystem == nullptr) return false;

	return inputSystem->isKeyboardUsed();
}

bool CursorMenu::controllerUsed() const
{
	if (inputSystem == nullptr) return false;

	return inputSystem->isControllerUsed();
}

void CursorMenu::hideCursor()
{
	if (cursor == nullptr) return;
	cursor->setSpriteVisible(false);
}

void CursorMenu::showCursor()
{
	if (cursor == nullptr) return;
	cursor->setSpriteVisible(true);
}
