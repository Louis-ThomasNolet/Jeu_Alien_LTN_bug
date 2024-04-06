#include "Player.h"
#include "ContentPipeline.h"
#include "Constants.h"

Player::Player()
{

}

void Player::init(const SoundBuffer& sound)
{
	setTexture(ContentPipeline::getInstance().getPlayerTexture());
	setCollisionCircleRadius(PLAYER_RADIUS);
	playerBuffer = sound;
	playerSound.setBuffer(playerBuffer);
}

void Player::rotateToMousePosition(const Vector2f mousePosition)
{
    Vector2f direction = mousePosition - getPosition();
    float angle = atan2f(direction.y, direction.x) * RADIANS_TO_DEGREE;

    setRotation(angle);
}


void Player::keepInBound(const float width, const float height)
{
	if (getPosition().x < PLAYER_RADIUS)
	{
		setPosition(PLAYER_RADIUS, getPosition().y);
	}
	if (getPosition().x > width - PLAYER_RADIUS)
	{
		setPosition(width - PLAYER_RADIUS, getPosition().y);
	}
	if (getPosition().y < PLAYER_RADIUS)
	{
		setPosition(getPosition().x, PLAYER_RADIUS);
	}
	if (getPosition().y > height - PLAYER_RADIUS)
	{
		setPosition(getPosition().x, height - PLAYER_RADIUS);
	}
}

void Player::resetBoost()
{
	boostActive = false;
	boostDuration = 0;
}

