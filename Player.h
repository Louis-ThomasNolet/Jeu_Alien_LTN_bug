#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	Player();
	void init(const SoundBuffer& sound);
	void rotateToMousePosition(const Vector2f mousePosition);
	void keepInBound(const float width, const float height);
	void resetBoost();

	SoundBuffer playerBuffer;
	Sound playerSound;

	bool boostActive = false;
	int boostDuration = 0;
	sf::Clock boostTimer;
private:
	const float PI = 3.14159265358979323846f;
	const float RADIANS_TO_DEGREE = 180.0f / PI;
	
};

