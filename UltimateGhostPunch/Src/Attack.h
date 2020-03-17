#pragma once

#ifndef ATTACK_H
#define ATTACK_H

#include <UserComponent.h>
#include <GameObject.h>


class Attack : public UserComponent
{
private:
	float cooldown = 0.0f;
	float quickAttackCooldown = 0.50f;
	float strongAttackCooldown = 2.00f;

	float quickAttackDamage = 1.0f;
	float strongAttackDamage = 2.0f;

	float attackDuration = 0.5f; // The time that the attack remains active
	float activeTime = 0.0f;
	
	enum AttackType
	{
		NOT_ATTACKING, QUICK, STRONG
	};

	AttackType currentAttack = NOT_ATTACKING;

	void attack(float newCooldown);
public:
	Attack(GameObject* g);


	virtual void start();
	virtual void update(float deltaTime);
	virtual void onTriggerEnter(GameObject* other);

	void quickAttack();
	void strongAttack();
};

#endif 