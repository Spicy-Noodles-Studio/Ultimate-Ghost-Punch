#pragma once
#ifndef ATTACK_H
#define ATTACK_H

#include <UserComponent.h>

class RigidBody;

class Attack : public UserComponent
{
private:
	float cooldown = 0.0f;
	float quickAttackCooldown = 0.50f;
	float strongAttackCooldown = 2.00f;

	float quickChargeTime = 0.5f;
	float strongChargeTime = 0.75f;
	float chargeTime = 0.0f;

	int quickAttackDamage = 1;
	int strongAttackDamage = 2;

	float attackDuration = 0.5f; // The time that the attack remains active
	float activeTime = 0.0f;

	enum AttackType
	{
		QUICK, STRONG, NONE
	};
	enum AttackState {
		NOT_ATTACKING, CHARGING, ATTACKING
	};

	AttackType currentAttack = NONE;
	AttackState state = NOT_ATTACKING;

	RigidBody* attackTrigger;

	void charge(float newCooldown, float newChargeTime);
	void attack();
public:
	Attack(GameObject* gameObject);

	virtual void start();
	virtual void update(float deltaTime);
	virtual void onObjectStay(GameObject* other);

	virtual void handleData(ComponentData* data);

	void quickAttack();
	void strongAttack();
};

#endif