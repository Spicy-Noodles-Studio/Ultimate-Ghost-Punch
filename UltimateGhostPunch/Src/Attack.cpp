#include "Attack.h"
#include "RigidBody.h"
#include "InputSystem.h"

Attack::Attack(GameObject* gameObject) : UserComponent(gameObject)
{
}

void Attack::start()
{
	// Deactivate the trigger until the attack is used
	gameObject->getComponent<RigidBody>()->setActive(false);
}

void Attack::update(float deltaTime)
{
	/////	PRUEBAS
	if (InputSystem::GetInstance()->getKeyPress("z"))
		quickAttack();

	if (InputSystem::GetInstance()->getKeyPress("x"))
		strongAttack();
	/////


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
			gameObject->getComponent<RigidBody>()->setActive(false);

			// Reset the current attack mode
			currentAttack = NOT_ATTACKING;
		}
	}
}

void Attack::attack(float newCooldown)
{
	gameObject->getComponent<RigidBody>()->setActive(true);
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

void Attack::onTriggerEnter(GameObject* other)
{
	if (other->getTag() == "player")
	{
		printf("You hit a player!\n");
		float damage;
		switch (currentAttack)
		{
		case Attack::QUICK:
			damage = quickAttackDamage;
			break;
		case Attack::STRONG:
			damage = strongAttackDamage;
			break;
		}

		// other->getComponent<Vida>().receiveDamage(damage);
	}
}