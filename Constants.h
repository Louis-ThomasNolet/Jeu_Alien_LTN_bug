#pragma once
static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 720;

static const int WORLD_WIDTH = 3840;
static const int WORLD_HEIGHT = 2160;

const float SHOOTING_SPEED = 1.0f / 6.0f;
const float BOOST_SHOOTING_SPEED = 1.0f / 3.0f;
const float MOVE_SPEED = 5.0f;
const float BOOST_SPEED = 7.0f;
const float BOOST_LIFE = 5.0f;
const float ALIEN_SPAWN_SHIELD = 1.0f;
const float COMBO_TIMER = 1.0f;
const float BULLET_SPEED = 5.0f;
const float BOOST_BULLET_SPEED = 3.0f;
const float PLAYER_RESPAWN_SHIELD = 3.0f;
const float DEATH_TIMER = 1.0f;
const float ALIEN_SPEED = 2.5f;
const float PI = 3.141592653f;
const float POWER_UP_LIFE = 15.0F;
const float SPAWN_RADIUS = 500.0F;
const float ALIEN_ANIMATION = 1.0f;

const int ALIENS = 50;
const int BULLETS = 100;
const int POWER_UP = 6;
const int BASE_ALIEN_VALUE = 100;
const int MAX_ALIEN_VALUE = 500;
const int COMBO_SCORE = 10;
const int HEALTH_UP = 50000;
const int BASE_HEALTH_POINTS = 5;
const int DEAD = 0;
const int PLAYER_RADIUS = 32;
const int LAST_BULLET = 99;
const int FIRST_BULLET = 0;
const int BOOST_DURATION = 5;
const int CONTROLLER_MULT = 1000;
const int PERCENTAGE_POWER_UP_SPAWN = 4;

const sf::Color WHITE = sf::Color(255, 255, 255, 255);
const sf::Color RED = sf::Color(255, 0, 0, 255);
const sf::Color RESPAWN_COLOR = sf::Color(255, 255, 255, 100);
const sf::Color BOOST_COLOR = sf::Color(212, 175, 55, 255);