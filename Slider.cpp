#include "Slider.h"

Slider::Slider(std::string name, int level, sf::RectangleShape rect) {
	bound = rect;
	bound.setFillColor(sf::Color::Transparent);
	bound.setOutlineColor(sf::Color::Red);
	bound.setOutlineThickness(.01f);

	focusRect = bound;
	focusRect.setPosition(bound.getPosition() + sf::Vector2f(1, 1));
	focusRect.setSize(bound.getSize() - sf::Vector2f(2, 2));
	focusRect.setFillColor(sf::Color::Transparent);
	focusRect.setOutlineColor(sf::Color::White);
	focusRect.setOutlineThickness(1);

	if (!font.loadFromFile("C:/Windows/Fonts/segoeui.ttf")) printf("Loading font error\n");

	this->name = name;
	nameText.setFont(font);
	nameText.setString(name);
	nameText.setPosition(bound.getPosition() + sf::Vector2f(10, 10));
	nameText.setFillColor(sf::Color::White);
	nameText.setCharacterSize(18);
	nameText.setOutlineThickness(.01f);

	this->level = level;
	levelText.setFont(font);
	levelText.setString(std::to_string(100));
	levelText.setPosition(bound.getPosition() + bound.getSize() - sf::Vector2f(levelText.getGlobalBounds().width + 20, levelText.getGlobalBounds().height + 25));
	levelText.setFillColor(sf::Color::White);
	levelText.setCharacterSize(30);
	levelText.setString(std::to_string(level));
	nameText.setOutlineThickness(1);
	
	background.setPosition(bound.getPosition() + sf::Vector2f(20, 50));
	background.setSize(sf::Vector2f(300, 8));
	background.setFillColor(sf::Color(60, 60, 60));

	foreground.setPosition(background.getPosition());
	foreground.setSize(sf::Vector2f(background.getSize().x * level / 100, background.getSize().y));
	foreground.setFillColor(sf::Color(0, 162, 237));
}

void Slider::render(sf::RenderWindow*& renderer) {
	renderer->draw(nameText);
	renderer->draw(levelText);

	renderer->draw(background);
	renderer->draw(foreground);

	if (focus) renderer->draw(focusRect);
}

void Slider::setLevel(int level) {
	this->level = level;

	levelText.setString(std::to_string(this->level));
	foreground.setSize(sf::Vector2f(background.getSize().x * this->level / 100, background.getSize().y));
}

void Slider::setFocus(int f) {
	this->focus = f;
}