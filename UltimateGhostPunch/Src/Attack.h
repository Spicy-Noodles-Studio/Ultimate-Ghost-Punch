#pragma once
#ifndef ATTACK_H
#define ATTACK_H

#include <UserComponent.h>
#include <GameObject.h>

class Attack : public UserComponent
{
public:
	Attack(GameObject* gameObject);
};

#endif