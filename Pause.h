#pragma once
#include <SFML/Graphics.hpp>
#include "Hud.h"

class Pause
{
	public:
		Pause();
		void pauseInit();
		void draw(sf::RenderWindow& renderWindow);
		void updatePause();
		bool getPause() const;
		void setPause(bool pause);

	private:
		sf::Text pauseText;
		bool paused = false;
};

