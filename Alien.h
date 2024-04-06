#pragma once
#include "GameObject.h"
#include "AlienScore.h"

class Alien : public GameObject
{
	public:
		Alien();
		void init(const int alienTextureNumber, const SoundBuffer& sound);
		void kill();
		void draw(sf::RenderWindow& renderWindow);
		void manageAlienScore(const int alienValue, sf::RenderWindow& renderWindow);
		void deactivate();

		SoundBuffer alienBuffer;
		Sound alienSound;

		AlienScore score;

		sf::Clock life;
		float currentScale = 0;

	private:
		const float SCORE_LIFE = 1.0f;
};

