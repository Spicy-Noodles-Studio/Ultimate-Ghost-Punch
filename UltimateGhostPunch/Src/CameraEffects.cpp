#include <ComponentRegister.h>
#include <GameObject.h>
#include "CameraEffects.h"
#include "RenderSystem.h"


REGISTER_FACTORY(CameraEffects);

CameraEffects::CameraEffects(GameObject* gameObject) : UserComponent(gameObject)
{
	
}

CameraEffects::~CameraEffects()
{

}

void CameraEffects::fixedUpdate(float deltaTime)
{
	if (state == FADEOUT) {
		current -= 0.01;
		if (current < min) {
			current = min;
			state = IDLE;
		}

		RenderSystem::GetInstance()->changeParamOfShader("LuminancePS", "brigh",current);
	}
	else if (state == FADEIN) {
		current += 0.01;
		if (current > max) {
			current = max;
			state = IDLE;
		}

		RenderSystem::GetInstance()->changeParamOfShader("LuminancePS", "brigh", current);

		//if(state == IDLE) 
	}
	
}

void CameraEffects::start()
{
	min = 0.0f;
	max = 1.0f;
	current = max;
	state = IDLE;
}

void CameraEffects::fadeOut()
{
	state = FADEOUT;
}

void CameraEffects::fadeIn()
{
	state = FADEIN;
}

