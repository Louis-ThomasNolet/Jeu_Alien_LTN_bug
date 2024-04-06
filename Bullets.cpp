#include "Bullets.h"
#include "ContentPipeline.h"
#include "Constants.h"


Bullets::Bullets()
{
}

void Bullets::init(const int texture,const SoundBuffer& sound)
{
	setTexture(ContentPipeline::getInstance().getProjectileTexture(texture));
	bulletBuffer = sound;
	bulletSound.setBuffer(bulletBuffer);
}

void Bullets::fireBulletController(const Vector2f playerPosition, const float rightStickHorizontal, const float rightStickVertical)
{
	this->setPosition(playerPosition.x, playerPosition.y);
	this->activate();
	this->xMove = rightStickHorizontal / CONTROLLER_MULT;
	this->yMove = rightStickVertical / CONTROLLER_MULT;
}

void Bullets::fireBulletMouse(const Vector2f playerPosition, const Vector2f mousePosition)
{
	this->setPosition(playerPosition.x, playerPosition.y);
	this->activate();

	sf::Vector2f direction = mousePosition - playerPosition;
	this->xMove = direction.x;
	this->yMove = direction.y;
}

void Bullets::moveBullet(const float speed)
{
	// On normalise le vecteur de direction pour avoir une vitesse constante
	Vector2f direction = Vector2f(this->xMove, this->yMove);
	float distance = sqrt(this->xMove * this->xMove + this->yMove * this->yMove);
	direction = direction / distance;
	this->move(direction * speed);

	if (this->getPosition().x > WORLD_WIDTH || this->getPosition().x < 0 || this->getPosition().y > WORLD_HEIGHT || this->getPosition().y < 0)
	{
		this->setPosition(WORLD_WIDTH + 200, WORLD_HEIGHT + 200);
		this->deactivate();
	}
}