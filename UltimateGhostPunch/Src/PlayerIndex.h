#pragma once
#ifndef PLAYER_INDEX_H
#define PLAYER_INDEX_H

#include <UserComponent.h>

class PlayerIndex : public UserComponent
{
private:
	int index;
	int pos;

protected:
	virtual void handleData(ComponentData* data);

public:
	PlayerIndex(GameObject* gameObject);
	virtual ~PlayerIndex();

	int getIndex();
	void setIndex(int index);

	int getPos();
	void setPos(int pos);
};

#endif