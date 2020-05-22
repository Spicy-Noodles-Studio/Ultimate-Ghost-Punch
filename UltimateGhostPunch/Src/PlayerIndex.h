#pragma once
#ifndef PLAYER_INDEX_H
#define PLAYER_INDEX_H

#include <UserComponent.h>

class PlayerIndex : public UserComponent
{
private:
	int index;

protected:
	virtual void handleData(ComponentData* data);

public:
	PlayerIndex(GameObject* gameObject);
	virtual ~PlayerIndex();

	int getIndex();
	void setIndex(int ind);
};

#endif