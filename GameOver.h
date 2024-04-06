#pragma once
#include <SFML/Graphics.hpp>
#include "Hud.h"

class GameOver
{
	public:
		GameOver();
		void gameOverInit();
		void draw(sf::RenderWindow& renderWindow);
		void updateGameOver();
		bool getGameOver() const;
		void setGameOver(bool gameOver);

	private:
		sf::Text gameOverText;
		bool gameOver = false;
};

