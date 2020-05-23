#include "PlayerFX.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <sstream>

#include "MeshRenderer.h"
#include "Health.h"
#include "GhostManager.h"

REGISTER_FACTORY(PlayerFX);

PlayerFX::PlayerFX(GameObject* gameObject) : UserComponent(gameObject), effect(NONE), shieldMesh(nullptr), mesh(nullptr), health(nullptr), ghost(nullptr), diffuses(),
time(0), hurtTime(0.5f), invencibleFrec(0.1f), frecuency(invencibleFrec), ghostFXFrec(0.1f), ghostFXTime(2)
{

}

PlayerFX::~PlayerFX()
{

}

void PlayerFX::start()
{
	mesh = gameObject->getComponent<MeshRenderer>();
	health = gameObject->getComponent<Health>();
	ghost = gameObject->getComponent<GhostManager>();
	checkNull(health);
	checkNull(ghost);

	if (notNull(mesh))
		for (int i = 0; i < mesh->getSubentitiesSize(); i++)
			diffuses.push_back(mesh->getDiffuse(i));

	std::vector<GameObject*> shieldObject = gameObject->findChildrenWithTag("shield");
	if (shieldObject.size() > 0)
		shieldMesh = shieldObject[0]->getComponent<MeshRenderer>();
	checkNullAndBreak(shieldMesh);

	if (diffuses.size() > 0) {
		shieldMesh->setFpParam(0, "r", diffuses[0].x);
		shieldMesh->setFpParam(0, "g", diffuses[0].y);
		shieldMesh->setFpParam(0, "b", diffuses[0].z);
		shieldMesh->setVisible(false);
	}
}

void PlayerFX::update(float deltaTime)
{
	updateHurtFX(deltaTime);
	updateInvencibleFX(deltaTime);
	updateGhostFX(deltaTime);
}

void PlayerFX::handleData(ComponentData* data)
{
	checkNullAndBreak(data);
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "hurtTime")
		{
			setFloat(hurtTime);
		}
		else if (prop.first == "invencibleFrec")
		{
			setFloat(invencibleFrec);
		}
		else if (prop.first == "ghostFXFrec")
		{
			setFloat(ghostFXFrec);
		}
		else if (prop.first == "ghostFXTime")
		{
			setFloat(ghostFXTime);
		}
	}
}

void PlayerFX::updateHurtFX(float deltaTime)
{
	if (time > 0.0f)
		time -= deltaTime;
	else if (effect == HURT)
		deactivateHurt();
}

void PlayerFX::updateInvencibleFX(float deltaTime)
{
	if (notNull(health) && health->isInvencible())
	{
		effect = INVENCIBLE;

		if (frecuency > 0.0f)
		{
			frecuency -= deltaTime;
			if (frecuency <= 0.0f)
			{
				activateInvencible();
				frecuency = -invencibleFrec;
			}
		}
		else
		{
			frecuency += deltaTime;
			if (frecuency >= 0.0f)
			{
				deactivateInvencible();
				frecuency = invencibleFrec;
			}
		}
	}
	else if (effect == INVENCIBLE)
		deactivateInvencible();
}

void PlayerFX::updateGhostFX(float deltaTime)
{
	if (notNull(ghost) && ghost->isGhost() && ghost->getGhostTime() < ghostFXTime)
	{
		effect = GHOST;

		if (frecuency > 0.0f)
		{
			frecuency -= deltaTime;
			if (frecuency <= 0.0f)
			{
				activateGhostFX();
				frecuency = -invencibleFrec;
			}
		}
		else
		{
			frecuency += deltaTime;
			if (frecuency >= 0.0f)
			{
				deactivateGhostFX();
				frecuency = invencibleFrec;
			}
		}
	}
	else if (effect == GHOST)
		deactivateGhostFX();
}

void PlayerFX::activateHurt()
{
	checkNullAndBreak(mesh);

	time = hurtTime;

	for (int i = 0; i < mesh->getSubentitiesSize(); i++)
		mesh->setDiffuse(i, { 255,0,0 }, 1);
}

void PlayerFX::deactivateHurt()
{
	checkNullAndBreak(mesh);

	effect = NONE;

	for (int i = 0; i < mesh->getSubentitiesSize() && i < diffuses.size(); i++)
		mesh->setDiffuse(i, diffuses[i], 1);
}

void PlayerFX::activateInvencible()
{
	checkNullAndBreak(mesh);

	for (int i = 0; i < mesh->getSubentitiesSize(); i++)
		mesh->setDiffuse(i, { 255,255,255 }, 1);
}

void PlayerFX::deactivateInvencible()
{
	checkNullAndBreak(mesh);

	effect = NONE;

	for (int i = 0; i < mesh->getSubentitiesSize() && i < diffuses.size(); i++)
		mesh->setDiffuse(i, diffuses[i], 1);
}

void PlayerFX::activateGhostFX()
{
	checkNullAndBreak(mesh);
	mesh->setVisible(false);
}

void PlayerFX::deactivateGhostFX()
{
	checkNullAndBreak(mesh);
	effect = NONE;
	mesh->setVisible(true);
}

void PlayerFX::activateShield()
{
	checkNullAndBreak(shieldMesh);
	
	effect = SHIELD;
	shieldMesh->setVisible(true);
}

void PlayerFX::updateShield(float blockTime, float maxBlockTime)
{
	checkNullAndBreak(shieldMesh);
	// Shield scale reduction
	if (notNull(shieldMesh->gameObject->transform))
		shieldMesh->gameObject->transform->setScale(Vector3::IDENTITY * blockTime + Vector3(4, 4, 4));
	// Shield alpha
	shieldMesh->setFpParam(0, "alpha", blockTime / maxBlockTime);
}

void PlayerFX::deactivateShield()
{
	checkNullAndBreak(shieldMesh);

	effect = NONE;
	shieldMesh->setVisible(false);
}