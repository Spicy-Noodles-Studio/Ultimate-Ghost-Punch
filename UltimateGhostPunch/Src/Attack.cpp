#include <GameObject.h>
#include <RigidBody.h>

#include "Health.h"
#include "Attack.h"

Attack::Attack(GameObject* gameObject) : UserComponent(gameObject)
{
}

void Attack::start()
{
	attackTrigger = gameObject->getComponent<RigidBody>();

	// Deactivate the trigger until the attack is used
	if (attackTrigger != nullptr) attackTrigger->setActive(false);
}

void Attack::update(float deltaTime)
{
	// Update the cooldown
	if (cooldown > 0.0f)
	{
		cooldown -= deltaTime;
	}

	// Attack active time
	if (activeTime > 0.0f)
	{
		activeTime -= deltaTime;
	}
	else
	{
		if (currentAttack != NOT_ATTACKING)
		{
			// Deactivate the trigger until the next attack is used
			attackTrigger->setActive(false);

			// Reset the current attack mode
			currentAttack = NOT_ATTACKING;
		}
	}
}

void Attack::attack(float newCooldown)
{
	if(attackTrigger!= nullptr) attackTrigger->setActive(true);
	cooldown = newCooldown;
	activeTime = attackDuration;
	printf("Attack!\n");
}


void Attack::quickAttack()
{
	if (cooldown <= 0.0f)
	{
		currentAttack = QUICK;
		attack(quickAttackCooldown);
	}
	else
		printf("Attack on CD...\n");
}

void Attack::strongAttack()
{
	if (cooldown <= 0.0f)
	{
		currentAttack = STRONG;
		attack(strongAttackCooldown);
	}
	else
		printf("Attack on CD...\n");
}

void Attack::onObjectStay(GameObject* other)
{
	if (other->getTag() == "player" && other != gameObject->getParent() && currentAttack != NOT_ATTACKING)//If it hits a player different than myself
	{
		printf("You hit player %s!\n", other->getName().c_str());
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

		Health* enemyHealth = other->getComponent<Health>();
		if (enemyHealth != nullptr) enemyHealth->receiveDamage(damage);
	}
}