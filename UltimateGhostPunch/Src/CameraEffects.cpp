#include <ComponentRegister.h>
#include <GameObject.h>
#include "CameraEffects.h"
#include "RenderSystem.h"
#include "Camera.h"
#include "InputSystem.h"

REGISTER_FACTORY(CameraEffects);

CameraEffects::CameraEffects(GameObject* gameObject) : UserComponent(gameObject)
{
	
}

CameraEffects::~CameraEffects()
{

}

void CameraEffects::start()
{
	min = 0.0f;
	max = 1.0f;
	current = max;
	state = IDLE;
	shakeDir = Vector3(0, 0, 0);
	dir = 1.0;
	moves = 0;
	time = 0;
	vel = 2.0f;
	duration = 2000;
	minRange = -5;
	maxRange = 5;

	cam = gameObject->getComponent<Transform>();
	input = InputSystem::GetInstance();

	initialRotation = cam->getRotation();
}

void CameraEffects::fixedUpdate(float deltaTime)
{
	checkInput();


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

	}

	else if (state == SHAKE) {
		
		cam->rotate(rotationDir * dir);
		
		moves += dir;
		time += 20;
		if ((moves >= maxRange && dir > 0) || (moves <= -minRange && dir < 0)) dir *= -1;

		if (time >= duration) {
			state = IDLE;
			cam->setRotation(initialRotation);
			time = 0;
			moves = 0;
		}

	}
	
}



void CameraEffects::fadeOut()
{
	if (state == IDLE)
	state = FADEOUT;

}

void CameraEffects::fadeIn()
{
	if(state == IDLE)
	state = FADEIN;
}

void CameraEffects::shake(Vector3 rotDir)
{
	if (state == IDLE) {
		state = SHAKE;
		rotationDir = rotDir;
	}
}

void CameraEffects::checkInput()
{
	if (input->getKeyPress("N")) fadeOut();
	else if (input->getKeyPress("M")) fadeIn();
	else if (input->getKeyPress("B")) shake(Vector3(0,0,1));
}

void CameraEffects::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "vel") {
			setFloat(vel);
		}
		else if (prop.first == "minRange") {
			setFloat(minRange);
		}
		else if (prop.first == "maxRange") {
			setFloat(maxRange);
		}
		else if (prop.first == "duration") {
			setFloat(duration);
		}
		else
			LOG("DODGE: Invalid property name %s", prop.first.c_str());
	}
}

