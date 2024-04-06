#pragma once
#include "GameObject.h"
#include "Score.h"

class AlienScore : public GameObject
{
	public:
		AlienScore();
		void init();
		void updateScore(const float score);
		void draw(sf::RenderWindow& renderWindow);
		void resetScore();
		
		sf::Text scoreText;
		sf::Clock timer;	
};

