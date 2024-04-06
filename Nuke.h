#pragma once
#include "PowerUp.h"

class Nuke : public PowerUp
{
	public:
		Nuke();
		void init(const int texture);

		SoundBuffer nukeBuffer;
		Sound nukeSound;
};

