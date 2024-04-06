#pragma once
#include <SFML/Graphics.hpp>
#include "Hud.h"

class Score : public Hud
{
	public:
		Score();
		void scoreInit();
		void draw(sf::RenderWindow& renderWindow);
		void updateScore(const int points);
		int getScore() const;
		void resetScore();
		int getHiddenScore() const;
		void resetHiddenScore();

		sf::Clock comboClock;
		bool comboActive = false;

		int alienValue = 100;
	private:
		sf::Text scoreText;
		int currentScore = 0;
		int hiddenScore = 0; // Sert a savoir si le joueur a atteint un certain score pour obtenir un point de vie
};

