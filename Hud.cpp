#include "Hud.h"
#include "ContentPipeline.h"

Hud::Hud()
{
}

void Hud::hudInit()
{
	bidon.setFont(ContentPipeline::getInstance().getFont());
	bidon.setString("Aliens are invading Mars!!!");
	bidon.setCharacterSize(50);
	bidon.setFillColor(Color::Red);
	bidon.setPosition(350.0f, 20.0f);
}


void Hud::draw(RenderWindow& renderWindow)
{
	renderWindow.draw(bidon);
}