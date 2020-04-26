#include <ComponentRegister.h>
#include <GameObject.h>
#include "CameraEffects.h"


REGISTER_FACTORY(CameraEffects);

CameraEffects::CameraEffects(GameObject* gameObject) : UserComponent(gameObject)
{
	
}

CameraEffects::~CameraEffects()
{

}

void CameraEffects::fixedUpdate(float deltaTime)
{

}

