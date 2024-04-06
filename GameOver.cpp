#include "GameOver.h"
#include "Hud.h"
#include "ContentPipeline.h"

GameOver::GameOver()
{
}

void GameOver::gameOverInit()
{
	gameOverText.setFont(ContentPipeline::getInstance().getFont());
	gameOverText.setString("");
	gameOverText.setCharacterSize(175);
	gameOverText.setFillColor(sf::Color::Red);
	gameOverText.setPosition(50, 200);
}

void GameOver::draw(sf::RenderWindow& renderWindow)
{
	
	if(gameOver)
		gameOverText.setString("GAME OVER");
	else if(!gameOver)
		gameOverText.setString("");

	renderWindow.draw(gameOverText);
}

void GameOver::updateGameOver()
{
	gameOver = !gameOver;
}

bool GameOver::getGameOver() const
{
	return gameOver;
}

void GameOver::setGameOver(bool gameover)
{
	gameOver = gameover;
}


