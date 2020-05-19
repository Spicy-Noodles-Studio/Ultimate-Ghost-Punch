#pragma once
#ifndef CURSOR_MENU_H
#define CURSOR_MENU_H

#include <UserComponent.h>

class InputSystem;
class Cursor;

class CursorMenu : public UserComponent
{
public:
	CursorMenu(GameObject* gameObject);
	virtual ~CursorMenu();

	void start();
	void preUpdate(float deltaTime);

private:
	bool mouseUsed() const;
	bool keyboardUsed() const;
	bool controllerUsed() const;

	void hideCursor();
	void showCursor();

private:
	InputSystem* inputSystem;
	Cursor* cursor;
};

#endif