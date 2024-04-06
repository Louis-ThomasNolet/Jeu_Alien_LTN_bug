#include "Boost.h"
#include "ContentPipeline.h"
#include "Constants.h"

Boost::Boost()
{
}

void Boost::init(const int powerUpTexture)
{
	PowerUp::init(powerUpTexture);
	boostBuffer = ContentPipeline::getInstance().getTokenSoundBuffer();
	boostSound.setBuffer(boostBuffer);
}

