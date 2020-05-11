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
#include "PlayerAnimController.h"

REGISTER_FACTORY(Attack);

Attack::Attack(GameObject* gameObject) : UserComponent(gameObject), attackTrigger(nullptr), score(nullptr), currentAttack(AttackType::NONE), state(AttackState::NOT_ATTACKING), activeTime(0.0f), attackDuration(0.5f),
										 strongAttackDamage(2), quickAttackDamage(1), chargeTime(0), strongChargeTime(0.75f), quickChargeTime(0.5f), strongAttackCooldown(2.0f),
										 quickAttackCooldown(0.5f), cooldown(0.0f), quickAttackScale(Vector3::IDENTITY), strongAttackScale(Vector3::IDENTITY), 
										 quickAttackOffset(Vector3::ZERO), strongAttackOffset(Vector3::ZERO), id(0), parent(nullptr), hit(false)
{

}

Attack::~Attack()
{

}

void Attack::start()
{
	parent = gameObject->getParent();
	if (parent != nullptr) 
		id = parent->getComponent<PlayerIndex>()->getIndex();

	attackTrigger = gameObject->getComponent<RigidBody>();
	score = GameManager::GetInstance()->getScore();

	// Deactivate the trigger until the attack is used
	if (attackTrigger != nullptr) attackTrigger->setActive(false);
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

void Attack::postUpdate(float deltaTime)
{
	hit = false;
}

void Attack::handleData(ComponentData* data)
{
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
		PlayerIndex* otherIndex = other->getComponent<PlayerIndex>();
		Block* enemyBlock = nullptr;

		if (aux.size() > 0)
			enemyBlock = aux[0]->getComponent<Block>();

		if (enemyBlock != nullptr && parent != nullptr)
		{
			Health* enemyHealth = other->getComponent<Health>();
			if(!enemyHealth->isInvencible()) hit = true;

			if(!enemyBlock->blockAttack(damage, parent->transform->getPosition()))
			{
				score->receiveHitFrom(otherIndex->getIndex(),id );
				score->damageRecivedFrom(otherIndex->getIndex(),id, damage);

				if (!enemyHealth->isAlive())
					score->killedBy(otherIndex->getIndex(), id);
			}
			
			// Deactivate the trigger until the next attack is used
			attackTrigger->setActive(false);

			// Reset the current attack state
			state = NOT_ATTACKING;

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

void Attack::setUpTriggerAttack(const Vector3& scale, const Vector3& offset)
{
	Transform* attackTransform = attackTrigger->gameObject->transform;

	// Scale trigger
	Vector3 scaleRatio = scale;
	Vector3 currentScale = attackTransform->getScale();
	attackTrigger->multiplyScale(scaleRatio);
	scaleRatio *= currentScale;

	// Move an offset
	attackTransform->setPosition(offset);
}

void Attack::quickAttack()
{
	if (parent == nullptr) return;
	
	PlayerState* aux = parent->getComponent<PlayerState>();
	if (cooldown <= 0.0f && aux != nullptr && aux->canAttack())
	{
		PlayerAnimController* anim = parent->getComponent<PlayerAnimController>();
		if (anim != nullptr) anim->quickAttackAnimation();

		currentAttack = QUICK;
		setUpTriggerAttack(quickAttackScale, quickAttackOffset);
		charge(quickAttackCooldown, quickChargeTime);
	}
	else
		LOG("Attack on CD...\n");
}

void Attack::strongAttack()
{
	if (parent == nullptr) return;

	PlayerState* aux = parent->getComponent<PlayerState>();
	if (cooldown <= 0.0f && aux != nullptr && aux->canAttack())
	{

		PlayerAnimController* anim = parent->getComponent<PlayerAnimController>();
		if (anim != nullptr) anim->strongAttackAnimation();

		currentAttack = STRONG;
		setUpTriggerAttack(strongAttackScale, strongAttackOffset);
		charge(strongAttackCooldown, strongChargeTime);
	}
	else
		LOG("Attack on CD...\n");
}

bool Attack::isAttacking() const
{
	return state == ATTACKING || state == CHARGING;
}

bool Attack::isHeavyAttacking() const
{
	return isAttacking() && currentAttack == STRONG;
}

bool Attack::isQuickAttacking() const
{
	return isAttacking() && currentAttack == QUICK;
}

bool Attack::hasHit() const
{
	return hit;
}
