#pragma once
#include "Hud.h"

class Health
{
	public:
		Health();
		void healthInit();
		void draw(sf::RenderWindow& renderWindow);
		void updateHealth();
		int getHealth() const;
		void resetHealth();
		void addHealth();

		int healthPoints = 5;

	private:
		sf::Text healthText;
};

