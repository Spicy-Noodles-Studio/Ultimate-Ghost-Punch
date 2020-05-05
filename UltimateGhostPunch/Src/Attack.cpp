#include "Attack.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>

#include "Health.h"
#include "Block.h"
#include "ComponentRegister.h"
#include "GameManager.h"
#include "Score.h"
#include "PlayerIndex.h"

REGISTER_FACTORY(Attack);

Attack::Attack(GameObject* gameObject) : UserComponent(gameObject), attackTrigger(nullptr), currentAttack(AttackType::NONE), state(AttackState::NOT_ATTACKING), activeTime(0.0f), attackDuration(0.5f),
										 strongAttackDamage(2), quickAttackDamage(1), chargeTime(0), strongChargeTime(0.75f), quickChargeTime(0.5f), strongAttackCooldown(2.0f),
										 quickAttackCooldown(0.5f), cooldown(0.0f), quickAttackScale(1.0f), strongAttackScale(1.0f), offset(0.0f), id(0), score(nullptr)
{

}

Attack::~Attack()
{

}

void Attack::start()
{
	attackTrigger = gameObject->getComponent<RigidBody>();
	id = gameObject->getParent()->getComponent<PlayerIndex>()->getIndex();
	score = GameManager::GetInstance()->getScore();
	// Deactivate the trigger until the attack is used
	if (attackTrigger != nullptr) attackTrigger->setActive(false);
	offset = gameObject->transform->getPosition().z;
}

void Attack::update(float deltaTime)
{
	// Update the cooldown
	if (cooldown > 0.0f)
		cooldown -= deltaTime;

	//Attack charge time
	if (chargeTime > 0.0f)
		chargeTime -= deltaTime;
	else if (state == CHARGING)
		attack();

	// Attack active time
	if (activeTime > 0.0f)
		activeTime -= deltaTime;
	else if (state == ATTACKING)
	{
		// Deactivate the trigger until the next attack is used
		attackTrigger->setActive(false);

		// Reset the current attack state
		state = NOT_ATTACKING;
	}
}

void Attack::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "quickCooldown") {
			setFloat(quickAttackCooldown);
		}
		else if (prop.first == "strongCooldown") {
			setFloat(strongAttackCooldown);
		}
		else if (prop.first == "quickDamage") {
			setInt(quickAttackDamage);
		}
		else if (prop.first == "strongDamage") {
			setInt(strongAttackDamage);
		}
		else if (prop.first == "attackDuration") {
			setFloat(attackDuration);
		}
		else if (prop.first == "quickCharge") {
			setFloat(quickChargeTime);
		}
		else if (prop.first == "strongCharge") {
			setFloat(strongChargeTime);
		}
		else if (prop.first == "quickAttackScale") {
			setFloat(quickAttackScale);
		}
		else if (prop.first == "strongAttackScale") {
			setFloat(strongAttackScale);
		}
		else
			LOG("ATTACK: Invalid property name \"%s\"", prop.first.c_str());
	}
}


void Attack::onObjectStay(GameObject* other)
{
	if (other->getTag() == "Player" && other != gameObject->getParent() && state == ATTACKING)//If it hits a player different than myself
	{
		LOG("You hit player %s!\n", other->getName().c_str());
		float damage = 0;

		switch (currentAttack)
		{
		case Attack::QUICK:
			damage = quickAttackDamage;
			break;
		case Attack::STRONG:
			damage = strongAttackDamage;
			break;
		}

		std::vector<GameObject*> aux = other->findChildrenWithTag("groundSensor");
		Block* enemyBlock = nullptr;
		PlayerIndex* otherIndex = other->getComponent<PlayerIndex>();
		if (aux.size() > 0) enemyBlock = aux[0]->getComponent<Block>();
		if (enemyBlock != nullptr) {
			if(!enemyBlock->blockAttack(damage, gameObject->getParent()->transform->getPosition()));
			{
				Health* enemyHealth = other->getComponent<Health>();
				score->receiveHitFrom(otherIndex->getIndex(),id );
				score->damageRecivedFrom(otherIndex->getIndex(),id, damage);
				if (!enemyHealth->isAlive())
				{
					score->killedBy(otherIndex->getIndex(), id);
				}
			}
			
			// Deactivate the trigger until the next attack is used
			attackTrigger->setActive(false);

			// Reset the current attack state
			state = NOT_ATTACKING;

		}
		/* PARECE CODIGO MUERTO (no lo he comprobado) */
		else {
			Health* enemyHealth = other->getComponent<Health>();
			int health = enemyHealth->getHealth();
			if (enemyHealth != nullptr) enemyHealth->receiveDamage(damage);
			score->receiveHitFrom(otherIndex->getIndex(),id);
			if(health!= enemyHealth->getHealth())
				score->damageRecivedFrom(otherIndex->getIndex(), id, damage);
			if (!enemyHealth->isAlive())
			{
				score->killedBy(otherIndex->getIndex(),id);
			}
		}
	}
}

void Attack::charge(float newCooldown, float newChargeTime)
{
	state = CHARGING;
	chargeTime = newChargeTime;
	cooldown = newCooldown + chargeTime;
	LOG("Attack charging...\n");
}

void Attack::attack()
{
	state = ATTACKING;
	activeTime = attackDuration;
	attackTrigger->setActive(true);
	score->attackDone(id, false);
	LOG("Attack!\n");
}

void Attack::setUpTriggerAttack(float scale)
{
	Transform* attackTransform = attackTrigger->gameObject->transform;
	// Scale trigger
	Vector3 scaleRatio = Vector3::IDENTITY;
	Vector3 currentScale = attackTransform->getScale();
	scaleRatio.z = scale;
	attackTrigger->multiplyScale(scaleRatio);
	scaleRatio *= currentScale;

	float diff = offset * scaleRatio.z / currentScale.z;

	// Move an offset
	Vector3 position = attackTransform->getPosition();
	position.z = diff;
	attackTransform->setPosition(position);
}

bool Attack::quickAttack()
{
	if (cooldown <= 0.0f)
	{
		currentAttack = QUICK;
		setUpTriggerAttack(quickAttackScale);
		charge(quickAttackCooldown, quickChargeTime);
		return true;
	}
	else
		LOG("Attack on CD...\n");
	return false;
}

bool Attack::strongAttack()
{
	if (cooldown <= 0.0f)
	{
		currentAttack = STRONG;
		setUpTriggerAttack(strongAttackScale);
		charge(strongAttackCooldown, strongChargeTime);
		return true;
	}
	else
		LOG("Attack on CD...\n");
	return false;
}

bool Attack::isAttacking() const
{
	return state == AttackState::CHARGING;
}
