#include "Alien.h"
#include "ContentPipeline.h"
#include "Constants.h"

Alien::Alien()
{
}

void Alien::init(const int alienTextureNumber, const SoundBuffer& sound)
{
	// R�cup�re la texture de l'alien depuis la ContentPipeline et configure sa position
	setTexture(ContentPipeline::getInstance().getAlienTexture(alienTextureNumber));
	setPosition(WORLD_WIDTH + 200, WORLD_HEIGHT + 200);
	deactivate();
	
	// Initialise le buffer audio de l'alien et configure le son de l'alien
	alienBuffer = sound;
	alienSound.setBuffer(alienBuffer);

	// Initialise le score de l'alien
	score.init();
	score.setPosition(getPosition().x, getPosition().y);
	score.deactivate();
}

void Alien::draw(sf::RenderWindow& renderWindow)
{
	if (isActive())
	{
		GameObject::draw(renderWindow);
		score.draw(renderWindow);
	}
}

void Alien::kill()
{
	// Joue le son de l'alien lorsqu'il est tu� et le d�place hors de l'�cran
	alienSound.play();
	setPosition(WORLD_WIDTH + 200, WORLD_HEIGHT + 200);
	currentScale = 0;
}

// S'occupe du score de l'alien lorsqu'il est tu�
void Alien::manageAlienScore(const int alienValue, sf::RenderWindow& renderWindow)
{
	score.activate();//active le score
	score.setPosition(getPosition().x, getPosition().y);//positionne le score sur l'alien
	score.scoreText.setPosition(getPosition().x, getPosition().y);//positionne le texte du score sur l'alien
	score.updateScore(alienValue);//met � jour le score
	score.draw(renderWindow);//affiche le score
	score.timer.restart();//relance le timer du score
}

// D�sactive l'alien et le score associ� si le score est actif et que le timer du score est sup�rieur � 1 seconde
void Alien::deactivate()
{
	if (score.isActive() && score.timer.getElapsedTime().asSeconds() >= SCORE_LIFE)
	{
		GameObject::deactivate();
		score.setPosition(WORLD_WIDTH + 200, WORLD_HEIGHT + 200);
		score.scoreText.setPosition(WORLD_WIDTH + 200, WORLD_HEIGHT + 200);
		score.deactivate();
	}
}
