#pragma once
#ifndef ATTACK_H
#define ATTACK_H

#include <UserComponent.h>

class RigidBody;

class Attack : public UserComponent
{
private:
	float cooldown;
	float quickAttackCooldown;
	float strongAttackCooldown;

	float quickChargeTime;
	float strongChargeTime;
	float chargeTime;

	int quickAttackDamage;
	int strongAttackDamage;

	float attackDuration; // The time that the attack remains active
	float activeTime;

	enum AttackType
	{
		QUICK, STRONG, NONE
	};

	enum AttackState
	{
		NOT_ATTACKING, CHARGING, ATTACKING
	};

	AttackType currentAttack;
	AttackState state;

	RigidBody* attackTrigger;

	void charge(float newCooldown, float newChargeTime);
	void attack();

public:
	Attack(GameObject* gameObject);
	virtual ~Attack();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);
	virtual void onObjectStay(GameObject* other);

	bool quickAttack();
	bool strongAttack();
};

#endif