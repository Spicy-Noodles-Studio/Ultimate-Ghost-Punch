#pragma once
#ifndef ATTACK_H
#define ATTACK_H

#include <UserComponent.h>

class RigidBody;
class Score;

class Attack : public UserComponent
{
private:
	int id;

	float cooldown;
	float quickAttackCooldown;
	float strongAttackCooldown;

	float chargeTime;
	float quickChargeTime;
	float strongChargeTime;

	int quickAttackDamage;
	int strongAttackDamage;

	float attackDuration; // The time that the attack remains active
	float activeTime;

	bool hit;

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
	Score* score;

	Vector3 quickAttackScale;
	Vector3 strongAttackScale;
	Vector3 quickAttackOffset;
	Vector3 strongAttackOffset;

	GameObject* parent;

	void charge(float newCooldown, float newChargeTime);
	void attack();

	void setUpTriggerAttack(const Vector3& scale, const Vector3& offset);

public:
	Attack(GameObject* gameObject);
	virtual ~Attack();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void postUpdate(float deltaTime);
	virtual void handleData(ComponentData* data);
	virtual void onObjectStay(GameObject* other);

	void quickAttack();
	void strongAttack();

	bool isAttacking() const;
	bool isHeavyAttacking() const;
	bool isQuickAttacking() const;

	bool hasHit() const;
};

#endif