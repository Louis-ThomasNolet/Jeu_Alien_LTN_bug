#include "Game.h"
#include "ContentPipeline.h"
#include <iostream>

Game::Game()
{
	//On place dans le contructeur ce qui permet à la game elle-même de fonctionner

	renderWindow.create(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32), "Invasion of Mars");  // , Style::Titlebar); / , Style::FullScreen);
	
	mainView = View(FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	mainView.setCenter(WORLD_WIDTH / 2, WORLD_HEIGHT / 2);
	
	hudView = View(FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

	//Synchonisation coordonnée à l'écran!  Normalement 60 frames par secondes. À faire absolument
	//renderWindow.setVerticalSyncEnabled(true);  //De plus en plus d'écrans sont rafraichis à plus de 60 frames par seconde, alors attention.
	renderWindow.setFramerateLimit(60);  //Équivalent... normalement, mais pas toujours. À utiliser si la synchonisation de l'écran fonctionne mal.
	//https://www.sfml-dev.org/tutorials/2.6/window-window.php

	renderWindow.setKeyRepeatEnabled(false);
	//Seed random.
	srand((int)time(0));
}

int Game::run()
{
	if (!init())return EXIT_FAILURE;

	while (renderWindow.isOpen())
	{
		calculateDeltaTime();
		getInputs();
		update();
		draw();
	}

	if (!unload())return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

bool Game::init()
{
	if (!ContentPipeline::getInstance().loadContent()) return false;
	if (!music.openFromFile("Ressources\\Sounds\\Music\\Carpenter_brut_remorse.ogg")) return false;

	field.setTexture(ContentPipeline::getInstance().getBackgroundTexture());
	field.setPosition(0, 0);
	field.setColor(Color(193, 68, 14, 255)); //Couleur "Rouge sol de Mars"

	player.init(ContentPipeline::getInstance().getPlayerDeathBuffer());
	player.setPosition(WORLD_CENTER_X, WORLD_CENTER_Y);

	hordeBuffer = ContentPipeline::getInstance().getHordeBuffer();
	hordeSound.setBuffer(hordeBuffer);


	// initialisation des aliens et des scores de chaque aliens
	for (int i = 0; i < ALIENS; i++)
	{
		aliens[i].init(rand() % 3, ContentPipeline::getInstance().getAlienDeathBuffer());
	}

	// initialisation des projectiles (bullets et boost)
	for (int i = 0; i < BULLETS; i++)
	{
		bullets[i].init(1, ContentPipeline::getInstance().getShotBuffer());
		bullets[i].setPosition(WORLD_WIDTH + 200, WORLD_HEIGHT + 200);
		bullets[i].deactivate();

		boostBullets[i].init(0, ContentPipeline::getInstance().getBoostedShotBuffer());
		boostBullets[i].setPosition(WORLD_WIDTH + 200, WORLD_HEIGHT + 200);
		boostBullets[i].deactivate();
	}

	// initialisation des powerups (nukes et boosts)
	for (int i = 0; i < POWER_UP; i++)
	{
		nukes[i].init(1);
		nukes[i].setPosition(WORLD_WIDTH + 200, WORLD_HEIGHT + 200);
		nukes[i].deactivate();

		boosts[i].init(0); 
		boosts[i].setPosition(WORLD_WIDTH + 200, WORLD_HEIGHT + 200); 
		boosts[i].deactivate(); 
	}

	hud.hudInit();	

	score.scoreInit();

	health.healthInit();

	pause.pauseInit();

	gameOver.gameOverInit();

	player.activate();
	music.setLoop(true);

	// Jai mis le volume à 10 car le son est trop fort
	music.setVolume(10);
	
	music.play();
	hordeSound.play();
	return true;
}

void Game::getInputs()
{
	inputs.reset();

	//On passe l'événement en référence et celui-ci est chargé du dernier événement reçu!
	while (renderWindow.pollEvent(event))
	{
		//x sur la fenêtre
		if (event.type == Event::Closed)
			renderWindow.close();
	}

	if (Joystick::isConnected(0))
	{
		inputs.moveHorizontal = inputs.manageGamepadAxis(Joystick::getAxisPosition(0, Joystick::Axis::X));
		inputs.moveVertical = inputs.manageGamepadAxis(Joystick::getAxisPosition(0, Joystick::Axis::Y));

		if (Joystick::isButtonPressed(0, 7) && !inputs.pausePressed) 
		{
			if (!inputs.pause)
			{
				inputs.pause = true;
			}
			else 
			{
				inputs.pause = false;
			}
			inputs.pausePressed = true;
		}
		
		if (!Joystick::isButtonPressed(0, 7)) inputs.pausePressed = false; // On dit a l'input que le bouton a été relaché

		float uAxisPosition = Joystick::getAxisPosition(0, Joystick::Axis::U);
		float vAxisPosition = Joystick::getAxisPosition(0, Joystick::Axis::V);

		if (abs(uAxisPosition) > inputs.DEAD_ZONE || abs(vAxisPosition) > inputs.DEAD_ZONE)
		{
			inputs.rightStickHorizontal = inputs.manageGamepadAxis(uAxisPosition) * CONTROLLER_MULT;
			inputs.rightStickVertical = inputs.manageGamepadAxis(vAxisPosition) * CONTROLLER_MULT;
			inputs.changeRotation = true;
			inputs.mousePosition = Vector2f(player.getPosition().x + inputs.rightStickHorizontal, player.getPosition().y + inputs.rightStickVertical);
		}
		else 
		{
			inputs.changeRotation = false;
		}

		
		if (Joystick::isButtonPressed(0, 5)) 
		{
			if (!inputs.hasFired) 
			{
				inputs.fireTimer.restart();
				inputs.fire = true;
				inputs.hasFired = true;
			}

			if (inputs.fireTimer.getElapsedTime().asSeconds() >= SHOOTING_SPEED && !player.boostActive)
			{
				inputs.fire = true;
				inputs.hasFired = true;
				inputs.fireTimer.restart();
			}
			else if(inputs.fireTimer.getElapsedTime().asSeconds() >= BOOST_SHOOTING_SPEED && player.boostActive)
			{
				inputs.fire = true;
				inputs.hasFired = true;
				inputs.fireTimer.restart();
			}
		}
		else 
		{
			inputs.hasFired = false;
		}
		inputs.isGamepadActive = true;
	}
	else
	{	
		inputs.mousePosition = renderWindow.mapPixelToCoords(Mouse::getPosition(renderWindow));
		inputs.changeRotation = true;
		inputs.rightStickHorizontal = inputs.mousePosition.x;
		inputs.rightStickVertical = inputs.mousePosition.y;

		if (Keyboard::isKeyPressed(Keyboard::A)) inputs.moveHorizontal--; //On décrémente la position x
		if (Keyboard::isKeyPressed(Keyboard::D)) inputs.moveHorizontal++; //On incrémente la position x
		if (Keyboard::isKeyPressed(Keyboard::W)) inputs.moveVertical--; //On décrémente la position y
		if (Keyboard::isKeyPressed(Keyboard::S)) inputs.moveVertical++; //On incrémente la position y

		if (Mouse::isButtonPressed(Mouse::Left)) 
		{
			if (!inputs.hasFired)
			{
				inputs.fireTimer.restart(); // Restart the timer when the button is initially pressed
				inputs.fire = true;
				inputs.hasFired = true;
			}

			if (inputs.fireTimer.getElapsedTime().asSeconds() >= SHOOTING_SPEED && !player.boostActive)
			{
				inputs.fire = true;
				inputs.hasFired = true;
				inputs.fireTimer.restart();
			}
			else if (inputs.fireTimer.getElapsedTime().asSeconds() >= BOOST_SHOOTING_SPEED && player.boostActive)
			{
				inputs.fire = true;
				inputs.hasFired = true;
				inputs.fireTimer.restart();
			}
		}
		else 
		{
			inputs.hasFired = false;
		}

		if (Keyboard::isKeyPressed(Keyboard::Space) && !inputs.pausePressed)
		{
			if (!inputs.pause) 
			{
				inputs.pause = true;
			}
			else 
			{
				inputs.pause = false;
			}
			inputs.pausePressed = true;
		}

		if (!Keyboard::isKeyPressed(Keyboard::Space)) inputs.pausePressed = false; // On dit a l'input que le bouton a été relaché
	}
}

//Vous devrez centrer la vue sur le player: https://www.sfml-dev.org/tutorials/2.6/graphics-view-fr.php
void Game::update()
{
	//On peut déplacer la vue, mais on peut aussi la centrer sur une position précise, 
	//comme celle du joueur (avec la méthode setCenter).  Quand votre joueur va se déplacer 
	//vous devrez centrer la vue sur lui.
	if (!gameOver.getGameOver())
	{
		if (inputs.pause) 
		{
			pause.setPause(true);
			alienSpawnClock.restart();//On remet le spawnClock à 0 pour éviter q'un alien spawn instantanément après avoir quitté le pause
			pause.draw(renderWindow);
		}
		else if (!inputs.pause) 
		{
			pause.setPause(false);
			pause.draw(renderWindow);

			mainView.move(inputs.moveHorizontal * MOVE_SPEED, inputs.moveVertical * MOVE_SPEED);

			
			mainView.setCenter(player.getPosition());

			spawnAlien();
			manageAliens();

			if(player.boostActive)
				manageBullets(boostBullets);
			else
				manageBullets(bullets);

			manageBulletMovement();

			manageBulletCollision(bullets, false);
			manageBulletCollision(boostBullets, true);

			managePowerUpLife(nukes);
			managePowerUpLife(boosts);

			managePowerUpCollision();	

			managePlayer();

			deactivateAlienScores();


			ajustCrossingWorldLimits();
		}
	}
	else
	{
		gameOver.draw(renderWindow);
	}
	
}

void Game::draw()
{
	//Toujours important d'effacer l'écran précédent
	renderWindow.clear();
	renderWindow.setView(mainView);
	renderWindow.draw(field);

	for (int i = 0; i < ALIENS; i++)
	{
		aliens[i].draw(renderWindow);
	}
		

	for (int i = 0; i < BULLETS; i++)
	{
		bullets[i].draw(renderWindow);
		boostBullets[i].draw(renderWindow);
	}
	
	for (int i = 0; i < POWER_UP; i++) 
	{
		nukes[i].draw(renderWindow);
		boosts[i].draw(renderWindow);
	}
		


	player.draw(renderWindow);
	
	renderWindow.setView(hudView);
	hud.draw(renderWindow);
	score.draw(renderWindow);
	health.draw(renderWindow);

	pause.draw(renderWindow);
	gameOver.draw(renderWindow);

	renderWindow.setView(mainView);

	renderWindow.display();
}

bool Game::unload()
{
	return true;
}

void Game::calculateDeltaTime()
{
	deltaTime = clock.restart().asSeconds();
}

void Game::ajustCrossingWorldLimits()
{
	if (mainView.getCenter().x < WORLD_LIMIT_MIN_X)
		mainView.setCenter(WORLD_LIMIT_MIN_X, mainView.getCenter().y);
	else if (mainView.getCenter().x > WORLD_LIMIT_MAX_X)
		mainView.setCenter(WORLD_LIMIT_MAX_X, mainView.getCenter().y);

	if (mainView.getCenter().y < WORLD_LIMIT_MIN_Y)
		mainView.setCenter(mainView.getCenter().x, WORLD_LIMIT_MIN_Y);
	else if (mainView.getCenter().y > WORLD_LIMIT_MAX_Y)
		mainView.setCenter(mainView.getCenter().x, WORLD_LIMIT_MAX_Y);
}

void Game::managePlayer() 
{
	if (!player.boostActive && respawnClock.getElapsedTime().asSeconds() >= BOOST_LIFE)
		player.setColor(WHITE);

	// On normalise le vecteur de direction pour que le joueur se déplace à une vitesse constante
	if (inputs.moveHorizontal != 0.0f && inputs.moveVertical != 0.0f) 
	{
		float length = sqrt(inputs.moveHorizontal * inputs.moveHorizontal + inputs.moveVertical * inputs.moveVertical);

		inputs.moveHorizontal /= length;
		inputs.moveVertical /= length;
	}

	if (!player.boostActive)
	{
		player.move(inputs.moveHorizontal * MOVE_SPEED, inputs.moveVertical * MOVE_SPEED);
	}
	else
	{
		player.move(inputs.moveHorizontal * BOOST_SPEED, inputs.moveVertical * BOOST_SPEED);
	}

	player.keepInBound(WORLD_WIDTH, WORLD_HEIGHT);

	if (inputs.changeRotation)
		player.rotateToMousePosition(inputs.mousePosition);

	if (player.boostTimer.getElapsedTime().asSeconds() >= player.boostDuration)
	{
		player.resetBoost();
	}
	else if (player.boostDuration > 0)
	{
		player.setColor(BOOST_COLOR);
	}
}

void Game::manageBullets(Bullets bulletType[BULLETS])
{
	//fire bullet if the fire button is pressed
	if (inputs.fire && player.isActive())
	{
		for (int i = 0; i < BULLETS; i++)
		{
			if (!bulletType[i].isActive())
			{
				//controller
				if (!bulletType[i].isActive() && inputs.isGamepadActive)
				{
					bulletType[i].fireBulletController(player.getPosition(), inputs.rightStickHorizontal, inputs.rightStickVertical);
				}
				//mouse and keyboard
				else if (!bulletType[i].isActive())
				{
					bulletType[i].fireBulletMouse(player.getPosition(), inputs.mousePosition);
				}
				bulletType[i].bulletSound.play();
				return;
			}
			
		}
	}
}

void Game::manageBulletCollision(Bullets bulletType[BULLETS], bool isBoost)
{
	for (int i = 0; i < BULLETS; i++) 
	{
		if (bulletType[i].isActive())
		{
			for (int j = 0; j < ALIENS; j++)
			{
				if (aliens[j].isActive() && bulletType[i].isCircleColliding(aliens[j]) && aliens[j].life.getElapsedTime().asSeconds() >= ALIEN_SPAWN_SHIELD)
				{
					// Donne la bonne valeur de score en fonction du combo
					manageCombo(j);

					// Essaye de faire spawn un powerup
					if(!managePowerUpSpawn(aliens[j].getPosition(), nukes)) //Essaye de faire spawn un nuke
						managePowerUpSpawn(aliens[j].getPosition(), boosts); //Sinon, essaye de faire spawn un boost

					if (!isBoost)
					{
						bulletType[i].setPosition(WORLD_WIDTH + 200, WORLD_HEIGHT + 200);
						bulletType[i].deactivate();
					}
					
					killAlien(j);
				}
			}
		}
	}
}

void Game::manageBulletMovement() 
{
	for (int i = 0; i < BULLETS; i++)
	{
		if (bullets[i].isActive())
		{
			bullets[i].moveBullet(BULLET_SPEED);
		}

		if (boostBullets[i].isActive())
		{
			boostBullets[i].moveBullet(BOOST_BULLET_SPEED);
		}
	}
}

void Game::manageAliens() 
{		
	for (int i = 0; i < ALIENS; i++)
	{
		if (aliens[i].isActive()) 
		{
			// Calculate direction vector from alien to player
			sf::Vector2f direction = player.getPosition() - aliens[i].getPosition();

			// Normalize the direction vector so aliens move at a constant speed
			float length = sqrt(direction.x * direction.x + direction.y * direction.y);
			if (length != 0)
			{
				direction.x /= length;
				direction.y /= length;
			}

			if (!player.boostActive)
			{
				// Check for collision with player after 3 seconds since respawn
				if (player.isCircleColliding(aliens[i]) && (respawnClock.getElapsedTime().asSeconds() >= PLAYER_RESPAWN_SHIELD || health.healthPoints == BASE_HEALTH_POINTS))
				{
					health.updateHealth();
					// death sound
					player.playerSound.play();
					respawnClock.restart();
					for (int y = 0; y < ALIENS; y++)
						aliens[y].rotate(180);
					player.deactivate();
				}
			}
			else if(player.boostActive && player.isCircleColliding(aliens[i]))
			{
				player.boostActive = false;
				player.boostDuration = 0;

				killAlien(i);
			}
			

			// Move alien towards player
			if (player.isActive() && aliens[i].life.getElapsedTime().asSeconds() >= DEATH_TIMER)
			{
				aliens[i].move(direction.x * ALIEN_SPEED, direction.y * ALIEN_SPEED);
				float angle = atan2(direction.y, direction.x) * 180 / PI;
				aliens[i].setRotation(angle);
			}
			// Move alien away from player
			else if (!player.isActive()) 
			{
				aliens[i].move(-direction.x * ALIEN_SPEED, -direction.y * ALIEN_SPEED);
			}
		}
	}

	if (!player.isActive() && respawnClock.getElapsedTime().asSeconds() >= DEATH_TIMER && health.getHealth() > DEAD)
	{
		hordeSound.play();
		player.activate();
		player.setColor(RESPAWN_COLOR);
	}
	else if (health.getHealth() <= 0) 
	{
		gameOver.updateGameOver();
	}
}

void Game::spawnAlien() 
{	
	for (int i = 0; i < 50; i++)
	{
		// Animation de l'alien qui apparait
		if (aliens[i].isActive() && aliens[i].life.getElapsedTime().asSeconds() < ALIEN_ANIMATION)
		{
			aliens[i].rotate(6);// 360 / 60 = 6 ---- 60 vient du nombre de fois que la fonction est appelé par nouvel alien
			aliens[i].setScale(aliens[i].currentScale += 0.01f, aliens[i].currentScale += 0.01f);//Augmente la taille de l'alien graduellement
		}
		else if (aliens[i].isActive() && aliens[i].life.getElapsedTime().asSeconds() > ALIEN_ANIMATION)
		{
			aliens[i].setScale(1, 1); // Mettre a grandeur normal au cas ou il y a un bug
		}

		if (!aliens[i].isActive() && !aliens[i].score.isActive())
		{
			// Spawn one per second
			if (alienSpawnClock.getElapsedTime().asSeconds() >= spawnInterval)
			{
				// make sure alien is spawned in the world in a radius around the player
				while (aliens[i].getPosition().x > WORLD_WIDTH || aliens[i].getPosition().x < 0 || aliens[i].getPosition().y > WORLD_HEIGHT || aliens[i].getPosition().y < 0)
				{

					// ****************************************************************************************************
					// Début du code obtenu avec chatGPT et modifié par la suite

					float angle = static_cast<float>(rand() % 360); // Random angle in degrees

					// Calculate random coordinates outside of the circular area
					float x = player.getPosition().x + SPAWN_RADIUS * cos(angle * PI / 180.0f);
					float y = player.getPosition().y + SPAWN_RADIUS * sin(angle * PI / 180.0f);
					
					// Fin du code obtenu avec chatGPT et modifié par la suite
					// ****************************************************************************************************

					aliens[i].setPosition(x, y);
					aliens[i].setScale(0, 0);
					aliens[i].currentScale = 0;
					aliens[i].life.restart();
					alienSpawnClock.restart();
				}
				aliens[i].activate();
				return;
			}
			
			
		}
	}
	
}

bool Game::managePowerUpSpawn(const Vector2f alienPosition, PowerUp powerUp[POWER_UP])
{
	// 4% de chance de spawn un powerup
	if (rand() % 100 < PERCENTAGE_POWER_UP_SPAWN) 
	{
		for (int i = 0; i < POWER_UP; i++) 
		{
			if (!powerUp[i].isActive()) 
			{
				powerUp[i].setPosition(alienPosition);
				powerUp[i].lifeTimer.restart();
				powerUp[i].activate();
				return true;
			}
		}
	}
	return false;
}

void Game::managePowerUpLife(PowerUp powerUp[POWER_UP])
{
	for (int i = 0; i < POWER_UP; i++) 
	{
		if (powerUp[i].lifeTimer.getElapsedTime().asSeconds() >= POWER_UP_LIFE) 
		{
			powerUp[i].setPosition(WORLD_WIDTH + 200, WORLD_HEIGHT + 200);
			powerUp[i].deactivate();
		}
	}
}

void Game::managePowerUpCollision()
{
	for (int i = 0; i < POWER_UP; i++)
	{
		if (nukes[i].isActive() && player.isCircleColliding(nukes[i]))
		{
			for (int j = 0; j < ALIENS; j++)
			{
				if (aliens[j].isActive())
				{
					manageCombo(j);

					if (score.getHiddenScore() >= HEALTH_UP) 
					{
						score.resetHiddenScore();
						health.addHealth();
					}

					killAlien(j);
				}
			}
			nukes[i].nukeSound.play();
			nukes[i].setPosition(WORLD_WIDTH + 200, WORLD_HEIGHT + 200);
			nukes[i].deactivate();
		}

		if (boosts[i].isActive() && player.isCircleColliding(boosts[i]))
		{
			if(!player.boostActive)
				player.boostTimer.restart();

			player.boostActive = true;
			player.boostDuration += BOOST_DURATION;
			boosts[i].boostSound.play();
			boosts[i].setPosition(WORLD_WIDTH + 200, WORLD_HEIGHT + 200);
			boosts[i].deactivate();
		}
	}
}	

void Game::killAlien(const int alien)
{
	aliens[alien].manageAlienScore(score.alienValue, renderWindow);
	aliens[alien].kill(); 
}

void Game::deactivateAlienScores()
{
	for (int i = 0; i < ALIENS; i++)
	{
		aliens[i].deactivate(); //Desactive les aliens et les scores des aliens
	}
}

void Game::manageCombo(const int alien)
{

	if (!score.comboActive) //Si le combo n'est pas actif
	{
		score.comboClock.restart();
		score.comboActive = true;
		score.alienValue = BASE_ALIEN_VALUE;
	}
	else if (score.comboClock.getElapsedTime().asSeconds() <= COMBO_TIMER) //Si le combo est actif et que le temps n'est pas écoulé
	{
		score.comboClock.restart();
		if (score.alienValue < MAX_ALIEN_VALUE)
			score.alienValue += COMBO_SCORE;
	}
	else //Si le combo est actif et que le temps est écoulé
	{
		score.comboActive = false;
		score.alienValue = BASE_ALIEN_VALUE;
	}

	score.updateScore(score.alienValue); //Met à jour le score

	if (score.getHiddenScore() >= HEALTH_UP) //Si le score caché est plus grand que le seuil pour augmenter la vie
	{
		score.resetHiddenScore();
		health.addHealth();
	}
}