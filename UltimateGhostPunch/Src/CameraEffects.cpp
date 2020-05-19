#include "CameraEffects.h"
#include <ComponentRegister.h>
#include <RenderSystem.h>
#include <InputSystem.h>
#include <GameObject.h>
#include <Camera.h>
#include "WindowManager.h"

REGISTER_FACTORY(CameraEffects);

CameraEffects::CameraEffects(GameObject* gameObject) : UserComponent(gameObject), min(0), max(1), current(0), state(IDLE), cam(nullptr), shakeDir(Vector3::ZERO), rotationDir(Vector3::ZERO),
initialRotation(Vector3::ZERO), dir(1), moves(0), time(0), vel(2), minRange(-5), maxRange(5), duration(2000)
{

}

CameraEffects::~CameraEffects()
{

}

void CameraEffects::start()
{
	max = WindowManager::GetInstance()->getBrightness();
	if (max == 0) max = 0.00001;
	current = max;
	state = IDLE;

	cam = gameObject->getComponent<Transform>();
	initialRotation = cam->getRotation();
}

void CameraEffects::update(float deltaTime)
{
	if (state == FADEOUT)
	{
		current -= (0.4 * max * deltaTime);
		if (current < min)
		{
			current = min;
			state = IDLE;
		}

		RenderSystem::GetInstance()->changeParamOfShader("LuminancePS", "brigh", current);
	}
	else if (state == FADEIN)
	{
		current += (0.4 * max * deltaTime);
		if (current > max)
		{
			current = max;
			state = IDLE;
		}

		RenderSystem::GetInstance()->changeParamOfShader("LuminancePS", "brigh", current);
	}
	else if (state == SHAKE)
	{

		cam->rotate(rotationDir * dir * vel * deltaTime);
		moves += dir*vel*deltaTime;
		time += deltaTime * 1000;

		if ((moves >= maxRange && dir > 0) || (moves <= minRange && dir < 0))
			dir *= -1;

		if (time >= duration)
		{
			state = IDLE;
			cam->setRotation(initialRotation);
			time = 0;
			moves = 0;
		}
	}
}

void CameraEffects::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "vel")
		{
			setFloat(vel);
		}
		else if (prop.first == "minRange")
		{
			setFloat(minRange);
		}
		else if (prop.first == "maxRange")
		{
			setFloat(maxRange);
		}
		else if (prop.first == "duration")
		{
			setFloat(duration);
		}
		else
			LOG("DODGE: Invalid property name %s", prop.first.c_str());
	}
}

void CameraEffects::fadeOut()
{
	if (state == IDLE)
		state = FADEOUT;
	else if (state == SHAKE) {
		state = FADEOUT;
		cam->setRotation(initialRotation);
	}
}

void CameraEffects::fadeIn()
{
	if (state == IDLE)
		state = FADEIN;
	else if (state == SHAKE) {
		state = FADEIN;
		cam->setRotation(initialRotation);
	}
}

void CameraEffects::setDarkness()
{
	RenderSystem::GetInstance()->changeParamOfShader("LuminancePS", "brigh", 0);
	current = 0;
}

bool CameraEffects::isFading()
{
	return state != IDLE;
}

void CameraEffects::shake(Vector3 rotDir)
{
	if (state == IDLE)
	{
		state = SHAKE;
		rotationDir = rotDir;
	}
}