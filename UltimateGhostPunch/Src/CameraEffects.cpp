#include <ComponentRegister.h>
#include <GameObject.h>
#include "CameraEffects.h"


REGISTER_FACTORY(CameraEffects);

CameraEffects::CameraEffects(GameObject* gameObject) : UserComponent(gameObject)
{
	alpha = 0.0f;
	resptr = Ogre::MaterialManager::getSingleton().getByName("Materials/OverlayMaterial");
	mat = dynamic_cast<Ogre::Material*>(resptr.getPointer());

	state = IDLE;
}

CameraEffects::~CameraEffects()
{

}

void CameraEffects::fixedUpdate(float deltaTime)
{
	if (state == FADEIN) {
		if (alpha == 0.0f) state = IDLE;
		else {
			Ogre::Technique* tech = mat->getTechnique(0);	// Get the technique
			Ogre::Pass* pass = tech->getPass(0);	// Get the pass
			Ogre::TextureUnitState* tex = pass->getTextureUnitState(0);	// Get the texture_unit state
			tex->setAlphaOperation(LBX_MODULATE, LBS_MANUAL, LBS_TEXTURE, alpha);	// Change the alpha operation
			alpha -= 0.01;
		}

	}

}

