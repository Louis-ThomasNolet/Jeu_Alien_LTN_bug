#include "AlienScore.h"
#include "ContentPipeline.h"
#include <iomanip>



AlienScore::AlienScore()
{
}

void AlienScore::init()
{
	scoreText.setFont(ContentPipeline::getInstance().getFont());
	scoreText.setCharacterSize(12);
	scoreText.setFillColor(sf::Color::White);
}

void AlienScore::updateScore(const float score)
{
	// Convert the float score to an integer to truncate the decimal part
	int scoreNoDecimal = static_cast<int>(score);

	// Convert the truncated score to a string and update the score text
	scoreText.setString(std::to_string(scoreNoDecimal));
}


void AlienScore::draw(sf::RenderWindow& renderWindow)
{
	renderWindow.draw(scoreText);
}

void AlienScore::resetScore()
{
	scoreText.setString("");
}