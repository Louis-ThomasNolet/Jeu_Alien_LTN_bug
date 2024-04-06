#pragma once
#include "GameObject.h"

class PowerUp : public GameObject
{	
	public:
		PowerUp();
		void init(const int texture);


		sf::Clock lifeTimer;
	private:
		

};

