#include "Pause.h"
#include "Hud.h"
#include "ContentPipeline.h"
#include "Constants.h"

Pause::Pause()
{
}

void Pause::pauseInit()
{
	pauseText.setFont(ContentPipeline::getInstance().getFont());
	pauseText.setString("");
	pauseText.setCharacterSize(200);
	pauseText.setFillColor(sf::Color::Red);
	pauseText.setPosition(200, 200);
}

void Pause::draw(sf::RenderWindow& renderWindow)
{
	
	if(paused)
		pauseText.setString("PAUSED");
	else if(!paused)
		pauseText.setString("");

	renderWindow.draw(pauseText);
}

void Pause::updatePause()
{
	paused = !paused;
}

bool Pause::getPause() const
{
	return paused;
}

void Pause::setPause(bool pause)
{
	paused = pause;
}



