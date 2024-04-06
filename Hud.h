#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Hud
{
public:
	Hud();
	void hudInit();
	void draw(RenderWindow& renderWindow);

private:
	Text bidon;
};

