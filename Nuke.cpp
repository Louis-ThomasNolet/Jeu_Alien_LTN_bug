#include "Nuke.h"
#include "ContentPipeline.h"
#include "Constants.h"

Nuke::Nuke()
{
}

void Nuke::init(const int powerUpTexture)
{
	PowerUp::init(powerUpTexture);
	nukeBuffer = ContentPipeline::getInstance().getExplosionSoundBuffer();
	nukeSound.setBuffer(nukeBuffer);

	// J'ai mis le son de la bombe a 50% car il est vraiment fort
	nukeSound.setVolume(50);
}