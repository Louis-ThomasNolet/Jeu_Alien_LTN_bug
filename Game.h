#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Inputs.h"
#include "Player.h"
#include "Alien.h"
#include "Hud.h"
#include "Bullets.h"
#include "Score.h"
#include "Health.h"
#include "Pause.h"
#include "GameOver.h"
#include "Nuke.h"
#include "Boost.h"

using namespace sf;

class Game
{
public:
	Game();
	int run();

private:
	const int WORLD_CENTER_X = WORLD_WIDTH / 2;
	const int WORLD_CENTER_Y = WORLD_HEIGHT / 2;

	//Puisque la utilise des positions centrales, les limites d'affichage
	//sont les positions centrales des cadres limites.
	const int WORLD_LIMIT_MIN_X = SCREEN_WIDTH / 2;
	const int WORLD_LIMIT_MIN_Y = SCREEN_HEIGHT / 2;
	const int WORLD_LIMIT_MAX_X = WORLD_WIDTH - WORLD_LIMIT_MIN_X;
	const int WORLD_LIMIT_MAX_Y = WORLD_HEIGHT - WORLD_LIMIT_MIN_Y;

	bool init();
	void getInputs();
	void update();
	void draw();
	bool unload();

	void calculateDeltaTime();

	void ajustCrossingWorldLimits();

	void managePlayer();

	void manageBullets(Bullets bulletType[BULLETS]);
	void manageBulletMovement();
	void manageBulletCollision(Bullets bulletType[BULLETS], bool isBoost);

	void manageCombo(const int alien);

	void manageAliens();
	void killAlien(const int alien);
	void spawnAlien();

	bool managePowerUpSpawn(const Vector2f alienPosition, PowerUp powerUp[POWER_UP]);
	void managePowerUpLife(PowerUp powerUp[POWER_UP]);
	void managePowerUpCollision();

	void deactivateAlienScores();

	RenderWindow renderWindow;
	View mainView;
	View hudView;
	Event event;

    Clock clock;
	float deltaTime;

	Inputs inputs;

	Hud hud;
	Music music;

	Sprite field;
	Player player;

	Alien aliens[50];
	Bullets bullets[100];
	Nuke nukes[6];
	Boost boosts[6];
	Bullets boostBullets[100];

	sf::Clock respawnClock;

	Health health;

	Score score;

	Pause pause;

	GameOver gameOver;

	

	SoundBuffer hordeBuffer;
	Sound hordeSound;

	sf::Clock alienSpawnClock;
	float spawnInterval = 1.0f;


};