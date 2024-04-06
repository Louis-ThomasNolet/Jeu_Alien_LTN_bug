#include "PowerUp.h"
#include "ContentPipeline.h"
#include "Constants.h"

PowerUp::PowerUp()
{
}

void PowerUp::init(const int powerUpTexture)
{
	setTexture(ContentPipeline::getInstance().getLogoTexture(powerUpTexture));
}