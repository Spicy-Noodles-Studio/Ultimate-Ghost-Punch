#include "Attack.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>

#include "PlayerIndex.h"
#include "PlayerState.h"
#include "Health.h"
#include "Block.h"
#include "Score.h"
#include "GameManager.h"

REGISTER_FACTORY(Attack);

Attack::Attack(GameObject* gameObject) : UserComponent(gameObject), currentAttack(AttackType::NONE), state(AttackState::NOT_ATTACKING), activeTime(0.0f), attackDuration(0.5f),
strongAttackDamage(2), quickAttackDamage(1), strongChargeTime(0.75f), quickChargeTime(0.5f), chargeTime(0), strongAttackCooldown(2.0f),
quickAttackCooldown(0.5f), cooldown(0.0f), quickAttackScale(Vector3::IDENTITY), strongAttackScale(Vector3::IDENTITY),
quickAttackOffset(Vector3::ZERO), strongAttackOffset(Vector3::ZERO), id(0), parent(nullptr), attackTrigger(nullptr), score(nullptr), hit(0)
{

}

Attack::~Attack()
{
	parent = nullptr;
	attackTrigger = nullptr;
	score = nullptr;
}

void Attack::start()
{
	checkNullAndBreak(gameObject);

	parent = gameObject->getParent();
	if (notNull(parent) && notNull(parent->getComponent<PlayerIndex>()))
		id = parent->getComponent<PlayerIndex>()->getIndex();

	attackTrigger = gameObject->getComponent<RigidBody>();
	if (notNull(GameManager::GetInstance()))
		score = GameManager::GetInstance()->getScore();

	checkNull(score);
	checkNullAndBreak(attackTrigger);

	// Deactivate the trigger until the attack is used
	attackTrigger->setActive(false);
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
		stop();
}

void Attack::postUpdate(float deltaTime)
{
	if (hit > 0)hit--;
}

