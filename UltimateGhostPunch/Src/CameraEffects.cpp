#include "CameraEffects.h"
#include <ComponentRegister.h>
#include <RenderSystem.h>
#include <InputSystem.h>
#include <GameObject.h>
#include <Camera.h>
#include "WindowManager.h"
#include "MathUtils.h"

REGISTER_FACTORY(CameraEffects);

CameraEffects::CameraEffects(GameObject* gameObject) : UserComponent(gameObject), min(0), max(1), current(0), state(IDLE), mainCamera(nullptr), shakeDir(Vector3::ZERO), rotationDir(Vector3::ZERO),
initialRotation(Vector3::ZERO), dirX(1), dirY(1), dirZ(1), moves(0), time(0), vel(2), minRange(-5), maxRange(5), duration(2000)
{

}

CameraEffects::~CameraEffects()
{

}

void CameraEffects::start()
{
	max = WindowManager::GetInstance()->getBrightness() + 0.5;
	if (max == 0) max = 0.00001;
	current = max;
	state = IDLE;

	mainCamera = gameObject->getComponent<Transform>();
	if (mainCamera != nullptr) {
		initialRotation = mainCamera->getRotation();
		initialPosition = mainCamera->getPosition();
	}
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

		RenderSystem::GetInstance()->changeParamOfShader("Brightness", "bright", current);
	}
	else if (state == FADEIN)
	{
		current += (0.4 * max * deltaTime);
		if (current > max)
		{
			current = max;
			state = IDLE;
		}

		RenderSystem::GetInstance()->changeParamOfShader("Brightness", "bright", current);
	}
	else if (state == SHAKE)
	{
		time += deltaTime * 1000;

		float moveX, moveY, moveZ;

		moveX = random() * vel * dirX;
		moveY = random() * vel * dirY;
		moveZ = random() * vel * dirZ;
		
		if (mainCamera == nullptr) return;
		
		Vector3 pos = mainCamera->getPosition();
		mainCamera->setPosition(Vector3(pos.x + moveX * rotationDir.x, pos.y + moveY * rotationDir.y, pos.z + moveZ * rotationDir.z));
		Vector3 newPos = mainCamera->getPosition();

		if ((newPos.x >= initialPosition.x + maxRange && dirX > 0) || (newPos.x <= initialPosition.x + minRange && dirX < 0))
			dirX *= -1;

		if ((newPos.y >= initialPosition.y + maxRange && dirY > 0) || (newPos.y <= initialPosition.y + minRange && dirY < 0))
			dirY *= -1;

		if ((newPos.z >= initialPosition.z + maxRange && dirZ > 0) || (newPos.z <= initialPosition.z + minRange && dirZ < 0))
			dirZ *= -1;

		if (time >= duration)
		{
			state = IDLE;
			mainCamera->setRotation(initialRotation);
			mainCamera->setPosition(initialPosition);
			time = 0;
			moves = 0;
		}
	}
}

void CameraEffects::handleData(ComponentData* data)
{
	if (data == nullptr) return;
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
		if(mainCamera != nullptr) mainCamera->setRotation(initialRotation);
	}
}

void CameraEffects::fadeIn()
{
	if (state == IDLE)
		state = FADEIN;
	else if (state == SHAKE) {
		state = FADEIN;
		if(mainCamera != nullptr) mainCamera->setRotation(initialRotation);
	}
}

void CameraEffects::setDarkness()
{
	RenderSystem::GetInstance()->changeParamOfShader("Brightness", "bright", 0);
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
		if(mainCamera != nullptr) initialPosition = mainCamera->getPosition();
	}
}