#include "Score.h"
#include "ContentPipeline.h"
Score::Score()
{
}

void Score::scoreInit()
{
	scoreText.setFont(ContentPipeline::getInstance().getFont());
	scoreText.setString("Score: 0");
	scoreText.setCharacterSize(30);
	scoreText.setFillColor(Color::Red);
	scoreText.setPosition(10.0f, 5.0f);
}

void Score::draw(sf::RenderWindow& renderWindow)
{
	renderWindow.draw(scoreText);
}	

void Score::updateScore(const int points)
{
	this->currentScore += points;
	this->hiddenScore += points;
	scoreText.setString("Score: " + std::to_string(currentScore));
}

int Score::getScore() const
{
	return currentScore;
}

void Score::resetScore()
{
	currentScore = 0;
}

int Score::getHiddenScore() const
{
	return hiddenScore;
}

void Score::resetHiddenScore()
{
	hiddenScore = 0;
}