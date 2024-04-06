#pragma once
#include "GameObject.h"

class Bullets : public GameObject
{
	public:
		Bullets();
		void fireBulletController(const Vector2f playerPositions,const float rightStickHorizontal, const float rightStickVertical);
		void fireBulletMouse(const Vector2f playerPositions, const Vector2f mousePosition);

		void moveBullet(const float speed);

		void init(const int texture,const SoundBuffer& sound);
		float xMove = 0;
		float yMove = 0;

		SoundBuffer bulletBuffer;
		Sound bulletSound;
};

