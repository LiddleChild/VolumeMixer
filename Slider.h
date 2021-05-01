#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "Process.h"

class Slider {

private:
	sf::Font font;
	std::string name;
	int level;
	sf::Text nameText, levelText;

	sf::RectangleShape background, foreground;

	sf::RectangleShape bound, focusRect;

	int focus = 0;

public:
	Slider(std::string, int, sf::RectangleShape);

	void setLevel(int);
	void render(sf::RenderWindow*&);
	void setFocus(int);

};