void Attack::handleData(ComponentData* data)
{
	checkNullAndBreak(data);
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "quickCooldown")
		{
			setFloat(quickAttackCooldown);
		}
		else if (prop.first == "strongCooldown")
		{
			setFloat(strongAttackCooldown);
		}
		else if (prop.first == "quickDamage")
		{
			setInt(quickAttackDamage);
		}
		else if (prop.first == "strongDamage")
		{
			setInt(strongAttackDamage);
		}
		else if (prop.first == "attackDuration")
		{
			setFloat(attackDuration);
		}
		else if (prop.first == "quickCharge")
		{
			setFloat(quickChargeTime);
		}
		else if (prop.first == "strongCharge")
		{
			setFloat(strongChargeTime);
		}
		else if (prop.first == "quickAttackScale")
		{
			setVector3(quickAttackScale);
		}
		else if (prop.first == "strongAttackScale")
		{
			setVector3(strongAttackScale);
		}
		else if (prop.first == "quickAttackOffset")
		{
			setVector3(quickAttackOffset);
		}
		else if (prop.first == "strongAttackOffset")
		{
			setVector3(strongAttackOffset);
		}
		else
			LOG("ATTACK: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void Attack::onObjectStay(GameObject* other)
{
	checkNullAndBreak(other);

	if (other->getTag() == "Player" && parent != nullptr && other != parent && state == ATTACKING) // If it hits a player different than myself
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

		if (aux.size() > 0 && aux[0] != nullptr)
			enemyBlock = aux[0]->getComponent<Block>();

		if (!notNull(enemyBlock) || !notNull(parent->transform) || !enemyBlock->blockAttack(parent->transform->getPosition()))
		{
			Health* enemyHealth = other->getComponent<Health>();

			if (notNull(enemyHealth) && !enemyHealth->isInvencible())
			{
				enemyHealth->receiveDamage(damage);
				hit = 2;

				PlayerIndex* otherIndex = other->getComponent<PlayerIndex>();
				if (notNull(otherIndex) && notNull(score))
				{
					score->attackHitted(id);
					score->damageReceivedFrom(otherIndex->getIndex(), id, damage);

					if (!enemyHealth->isAlive())
						score->killedBy(otherIndex->getIndex(), id);
				}
			}
		}
		stop();
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
	if (notNull(attackTrigger)) attackTrigger->setActive(true);
	if (notNull(score)) score->attackDone(id);
	LOG("Attack!\n");
}

void Attack::setUpTriggerAttack(const Vector3& scale, const Vector3& offset)
{
	checkNullAndBreak(attackTrigger);
	checkNullAndBreak(attackTrigger->gameObject);

	Transform* attackTransform = attackTrigger->gameObject->transform;
	checkNullAndBreak(attackTransform);

	// Scale trigger
	Vector3 scaleRatio = scale;
	Vector3 currentScale = Vector3::ZERO;
	currentScale = attackTransform->getScale();
	attackTrigger->multiplyScale(scaleRatio);
	scaleRatio *= currentScale;

	// Move an offset
	attackTransform->setPosition(offset);
}

bool Attack::attackOnCD() const
{
	return (cooldown > 0);
}

void Attack::quickAttack()
{
	checkNullAndBreak(parent);

	PlayerState* aux = parent->getComponent<PlayerState>();
	if (cooldown <= 0.0f && notNull(aux) && aux->canAttack())
	{
		currentAttack = QUICK;
		setUpTriggerAttack(quickAttackScale, quickAttackOffset);
		charge(quickAttackCooldown, quickChargeTime);
	}
	else
		LOG("Attack on CD...\n");
}

void Attack::strongAttack()
{
	checkNullAndBreak(parent);

	PlayerState* aux = parent->getComponent<PlayerState>();
	if (cooldown <= 0.0f && notNull(aux) && aux->canAttack())
	{
		currentAttack = STRONG;
		setUpTriggerAttack(strongAttackScale, strongAttackOffset);
		charge(strongAttackCooldown, strongChargeTime);
	}
	else
		LOG("Attack on CD...\n");
}

void Attack::stop()
{
	// Deactivate the trigger until the next attack is used
	if (notNull(attackTrigger)) attackTrigger->setActive(false);

	// Reset the current attack state
	state = NOT_ATTACKING;
}

bool Attack::isAttacking() const
{
	return state == ATTACKING || state == CHARGING;
}

bool Attack::isAttackOnRange(GameObject* obj, const Vector3& scale) const
{
	Transform* target = nullptr;
	Transform* attacker = nullptr;
	if (notNull(obj)) target = obj->transform;
	if (notNull(parent)) attacker = parent->transform;

	// Target is in the direction of attack?
	checkNullAndBreak(target, false);
	checkNullAndBreak(attacker, false);
	if (!(attacker->getRotation().y < 0 && target->getPosition().x < attacker->getPosition().x) && !(attacker->getRotation().y > 0 && target->getPosition().x > attacker->getPosition().x))
		return false; // If not, return false

	checkNullAndBreak(attackTrigger, false);
	checkNullAndBreak(attackTrigger->gameObject, false);
	Transform* attackTransform = attackTrigger->gameObject->transform;

	// trigger Scale
	checkNullAndBreak(attackTransform, false);
	Vector3 triggerScale = scale * attackTransform->getScale();
	// Distance between attacker and target
	float distX = abs(target->getPosition().x - attacker->getPosition().x);
	float distY = abs(target->getPosition().y - attacker->getPosition().y);

	bool inside;
	// Target is inside sensor?
	inside = distX <= triggerScale.z && distY <= triggerScale.y;

	return inside;
}

bool Attack::isQuickAttackOnRange(GameObject* obj) const
{
	return isAttackOnRange(obj, quickAttackScale);
}

bool Attack::isStrongAttackOnRange(GameObject* obj) const
{
	return isAttackOnRange(obj, strongAttackScale);
}

bool Attack::isQuickAttacking() const
{
	return isAttacking() && currentAttack == QUICK;
}

bool Attack::isHeavyAttacking() const
{
	return isAttacking() && currentAttack == STRONG;
}

bool Attack::hasHit() const
{
	return hit > 0;
}