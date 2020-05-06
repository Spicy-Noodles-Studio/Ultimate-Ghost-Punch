#pragma once
#ifndef PLAYER_INDEX_H
#define PLAYER_INDEX_H

#include <UserComponent.h>

class PlayerIndex : public UserComponent
{
private:
	int index;

public:
	PlayerIndex(GameObject* gameObject);
	virtual ~PlayerIndex();

	virtual void handleData(ComponentData* data);

	int getIndex();
	void setIndex(int ind);
};

#endif