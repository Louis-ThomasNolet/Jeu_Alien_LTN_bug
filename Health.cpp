#include "Health.h"
#include "ContentPipeline.h"

Health::Health()
{
}

void Health::healthInit()
{
	healthText.setFont(ContentPipeline::getInstance().getFont());
	healthText.setString("Health: " + std::to_string(healthPoints));
	healthText.setCharacterSize(20);
	healthText.setFillColor(Color::Red);
	healthText.setPosition(10.0f, 40.0f);
}

void Health::draw(sf::RenderWindow& renderWindow)
{
	renderWindow.draw(healthText);
}

void Health::updateHealth()
{
	healthPoints--;
	healthText.setString("Health: " + std::to_string(healthPoints));
}

int Health::getHealth() const
{
	return healthPoints;
}

void Health::resetHealth()
{
	healthPoints = 5;
	healthText.setString("Health: " + std::to_string(healthPoints));
}

void Health::addHealth()
{
	healthPoints++;
	healthText.setString("Health: " + std::to_string(healthPoints));
}

