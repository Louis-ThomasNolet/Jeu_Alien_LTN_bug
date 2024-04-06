#pragma once

struct Inputs
{
	const float DEAD_ZONE = 20.0f;

	Inputs()
	{
		reset();
	}

	void reset()
	{
		moveVertical = 0.0f;
		moveHorizontal = 0.0f;

		//Peut-être pratique de sauvegarder le fait
		//que le gamepad soit actif ou non (voir game.getInputs)
		isGamepadActive = false; 
		changeRotation = false;
		fire = false;
	}	

	//Portée analogue -100 à 100
	float manageGamepadAxis(const float axisValue)
	{
		if (abs(axisValue) < DEAD_ZONE) return 0.0f;
		return axisValue / 100.0f;
	}

	float moveVertical;
	float moveHorizontal;
	sf::Vector2f mousePosition ;
	bool isGamepadActive;
	bool fire;
	bool hasFired;

	float rightStickHorizontal = 1;
	float rightStickVertical = 0;
	bool changeRotation;
	int bullet = 0;
	int boostBullet = 0;

	sf::Clock fireTimer;

	bool pause = false;
	bool pausePressed = false; //Pour éviter de déclencher l'action plusieurs fois
};

