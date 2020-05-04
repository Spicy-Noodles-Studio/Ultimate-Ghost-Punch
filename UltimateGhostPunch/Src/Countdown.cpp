#include "Countdown.h"
#include <InterfaceSystem.h>
#include <RenderSystem.h>
#include <SceneManager.h>
#include <WindowManager.h>
#include <GameObject.h>

#include "GameManager.h"

#include <ComponentRegister.h>

REGISTER_FACTORY(Countdown);


Countdown::Countdown(GameObject* gameObject) : UserComponent(gameObject)
{

}

Countdown::~Countdown()
{
	
}