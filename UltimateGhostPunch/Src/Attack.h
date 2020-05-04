#pragma once
#ifndef ATTACK_H
#define ATTACK_H

#include <UserComponent.h>

class RigidBody;
class Score;
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

	Vector3 quickAttackScale;
	Vector3 strongAttackScale;
	Vector3 quickAttackOffset;
	Vector3 strongAttackOffset;

	void charge(float newCooldown, float newChargeTime);
	void attack();

	void setUpTriggerAttack(const Vector3& scale, const Vector3& offset);

	int id;
	Score* score;

public:
	Attack(GameObject* gameObject);
	virtual ~Attack();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);
	virtual void onObjectStay(GameObject* other);

	bool quickAttack();
	bool strongAttack();
	bool isAttacking() const;
};

#endif