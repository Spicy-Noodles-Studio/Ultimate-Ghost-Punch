#pragma once
#ifndef CURSOR_GAME_H
#define CURSOR_GAME_H

#include <UserComponent.h>

class InputSystem;
class InterfaceSystem;
class Cursor;
class GameManager;

class CursorGame : public UserComponent
{
public:
	CursorGame(GameObject* gameObject);
	virtual ~CursorGame();

	void start();
	void preUpdate(float deltaTime);

private:
	bool isPaused() const;

	bool mouseUsed() const;
	bool keyboardUsed() const;
	bool controllerUsed() const;

	bool usingMouse() const;

	void hideCursor();
	void showCursor();

private:
	InputSystem* inputSystem;
	InterfaceSystem* interfaceSystem;
	Cursor* cursor;
	GameManager* gameManager;

};
#endif
