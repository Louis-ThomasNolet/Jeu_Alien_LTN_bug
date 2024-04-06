#pragma once
#include "PowerUp.h"

class Boost : public PowerUp
{
	public:
		Boost();
		void init(const int texture);

		SoundBuffer boostBuffer;
		Sound boostSound;
};

